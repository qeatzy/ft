#include "utility.h"

int main() {
    std::ifstream file("/cygdrive/c/Users/Dell/code/qeatzy/ft/_data/transformed_tian.csv");
    llong size = 0;
    string buf;
    buf.reserve(500);
    auto start = std::chrono::high_resolution_clock::now();
    while (getline(file, buf)) {
        // cout << buf << '\n';
        size += buf.size();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << diff.count() << " s\n";
    cout << "size = " << size;
}

