#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <chrono>

using std::cout;
using std::cerr;
using std::endl;

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

void get_rules_and_orders(
        std::vector<std::pair<int, int>>& rule,
        std::vector<std::vector<int>>& orders,
        const std::string& filename
    );

bool isOrderValid(const std::vector<int>& o, const std::vector<std::pair<int, int>>& rule);

std::vector<std::vector<int>> part1(std::vector<std::pair<int, int>>&, std::vector<std::vector<int>>&);
void part2(std::vector<std::pair<int, int>>&, std::vector<std::vector<int>>&);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }
    std::vector<std::pair<int, int>> rule;
    std::vector<std::vector<int>> orders;
    get_rules_and_orders(rule, orders, argv[1]);

    /** Printing
    for(auto& i:rule)
        cout << i.first <<"," << i.second << endl;

    for(auto& i:orders)
    {
        for(auto& j:i)
            cout << j <<",";
        cout << endl;
    }
    */

    cout << "Part 1: ";
    std::vector<std::vector<int>> incorrect_orders =  part1(rule, orders);
    cout << "Part 2: ";
    part2(rule, incorrect_orders);
    return 0;
}

std::vector<std::vector<int>> part1(
        std::vector<std::pair<int, int>>& rule,
        std::vector<std::vector<int>>& orders
    )
{
    Timer t;
    std::vector<std::vector<int>> incorrect_orders;
    int result = 0;
    for(std::vector<int> o: orders)
    {
        if(isOrderValid(o, rule))
            result += o[o.size()/2];
        else
            incorrect_orders.push_back(std::move(o));
    }
    cout << result << endl;
    return incorrect_orders;
}

void part2(
        std::vector<std::pair<int, int>>& rule,
        std::vector<std::vector<int>>& incorrect_orders
    )
{
    Timer t;
    int result = 0;

    for(std::vector<int>& o: incorrect_orders)
    {
        while(true)
        {
            bool order_valid = true;

            for(auto r: rule)
            {
                auto i=0,j=0;
                for(; i < o.size(); ++i)
                {
                    if(o[i] == r.first) break;
                }
                for(; j < o.size(); ++j) 
                {
                    if(o[j] == r.second) break;
                }

                if(i==o.size() || j==o.size()) continue;
                if(j < i)
                {
                    std::swap(o[i], o[j]);
                    order_valid = false;
                    break;
                }
            }

            if (order_valid) break;
        }
    }
    auto temp =  part1(rule, incorrect_orders);
}

bool _isOrderValid(const std::vector<int>& o, const std::vector<std::pair<int, int>>& rule)
{
    for(auto i=0; i < o.size()-1; ++i) 
    {
        for(auto j=i+1; j < o.size(); ++j) 
        {
            for(auto r: rule)
            {
                if(o[i] == r.second && o[j] == r.first)
                {
                    // incorrect order
                    return false;
                }
            }
        }
    }
    return true;
}

bool isOrderValid(const std::vector<int>& o, const std::vector<std::pair<int, int>>& rule) {
    // Map elements in 'o' to their indices for fast lookup
    std::unordered_map<int, int> indexMap;
    for (int i = 0; i < o.size(); ++i) {
        indexMap[o[i]] = i;
    }

    // Check if any rule is violated
    for (const auto& r : rule) {
        if (indexMap.find(r.first) != indexMap.end() && indexMap.find(r.second) != indexMap.end()) {
            if (indexMap[r.first] > indexMap[r.second]) {
                // Order is invalid
                return false;
            }
        }
    }

    // Order is valid
    return true;
}

void get_rules_and_orders(
        std::vector<std::pair<int, int>>& rule,
        std::vector<std::vector<int>>& orders,
        const std::string& filename
    )
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        auto n = line.find("|");
        if (n == std::string::npos) break;
        
        rule.emplace_back(
            std::stoi(line.substr(0, n)),
            std::stoi(line.substr(n + 1))
        );
    }
    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::vector<int> pages;
        std::string page;
        while (std::getline(iss, page, ','))
        {
            pages.push_back(std::stoi(page));
        }
        orders.push_back(std::move(pages));
    }
}
