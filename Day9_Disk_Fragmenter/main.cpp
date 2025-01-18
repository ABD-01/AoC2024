#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>

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

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& );

std::ostream& operator<<(std::ostream& os, const std::vector<char>& v);

void part1(std::vector<int>& dm);
void part2(std::vector<int>& dm);


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

    std::string line;
    file >> line;
    std::vector<int> disk_map(line.size());

    int i = 0;
    for (char c : line) {
        disk_map[i++] = c - '0';
    }
        
    //cout << disk_map <<endl;

    part1(disk_map);
    //part2(disk_map);

    return 0;
}

void part1(std::vector<int>& dm)
{
    Timer t;
    std::stringstream ss;

    long long unsigned int result = 0;
    int i = 0,j,id_first = 0, id_last;
    j = (dm.size() % 2) ? dm.size() - 1 : dm.size() - 2;
    id_last = j / 2;

    for(int pos = 0; pos < dm.size(); ++pos)
    {
        //cout << dm << endl;
        //cout << "pos = " << pos << " j = " << j <<  " id_last = " << id_last << endl;
        //cout << ss.str() <<endl;
        if( pos % 2 == 0) // even position i.e it is file block
        {
            for(int k = 0; k < dm[pos]; ++k)
            {
                ss << id_first;
                result += (id_first*i);
                ++i;
            }
            ++id_first;
        }
        else // odd position i.e. empty block
        {
            for(int k = 0; k < dm[pos]; ++k)
            {
                while(dm[j] == 0 && j > pos)
                {
                    j-=2;
                    --id_last;
                }
                if(j<pos) break;

                --dm[j];
                ss << id_last;
                result += (id_last*i);
                ++i;
            }
        }
        if(j<pos) break;
    }
    //cout << ss.str() << endl;
    cout << "Part 1: " << result << endl;
}

#if 1
void part2(std::vector<int>& dm)
{
    Timer t;
    std::stringstream ss;

    std::vector<int> udm;
    long long unsigned int result = 0;
    int i = 0,j,id_first = 0, id_last;
    j = (dm.size() % 2) ? dm.size() - 1 : dm.size() - 2;
    id_last = j / 2;
    
    std::vector<char> memory;

    int first_ptr = i, second_ptr = j;
    int id1 = id_first, id2 = id_last;

    for(int pos = 0; pos < dm.size(); ++pos)
    {
        cout << dm << endl;
        cout << "pos = " << pos << " j = " << j <<  " id_last = " << id_last << endl;
        //cout << ss.str() <<endl;
        cout <<memory<<endl;
        if( pos % 2 == 0) // even position i.e it is file block
        {
            for(int k = 0; k < dm[pos]; ++k)
            {
                ss << id_first;
                udm.push_back(id_first);
                result += (id_first*i);
                ++i;
                memory.push_back(id_first + '0');
            }
            ++id_first;
            continue;
        }
        id1 = id_first;
        // odd position i.e. empty block
        for(first_ptr = pos; first_ptr<dm.size(); ++first_ptr)
        {
            cout << "first_ptr = " << first_ptr << " second_ptr = " << second_ptr <<  " id2 = " << id2 << endl;
            //cout << ss.str() <<endl;
            cout <<memory<<endl;

            if(first_ptr%2==0) 
            {
                for(int k = 0; k < dm[first_ptr]; ++k)
                {
                    ss << id1;
                    memory.push_back(id1+'0');
                }
                if(dm[first_ptr] == 0)
                    memory.push_back('.');
                ++id1;
                continue;
            }
            while(dm[first_ptr] >= dm[second_ptr] && second_ptr > first_ptr) // if empty space has capacity for files
            {
                dm[first_ptr] -= dm[second_ptr]; // empty spaces consumed

                for(auto _=0;_<dm[second_ptr];_++) {ss << id2; memory.push_back(id2+'0');}

                dm[second_ptr] = 0; // all files moved from this block
                second_ptr -= 2;
                --id2;
            }

            for(int k = 0; k < dm[first_ptr]; ++k)
            {
                ss << ".";
                memory.push_back('.');
            }

            if(dm[first_ptr] == 0 || second_ptr < first_ptr)
            {
                memory.push_back('.');
            }
        }
        break;
        j -= 2; // move left to see if any more file can fit in the block

        if(j<pos) break;
    }
    //cout << ss.str() << endl;
    cout <<memory<<endl;

    cout << udm << endl;
    result = 0;
    for(int l =0;l<udm.size();++l)
    {
        result = result + (l*udm[l]);
    }
    
    cout << "Part 2: " << result << endl;
}
#endif

#if 0
void part2(std::vector<int>& dm)
{
    Timer t;
    std::stringstream ss;

    std::vector<int> udm;
    long long unsigned int result = 0;
    int i = 0,j,id_first = 0, id_last;
    j = (dm.size() % 2) ? dm.size() - 1 : dm.size() - 2;
    id_last = j / 2;

    for(int pos = 0; pos < dm.size(); ++pos)
    {
        cout << dm << endl;
        cout << "pos = " << pos << " j = " << j <<  " id_last = " << id_last << endl;
        cout << ss.str() <<endl;
        if( pos % 2 == 0) // even position i.e it is file block
        {
            for(int k = 0; k < dm[pos]; ++k)
            {
                ss << id_first;
                udm.push_back(id_first);
                result += (id_first*i);
                ++i;
            }
            ++id_first;
        }
        else // odd position i.e. empty block
        {
            while(dm[pos] >= dm[j] && j > pos) // while empty space has capacity for files
            {
                dm[pos] -= dm[j]; // empty spaces consumed
                                  //
                for(auto _=0;_<dm[j];_++) ss << id_last;

                dm[j] = 0; // all files moved from this block
                j -= 2; // move left to see if any more file can fit in the block
                --id_last;
            }

            for(int k = 0; k < dm[pos]; ++k)
            {
                ss << ".";
            }
        }
        if(j<pos) break;
    }
    cout << ss.str() << endl;
    cout << udm << endl;
    result = 0;
    for(int l =0;l<udm.size();++l)
    {
        result = result + (l*udm[l]);
    }
    
    cout << "Part 2: " << result << endl;
}
#endif

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << "[";
    for (auto i = v.begin(); i != v.end(); ++i) {
        if (i != v.begin()) os << ", ";
        os << *i;
    }
    os << "]";
    return os;
}
std::ostream& operator<<(std::ostream& os, const std::vector<char>& v)
{
    for (auto i = v.begin(); i != v.end(); ++i) {
        os << *i;
    }
    return os;
}

