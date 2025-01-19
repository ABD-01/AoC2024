#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>
#include <Python.h>

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

void part1(std::vector<int> dm);
void part2(std::vector<int> dm);


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
        
    part1(disk_map);
    part2(disk_map);

    return 0;
}

void part1(std::vector<int> dm)
{
    Timer t;
    std::stringstream ss;

    long long unsigned int result = 0;
    int i = 0,j,id_first = 0, id_last;
    j = (dm.size() % 2) ? dm.size() - 1 : dm.size() - 2;
    id_last = j / 2;
    // An individual file/empty space is different from file/empty block. Files in same block have same id. 
    // i is the position of individual file/empty space
    // j is the position of file block starting from the end (decrements by 2 to keep pointing to file block)
    // id_first and id_last is the ids of files blocks. 

    for(int pos = 0; pos < dm.size(); ++pos)
    {
        // cout << ss.str() <<endl;
        if( pos % 2 == 0) // even position i.e it is file block
        {
            for(int k = 0; k < dm[pos]; ++k)  // for files in block at pos
            {
                ss << id_first;
                result += (id_first*i);
                ++i;
            }
            ++id_first;
        }
        else // odd position i.e. empty block
        {
            for(int k = 0; k < dm[pos]; ++k) // for empty spaces in block at pos
            {
                while(dm[j] == 0 && j > pos) //  shift j left until 
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
    // cout << ss.str() << endl;
    cout << "Part 1: " << result << endl;
}

void part2(std::vector<int> dm)
{
    Py_Initialize();
    PyRun_SimpleString("import sys; sys.path.append('.')");
    PyObject* pName = PyUnicode_FromString("day9"); // Python script name
    PyObject* pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != nullptr) {
        // Get the Python function
        PyObject* pFunc = PyObject_GetAttrString(pModule, "part2");
        if (PyCallable_Check(pFunc)) {
            // Convert C++ vector to Python list
            PyObject* pList = PyList_New(dm.size());
            for (size_t i = 0; i < dm.size(); ++i)
                PyList_SetItem(pList, i, PyLong_FromLong(dm[i])); // Note: PyList_SetItem steals the reference
            // Call the Python function
            PyObject* pResult = PyObject_CallFunctionObjArgs(pFunc, pList, nullptr);
            Py_DECREF(pList);
            Py_DECREF(pFunc);
        } else {
            PyErr_Print();
        }
        Py_DECREF(pModule);
    } else {
        PyErr_Print();
    }

    Py_Finalize();    
}

#if 0
void part2(std::vector<int> dm)
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
            cout << ss.str() <<endl;

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
    cout << ss.str() << endl;

    result = 0;
    for(int l =0;l<udm.size();++l)
    {
        result = result + (l*udm[l]);
    }
    
    cout << "Part 2: " << result << endl;
}
#endif

#if 0
void part2(std::vector<int> dm)
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

    result = 0;
    for(int l =0;l<udm.size();++l)
    {
        result = result + (l*udm[l]);
    }
    
    cout << "Part 2: " << result << endl;
}
#endif

std::ostream& operator<<(std::ostream& os, const std::vector<char>& v)
{
    for (auto i = v.begin(); i != v.end(); ++i) {
        os << *i;
    }
    return os;
}

