#include "utility.h"
#include "ar.h"


bool LUSolver(Matrix &mat) {
    /** do LU decomposition, L is mat[i][j] i>j, else (upper right) is U.
     * mat, square matrix append with b, i.e., of size m * (m+1).
     * return, false if failed, eg, singular matrix.
     * CAUTION, if mat is not append with b, the permutation is lost and thus can't be
     * used to solve the original linear equation.
     * PA = LU, Gaussian elimination with partial pivoting.
     * ref to Trefethen and Bau (1997) Lecture 21 Pivoting
     */
    // (0). start with U = A, L = I, P = I.
    // the use of computer memory can be minimized if desired by overwriting U and L into the same array used to store A.
    double max_val, tmp, pivot, normalizer;
    size_t m = mat.size(), n = mat[0].size(), max_row, k,j,h;
    // printf("m = %zu, n = %zu\n", m,n);

    for(k=0; k<m-1; ++k) {    // column
        // (1). partial pivoting
        // select i such that U[i][k] is largest;
        // swap U[i] and U[k], L[i] and L[k], P[i] and P[k]
        max_row = k;
        max_val = fabs(mat[k][k]);
        for(j=k+1; j<m; ++j) {
            if ((tmp = fabs(mat[j][k])) > max_val) {
                max_row = j;
                max_val = tmp;
            }
        }
        if (max_row != k) { // permutation, could be returned instead of modify argument.
            std::iter_swap(mat.begin() + k, mat.begin() + max_row);
        }
        // (2). elimination, for each in lower part of pivot column, sum scaled pivot row to eliminate it.
        // L[j][k] = U[j][k] / U[k][k]
        // U[j][k:] = U[j][k:] - l[j][k] * U[k][k:]

        pivot = mat[k][k];
        if (fabs(pivot) == 0) { // TODO or less than some adequately small number?
            std::cerr << "Singular matrix - fatal!\n";
            return false;
        }
        for(j=k+1; j<m; ++j) {    // eliminate lower part of current column
            normalizer =  mat[j][k] / pivot;
            mat[j][k] = normalizer; // fill element of L at current position. or inverse of L?
            for(h=k+1; h<n; ++h) {
                mat[j][h] -= normalizer * mat[k][h]; // normalize remaider part of current row.
            }
            // print(mat);
            // wait();
        }

    }
    return true;
}

dVec LUSolver(const Matrix &mat, const dVec &target) {
    /** solve linear equation of square size.
     * mat, A in Ax=b, of size n * n, n>0.
     * target, b in Ax=b, of size n.
     * return, x in Ax=b.
     * TODO what if A is singular?
     * ref to Trefethen and Bau (1997) Lecture 17 Stability of Back Substitution
     */
    Matrix emat(mat);
    for(size_t i=0; i<emat.size(); ++i) {
        emat[i].push_back(target[i]);
    }
    // Gaussian elimination
    if(!LUSolver(emat)) {
        std::cerr << "LU decomposition failed. Fatal!\n";
        exit(1);
    }

    dVec sol;
    for(const auto &v: emat) {
        sol.push_back(v.back());  // get possibly permuted copy of argument target.
    }
    // back substitution, for each in right part of pivot row, sum scaled rows of below to eliminate it.
    // the lower part submatrix is identity matrix now.
    for(size_t i = 1; i <= emat.size(); ++i) {
        size_t row = emat.size() - i;                               // from bottom row upward.
        for(size_t r_col = row + 1; r_col < emat.size(); ++r_col) {
            sol[row] -= sol[r_col] * emat[row][r_col];              // eliminate right side off-diagonal element of U in current row.
        }
        sol[row] /= emat[row][row];                                 // normalize diagonal of U in current row.
    }
    return sol;
}

// Matrix AutoRegression(dVec inputseries, size_t degree, std::string method) {
dVec AutoRegression(dVec inputseries, size_t degree, std::string method) {
    double mean = std::accumulate(inputseries.begin(), inputseries.end(), 0) / inputseries.size();
    // std::cout << "mean of inputseries = " << mean << '\n';
    std::for_each(inputseries.begin(), inputseries.end(), [mean](double &x) { x -= mean; });
    // std::cout << "degree = " << degree <<'\n';
    dVec coefficients(degree);
    Matrix mat(degree);
    std::for_each(mat.begin(), mat.end(), [degree](std::vector<double> &x) { x.resize(degree); });
    // std::cout << "here 7\n";

   //  print(inputseries);
   //  std::cout << std::endl;
   // #<{(|<*************************|)}>#
   //  for(auto v: mat) print(v);
   //  std::cout << std::endl;
   // #<{(|>*************************|)}>#

    for(auto i=degree-1; i<inputseries.size() - 1; ++i) {
        // std::cout << "i = " << i << '\n';
        int hi = i + 1;
        for(size_t j=0; j<degree; ++j) {
            size_t hj = i - j;
            coefficients[j] += inputseries[hi] * inputseries[hj];
            for(size_t k=j; k<degree; ++k) {
                mat[j][k] += inputseries[hj] * inputseries[i-k];
            }
        }
    }

   // #<{(|<*************************|)}>#
   //  for(auto v: mat) print(v);
   //  std::cout << std::endl;
   // #<{(|>*************************|)}>#

    for(size_t i=0; i<degree; ++i) {
        coefficients[i] /= (inputseries.size() - degree);
        for(auto j=i; j<degree; ++j) {
            mat[i][j] /= (inputseries.size() - degree);
            mat[j][i] = mat[i][j];
        }
    }

   // #<{(|<*************************|)}>#
   //  for(auto v: mat) print(v);
   //  std::cout << std::endl;
   // #<{(|>*************************|)}>#

    dVec res = LUSolver(mat, coefficients);
    return res;
}

namespace ar {
    class test {
        public:
            static void LUSolver();
            static void AutoRegression();
    };
}

void ar::test::LUSolver() {
    std::vector<double> vec {1,2,5,3};
    Matrix mat { {2,1,1,0}, {4,3,3,1}, {8,7,9,5}, {6,7,9,8} };
    // Matrix mat { {1,3,3,2}, {2,6,9,7}, {-1,-3,3,4} };
    // std::iter_swap(mat.begin() + 1, mat.begin() + 2);
    dVec sol = ::LUSolver(mat, vec);    // use name in default namespace
    print(sol);
    // for(auto vec: mat) print(vec);
}

void ar::test::AutoRegression() {
    dVec vec{1,3,2,5,4,6,9,7,8};
    for (size_t i=1; i<8; ++i) {
        int degree = i;
        dVec res = ::AutoRegression(vec, degree, "ols");
        printf("i=%d: ", i);
        print(res);
    }
    // Matrix mat = AutoRegression(vec, 6, "ols");
    // std::cout << "mat.size() = " << mat.size() << '\n';
    // for(dVec v: mat) print(v);
    // std::cout << std::accumulate(vec.begin(), vec.end(), 0);
}
