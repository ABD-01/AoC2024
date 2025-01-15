#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <chrono>

class Timer {
public:
    Timer() { start = std::chrono::high_resolution_clock::now(); }
    ~Timer() {
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Elapsed time: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
                  << " us" << std::endl;
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v)
{
    os << "[";
    for (auto i = v.begin(); i != v.end(); ++i) {
        if (i != v.begin()) os << ", ";
        os << *i;
    }
    os << "]";
    return os;
}

#endif /* UTIL_H */
