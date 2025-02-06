#include <fstream>
#include <iostream>
#include <sstream>  // For ostringstream
#include <vector>
#include <iterator>
#include <chrono>

#include "util.h"

using std::cout;
using std::cerr;
using std::endl;
using ll = long long;

void part1(const std::vector<std::pair<ll, std::vector<int>>>& data);
void part2(const std::vector<std::pair<ll, std::vector<int>>>& data);

void part1_v2(const std::vector<std::pair<ll, std::vector<int>>>& data);
void part2_v2(const std::vector<std::pair<ll, std::vector<int>>>& data);

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

    std::vector<std::pair<ll, std::vector<int>>> data;
    std::string line;
    while (std::getline(file, line)) {
        auto n = line.find(":");
        if (n == std::string::npos) break;

        ll test_value = std::stoll(line.substr(0, n));
        
        std::istringstream iss(line.substr(n + 1));
        std::vector<int> operands = {
            std::istream_iterator<int>(iss), std::istream_iterator<int>()
        };

        data.emplace_back(test_value, operands);
    }

    {
        Timer t;
        part1(data);
    }
    {
        Timer t;
        part2(data);
    }
    {
        Timer t;
        part1_v2(data);
    }
    {
        Timer t;
        part2_v2(data);
    }

    return 0;
}

bool check_target1(const std::vector<int>& operands, int index, ll current_value, ll target)
{
    if(index==(int)operands.size()) return current_value == target;

    if(current_value > target) return false;

    // Ohh this is so subtle Bug! You only want to check for equality when list is exhausted
    //if(current_value == target) return true;

    return check_target1(operands, index+1, current_value + operands[index], target) 
        || check_target1(operands, index+1, current_value * operands[index], target);
}

void part1(const std::vector<std::pair<ll, std::vector<int>>>& data)
{
    ll result = 0;
    for(const std::pair<ll, std::vector<int>>& d: data)
    {
        if(check_target1(d.second, 0, 1, d.first))
        {
            result += d.first;
        }
    }
    cout << "Part 1: " << result << endl;
}

ll concat(ll a, ll b)    
{
    ll pow = 10;
    while(b>=pow)
    {
        pow *= 10;
    }
    return a*pow + b;
    // ref: https://stackoverflow.com/a/12700533
}

bool check_target2(const std::vector<int>& operands, int index, ll current_value, ll target)
{
    if(index==(int)operands.size()) return current_value == target;

    if(current_value > target) return false;

    return check_target2(operands, index+1, current_value + operands[index], target) 
        || check_target2(operands, index+1, current_value * operands[index], target)
        || check_target2(operands, index+1, concat(current_value, operands[index]), target);
}

void part2(const std::vector<std::pair<ll, std::vector<int>>>& data)
{
    ll result = 0;
    for(const auto& d: data)
    {
        if(check_target2(d.second, 0, 1, d.first))
        {
            result += d.first;
        }
    }
    cout << "Part 2: " << result << endl;
}

// Version 2


bool check_target1_v2(const std::vector<int>& operands, int index, ll target)
{
    if(index < 0) return target == 0 || target == 1;

    if(target < 0) return false;

    if(target % operands[index] == 0)
    {
        if(check_target1_v2(operands, index-1, target/operands[index]))
            return true;
        // Here too there can be a subtle Bug
        // if you return check_target1_v2(i-1, target/operands[i])
        // it fails to account for check_target1_v2(i-1, target-operands[i])
        // example: 103304: 269 4 96 8
    }

    return check_target1_v2(operands, index-1, target-operands[index]);
}

void part1_v2(const std::vector<std::pair<ll, std::vector<int>>>& data)
{
    ll result = 0;
    for(const auto& d: data)
    {
        if(check_target1_v2(d.second, d.second.size() - 1, d.first))
        {
            result += d.first;
        }
    }
    cout << "Part 1 (Version 2): " << result << endl;
}

bool check_int_contains(ll target, ll subint, ll& new_target)
{
    int lt, ls;
    while(subint > 0)
    {
       lt = target % 10;
       target /= 10;
       ls = subint % 10;
       subint /= 10;
       if(lt != ls)
       {
           return false;
       }
    }
    new_target = target;
    return true;
}

bool check_target2_v2(const std::vector<int>& operands, int index, ll target)
{
    if(index < 0) return target == 0 || target == 1;

    if(target < 0) return false;

    if(target % operands[index] == 0)
    {
        if(check_target2_v2(operands, index-1, target/operands[index]))
            return true;
    }
    
    ll new_target;
    if(check_int_contains(target, operands[index], new_target))
    {
        if(check_target2_v2(operands, index-1, new_target))
            return true;
    }

    return check_target2_v2(operands, index-1, target-operands[index]);
}

void part2_v2(const std::vector<std::pair<ll, std::vector<int>>>& data)
{
    ll result = 0;
    for(const auto& d: data)
    {
        if(check_target2_v2(d.second, d.second.size() - 1, d.first))
        {
            result += d.first;
        }
    }
    cout << "Part 2 (Version 2): " << result << endl;
}
