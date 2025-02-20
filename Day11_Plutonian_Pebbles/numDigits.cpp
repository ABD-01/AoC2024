#include <iostream>
#include <chrono>
#include <string>
#include <cmath>
#include <climits>

int numDigits(unsigned long long i)
{
    int n = 1;
    if ( i >= 10000000000000000 ) { n += 16; i /= 10000000000000000; }
    if ( i >= 100000000         ) { n += 8; i /= 100000000; }
    if ( i >= 10000             ) { n += 4; i /= 10000; }
    if ( i >= 100               ) { n += 2; i /= 100; }
    if ( i >= 10                ) { n += 1; }

    return n;
    // ref: https://stackoverflow.com/a/6655759
}

int numDigits_log(unsigned long long i) {
    return (i == 0) ? 1 : std::log10(i) + 1;
}

int numDigits_div(unsigned long long i) {
    int n = 1;
    while (i >= 10) {
        n++;
        i /= 10;
    }
    return n;
}

int numDigits_str(unsigned long long i) {
    return std::to_string(i).length();
}

int numDigits_builtin(unsigned long long i) {
    return 1 + (i ? (63 - __builtin_clzll(i)) * 1233 >> 12 : 0);
}


template<typename Func>
void benchmark(Func func, unsigned long long val, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) func(val);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() 
              << " ns\n";
}

int main() {
    unsigned long long val /*= 12345678901234567ULL*/;
    int iterations = 1000000;
    std::cin >> val;

    std::cout << "Stackoverflow: "; benchmark(numDigits, val, iterations);
    std::cout << "Logarithmic: "; benchmark(numDigits_log, val, iterations);
    std::cout << "Iterative Division: "; benchmark(numDigits_div, val, iterations);
    std::cout << "String Conversion: "; benchmark(numDigits_str, val, iterations);
    std::cout << "Builtin CLZ: "; benchmark(numDigits_builtin, val, iterations);

    return 0;
}
