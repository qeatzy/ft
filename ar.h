#ifndef __AR_H_INCLUDED__   // include guard, http://www.cplusplus.com/forum/articles/10627/#msg49679
#define __AR_H_INCLUDED__ 

typedef std::vector<std::vector<double>> Matrix;
typedef std::vector<double> dVec;

dVec AutoRegression(dVec inputseries, size_t degree, std::string method);
// Matrix AutoRegression(dVec inputseries, size_t degree, std::string method);
dVec LUSolver(const Matrix &mat, const dVec &target);

#endif
