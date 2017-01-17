#include "utility.h"
#include <cfloat>

namespace ft {
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
        res.mean = 0, res.min = DBL_MIN, res.max = DBL_MAX, res.std = 0;
        res.cnt = std::distance(first, last);
        // print(first, last);
        for (auto it = first; it != last; ++it) {
            res.mean += *it;
            if (*it < res.min) {
                res.min = *it;
            } else {
                res.max = *it;
            }
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
}
    using namespace ft;

namespace test {
    // using namespace ft;
    void describe() {
        auto seq = range(-5,10);
        // auto res = ::describe(seq.begin(), seq.end());
        auto res = ft::describe(seq.begin(), seq.end());
        cout << res;
    }
}


int main() {
    test::describe();
}
