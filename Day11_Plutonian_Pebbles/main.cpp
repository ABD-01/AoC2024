#include <iostream>
#include <sstream>
#include <fstream> // contains ifstream and ofstream
#include <iterator>
#include <chrono>
#include <vector>
#include <stack>

#define MAX_NUM_BLINKS 75

#define DEBUG_PRINT 0

#if DEBUG_PRINT
#define DEBUG(x) std::cout << x
#else
#define DEBUG(x)
#endif

using std::cout;
using std::cerr;
using std::endl;
using ull = unsigned long long;
// Max Value: 18,446,744,073,709,551,615 

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

    
void part(const std::vector<ull>& pebbles, bool=false);
int numDigits(ull i);

std::vector<std::vector<ull>> cache(10, std::vector<ull>(MAX_NUM_BLINKS, 0));
// stores the resulting number of stones for values from 0 to 9
// cache[v][b-1] represent number of pebbles after blinking b times starting with pebble of value v
ull g_numShortCircuited = 0;

void fill_cache(int); // TODO: make this constexpr

int main(int argc, char* argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << argv[1] << endl;
        return 1;
    }

    std::vector<ull> pebbles = {std::istream_iterator<ull>(file), std::istream_iterator<ull>()};

#if DEBUG_PRINT
    for(ull p: pebbles)
        cout << p << " ";
    cout << endl;
#endif

    fill_cache(MAX_NUM_BLINKS);

    part(pebbles);
    part(pebbles, true);

#if DEBUG_PRINT
    for(auto c: cache)
    {
        for(auto i: c) cout << i << " ";
        cout << endl;
    }
#endif
}

void blink(ull value, ull& result, int numBlinks,  int depth=0);

void part(const std::vector<ull>& pebbles, bool run_part2)
{
    Timer t;
    // assuming the worst case scenario
    // a pebble split into 2 at every step, after n steps for m pebbles there would be m*2^n pebbles
    // 2^25 = 33554432, no point in storing all the result.
    // only count when the stones split and forget

    int numBlinks(run_part2 ? MAX_NUM_BLINKS: 25);
    ull numPebbles(0);

    for(ull pebble: pebbles)
    {
        ull n = 1;
        blink(pebble, n, numBlinks);

        numPebbles += n;
        DEBUG("Pebble: " << pebble << " split into " << n << " pebbles, after " << numBlinks << " blinks" << endl);
    }
    cout << "Part " << (run_part2?"2":"1")  <<  ": " << numPebbles << endl;
}

int numDigits(ull i)
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

void fill_cache(int numBlinks)
{
    for(auto nb = 0; nb < numBlinks; ++nb)
    {
        for(ull i = 0; i < 10; ++i)
        {
            ull r = 1;
            blink(i, r, nb+1);
            cache[i][nb] = r;
        }
    }
}

void blink(ull value, ull& result, int numBlinks,  int depth)
{
    DEBUG("blinks: " << depth << " P: " << value << " " <<  "Number of pebbles: " << result << endl);
    if(depth > numBlinks - 1)
    {
        return;
    }
    if(value < 10)
    {
        if (cache[value][numBlinks - depth - 1] != 0)
        {
            g_numShortCircuited++;
            DEBUG("Short circuited (" << g_numShortCircuited << ")" << endl);
            DEBUG("Pebble " << value << " after " << numBlinks - depth << " blinks will be split into " << cache[value][numBlinks-1 - depth] << endl);
            result += (cache[value][numBlinks-1 - depth] - 1); // that -1 is there because the current stone is being counted twice.
            return;
        }
    }

    if(value == 0) 
    {
        value = 1;
        return blink(value, result, numBlinks, depth+1);
    }
    int n = numDigits(value);
    if (!(n%2)) // n is even
    {
        ull splitValue = 0;
        int lastDigit = 0;
        int base10 = 1;
        for(auto i = 0; i < n/2; ++i)
        {
            lastDigit = value % 10;
            value /= 10;
            splitValue += (lastDigit * base10);
            base10 *= 10;
        }
        result++;
        DEBUG("Split into " << value << " and " << splitValue << endl);
        blink(value, result, numBlinks, depth+1);
        blink(splitValue, result, numBlinks, depth+1);
        return;
    }
    value = value * 2024;
    return blink(value, result, numBlinks, depth+1);
}