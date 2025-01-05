#include <iostream>
#include <iterator>
#include <fstream>
#include <cctype>
#include <vector>
#include <sstream>
#include <regex>
#include <string>


using std::cout;
using std::cerr;
using std::endl;

void part1(const std::string& filename);
void part2(const std::string& filename);

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file>" << endl;
        return 1;
    }
    part1(argv[1]);
    part2(argv[1]);
    return 0;
}

void part1(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    std::istreambuf_iterator<char> it(file), end;
    auto first = it;
    auto second = it;
    
    std::string mul_substr("mul(");

    std::vector<std::pair<int, int>> mulVec;
    unsigned int result(0);

    for(first = it; it != end; ++it)
    {
        if (
            (char)*first == 'm' && 
            (char)*++first == 'u' &&
            (char)*++first == 'l' &&
            (char)*++first == '('
        )
        {
            bool secondNum = false;
            unsigned int num1(0), num2(0); 

            for(second=++first; second != end; ++second)    
            {
                if(std::isdigit(*second))
                {
                    if(secondNum)
                        num2 = num2*10 + (*second - '0');
                    else
                        num1 = num1*10 + (*second - '0');
                }
                else if ((char)*second == ',')
                    secondNum=true;
                else if ((char)*second == ')' && secondNum)
                {
                    mulVec.emplace_back(num1, num2);
                    result += (num1*num2);
                    break;
                }
                else
                    break;
            }
        }

    }

    //for(auto i: mulVec) cout << i.first << "," << i.second << endl; cout << endl;

    cout << "Part 1: " << result << endl;
}


void part2(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }
    std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    std::regex regex(R"((mul\(\d{1,3},\d{1,3}\))|(do\(\))|(don't\(\)))");
    
    std::sregex_iterator start{ std::sregex_iterator(contents.begin(), contents.end(), regex) };
    std::sregex_iterator end{ std::sregex_iterator() };

    unsigned int result(0);

    int mulEnabled(1);
    for (auto i = start; i != end; ++i)
    {
        //std::smatch match = *i;
        std::string match = (*i).str();

        if(match.compare("don't()") == 0)
            mulEnabled = 0;

        else if (match.compare("do()") == 0) 
            mulEnabled = 1;

        else
        {
            int num1(0), num2(0);
            auto j = 4;
            while(match[j] != ',')
                num1 = num1*10 + (match[j++] - '0'); 
            ++j;
            while(match[j] != ')')
                num2 = num2*10 + (match[j++] - '0');

            result += (mulEnabled * num1 * num2); 
        }
    }

    cout << "Part 2: " << result << endl;
}
