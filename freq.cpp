#include "utility.h"
#include "ft_utility.h"     // PI, M_PI
#include <cfloat>

// namespace ft {
    struct DistValues { // value aggregate for describe distribution characteristic of sample
    public:
        double mean;
        double min;
        double max;
        double std; // standard deviation
        int cnt;    // count of items;
    };
    std::ostream& operator<<(std::ostream& os, const DistValues& x) {
        // return os << x.mean << ", " << x.min << ", " << x.max << ", " << x.std << ", " << x.cnt << endl;
        return os << "mean = " << x.mean << ", min = " << x.min << ", max = " << x.max << ", std = " << x.std << ", cnt = " << x.cnt << endl;
    }
    template <typename ForwardIt>
    DistValues describe(ForwardIt first, ForwardIt last) {
        DistValues res;
        res.mean = 0, res.min = DBL_MAX, res.max = DBL_MIN, res.std = 0;
        res.cnt = std::distance(first, last);
        // print(first, last);
        for (auto it = first; it != last; ++it) {
            res.mean += *it;
            if (*it < res.min) { res.min = *it; }
            if (*it > res.max) { res.max = *it; }
        }
        res.mean /= res.cnt;
        for (auto it = first; it != last; ++it) {
            auto tmp = *it - res.mean;
            res.std += tmp * tmp;
        }
        assert(res.cnt > 1);
        res.std /= res.cnt - 1;
        res.std = std::sqrt(res.std);
        return res;
    }
// }
//     using namespace ft;

struct posi_prob {
	double x;
	double y;
};
std::ostream& operator<<(std::ostream& os, const posi_prob& x) {
    return os << "value = " << x.x << ", prob = " << x.y;
}
bool operator==(const posi_prob& lhs, const posi_prob& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
posi_prob operator-(const posi_prob& lhs, const posi_prob& rhs) {
    posi_prob res;
    res.x = lhs.x - rhs.x;
    res.y = lhs.y - rhs.y;
    return res;
}


double Normal(double x) //概率密度函数
{
	return (1.0 / (sqrt(2.0*PI)))* exp(-1.0*pow(x, 2) / 2.0);
}

vector<posi_prob> KDE(vector<double> &residual, double m_min, double m_max, int num, double average, double sigma, int INTERVAL) {
	vector<posi_prob> pdf;
	double stemp = (m_max - m_min) / INTERVAL;
	double h = pow(4.0 / (3.0*num), 1.0 / 5.0)*sigma;//高斯核的带宽
	for (int i = 0; i < INTERVAL; ++i) {
		posi_prob temp;
		temp.x = m_min + i*stemp+0.5*stemp;
		temp.y = 0;
		for (int j = 0; j < num; ++j) {
			temp.y += Normal((temp.x - residual[j]) / h);
		}
		temp.y /= (num*h);
		pdf.push_back(temp);
	}
	return pdf;
}

vector<posi_prob> KDE_refactored(vector<double> &data, int INTERVAL) {
    auto d = describe(data.begin(), data.end());
	vector<posi_prob> pdf;
    pdf.reserve(INTERVAL);
	const double stemp = (d.max - d.min) / INTERVAL;
	double h = pow(4.0 / (3.0 * d.cnt), 1.0 / 5.0) * d.std;//高斯核的带宽
	for (int i = 0; i < INTERVAL; ++i) {
		posi_prob temp;
		temp.x = d.min + i*stemp+0.5*stemp;
		temp.y = 0;
		for (auto value: data) {
			temp.y += Normal((temp.x - value) / h);
		}
		temp.y /= (d.cnt * h);
		pdf.push_back(temp);
	}
	return pdf;
}

namespace test {
    // using namespace ft;
    void KDE() {
        const int INTERVAL = 100000;
        std::vector<double> seq;
        for (auto i: range(200))
            seq.push_back(i);
        auto res = ::KDE_refactored(seq, INTERVAL);
        auto d = describe(seq.begin(), seq.end());
        auto res_old = ::KDE(seq, d.min, d.max, d.cnt, d.mean, d.std, INTERVAL);
        cout << res.size() << ", " << res_old.size() << endl;
        for (size_t i = 0; i < res.size(); ++i) {
            if (res[i] == res_old[i]) continue;
            // cout << "i = " << i << ", res[i] = " << res[i] << ", res_old[i] = " << res_old[i] << endl;
            if (true) {
            // if (i < 20 || i % 500 == 0) {
                cout << "i = " << i << ", diff = " << res[i] - res_old[i] << endl;
                wait();
            }
        }
        cout << (res == res_old ? "equal" : "not equal");
        // print(res,"KDE_refactored",'\n');
        // print(res_old,"KDE",'\n');
    }
}

int main() {
    test::KDE();
}
