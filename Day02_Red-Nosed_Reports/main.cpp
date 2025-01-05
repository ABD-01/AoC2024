#include <iostream>
#include <sstream>
#include <fstream> // contains ifstream and ofstream
#include <vector>
#include <iterator>
#include <algorithm> // for is_sorted
//#include <ranges> // for ranges, use -std=c++20 during compilation

using std::cout;
using std::cerr;
using std::endl;

int part1(const std::vector< std::vector<int> >& vec, bool);
bool part2(const std::vector<int>& vec);
void getVectors(std::vector<std::vector<int>>& vec, std::istream& in);

bool isReportSafe(const std::vector<int>& v)
{
    bool isSafe = true;
    // Must be all increasing or all decreasing.
    bool isSorted = (v[0] > v[1])
        ? std::is_sorted(v.begin(), v.end(), std::greater<int>{})
        : std::is_sorted(v.begin(), v.end()); 

    if(!isSorted) return false;

    for (std::size_t i = 1; i < v.size(); ++i)
    {
       int diff = abs(v[i]-v[i-1]); 
       // Any two adjacent values must differ by at least one and at most three.
       if (!(1 <= diff && diff<= 3)) 
       {
            isSafe = false;
            break;
       }
    }

    /** Could use std::adjacent_find 
    return std::adjacent_find(v.begin(), v.end(), 
        [](int a, int b) {
            int diff = std::abs(b - a);
            return diff < 1 || diff > 3;
        }) == v.end();    
    */

    return isSafe;
}

int part1(const std::vector< std::vector<int> >& vec, bool run_part2 = false)
{
    int safeReports = 0;
    for (const std::vector<int>& v: vec)
    {
        if(isReportSafe(v))
            safeReports++;
        else if (run_part2)
            safeReports += part2(v);
    }

   return safeReports;
}

// Safe after one tolerance
bool part2(const std::vector<int>& vec)
{
    for(std::size_t i = 0; i < vec.size(); ++i)
    {
        std::vector<int> temp = vec;
        temp.erase(temp.begin() + i);
        if (isReportSafe(temp)) 
            return true;
    }
    return false;
}


int main(int argc, char *argv[])
{

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }

    std::ifstream file;
    file.open(argv[1]);
    
    std::vector< std::vector<int> > vec1;
    getVectors(vec1, file);

    cout << "Part 1: " << endl << part1(vec1) << endl;
    cout << "Part 2: " << endl << part1(vec1, true) << endl;
}


void getVectors(std::vector<std::vector<int>>& vec, std::istream& in)
{
    if (!in) {
        std::cerr << "Error: Invalid input stream." << std::endl;
        return;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        std::vector<int> results(
                (std::istream_iterator<int>(iss)),
                std::istream_iterator<int>()
        );
        // ref: https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/

        vec.push_back(results);
    }
}

