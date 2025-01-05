#include <iostream>
#include <sstream>
#include <fstream> // contains ifstream and ofstream
#include <vector>
#include <algorithm> // for sort
//#include <ranges> // for ranges, use -std=c++20 during compilation

void getVectors(std::vector<int>& vec1, std::vector<int>& vec2, std::istream& in);

template<typename T>
void insertionSort(T&);


int main(int argc, char *argv[])
{

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file>" << std::endl;
        return 1;
    }

    std::ifstream file;
    file.open(argv[1]);
    
    std::vector<int> vec1, vec2;
    getVectors(vec1, vec2, file);

    if (vec1.size() != vec2.size())
    {
        std::cerr << "Error: Vector size not equal." << std::endl;
        return 1;
    }

    //insertionSort<std::vector<int>>(vec1);
    //insertionSort<std::vector<int>>(vec2);
    
    std::sort(vec1.begin(), vec1.end());
    std::sort(vec2.begin(), vec2.end());
    //std::ranges::sort(vec2);

    int result = 0;
    for (std::size_t i = 0; i < vec1.size(); i++)
        result += abs(vec1[i] - vec2[i]);

    std::cout << result << std::endl;

    result = 0;
    for (std::size_t i = 0; i < vec1.size(); i++)
    {
        /**
        int count = 0;
        for (std::size_t j = 0; j < vec1.size(); j++)
            if (vec1[i] == vec2[j])
                count++;
        */
        int count = std::count(vec2.begin(), vec2.end(), vec1[i]);
        //int count = std::ranges::count(vec2, vec1[i]);
        result += (count * vec1[i]);
    }

    std::cout << result << std::endl;

    file.close();
}

void getVectors__unused(std::vector<int>& vec1, std::vector<int>& vec2, std::istream& in)
{
    if (!in) {
        std::cerr << "Error: Invalid input stream." << std::endl;
        return;
    }
    int n,m;
    while (in >> n >> m) {
        vec1.push_back(n);
        vec2.push_back(m);
    }
}

void getVectors(std::vector<int>& vec1, std::vector<int>& vec2, std::istream& in)
{
    if (!in) {
        std::cerr << "Error: Invalid input stream." << std::endl;
        return;
    }
    int n,m;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        iss >> n >> m;
        vec1.push_back(n);
        vec2.push_back(m);
    }
}

template<typename T>
void insertionSort(T& vec)
{
    typename T::iterator first = vec.begin();
    typename T::iterator last = vec.end();

    for(auto current = first + 1; current != last; current++)
    {
       auto key = *current;
       //auto position = current - 1;
       auto position = current;
       for (; position != first && *(position-1) > key; position--)
       {
            //*(position+1) = *position;
            *(position) = *(position-1);
       }
       //*(position+1) = key;
       *(position) = key;
    }
    return;
}
