#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <sstream>

#define DEBUG_PRINT 0

#if DEBUG_PRINT
#define DEBUG(x) std::cout << x
#else
#define DEBUG(x)
#endif

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

void part1(std::vector<int> dm);
void part2(std::vector<int> );


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
        DEBUG(ss.str() << endl);
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
    DEBUG(ss.str() << endl);
    cout << "Part 1: " << result << endl;
}


struct Block {
    int block_size;
    int file_id;

    Block(int x, int y = -1) : block_size(x), file_id(y) {}

    void print() const {
        for (int i = 0; i < block_size; i++) {
            if (file_id == -1)
                std::cout << ".";
            else
                std::cout << file_id;
        }
    }   
};

void printBlocks(const std::vector<Block>& blocks) {
#if DEBUG_PRINT
    for (const auto& b : blocks) {
        b.print();
    }
    std::cout << std::endl;
#endif
}

std::ostream& operator<<(std::ostream& os, const Block& b) {
    os << "Block(" << b.block_size << ", " << b.file_id << ")";
    return os;
}

std::vector<Block> moveBlocks(std::vector<Block>& dm, int j) {
    std::vector<Block> tmp;
    if (dm[j].file_id == -1)
        return dm;  // Selected block is empty, return as is

    for (size_t i = 0; i < dm.size(); i++) {
        if (j < i) {
            tmp.insert(tmp.end(), dm.begin() + i, dm.end());
            break;
        }

        if (dm[i].file_id != -1) {
            tmp.push_back(dm[i]);
            continue;
        }

        if (dm[i].block_size < dm[j].block_size) {
            tmp.push_back(dm[i]);
            continue;
        }

        int diff = dm[i].block_size - dm[j].block_size;
        if (diff > 0) {
            tmp.emplace_back(dm[j].block_size, dm[j].file_id);
            tmp.emplace_back(diff, -1);
            dm[j].file_id = -1;
        } else {
            tmp.emplace_back(dm[j].block_size, dm[j].file_id);
            dm[j].file_id = -1;
        }

        tmp.insert(tmp.end(), dm.begin() + i + 1, dm.end());
        break;
    }

    return tmp;
}


void part2(std::vector<int> data) {
    Timer t;
    std::vector<Block> dm;
    
    for (size_t i = 0; i < data.size(); i++) {
        if (i % 2 == 0)
            dm.emplace_back(data[i], i / 2);
        else
            dm.emplace_back(data[i], -1);
    }

    printBlocks(dm);

    for (int j = dm.size() - 1; j >= 0; j--) {
        dm = moveBlocks(dm, j);
        printBlocks(dm);
    }

    long long unsigned int result = 0;
    int pos = 0;
    
    for (const auto& b : dm) {
        for (int i = 0; i < b.block_size; i++) {
            if (b.file_id != -1)
                //result += (b.file_id * pos);
                result += (static_cast<unsigned long long>(b.file_id) * pos);
            pos++;
        }
    }

    std::cout << "Part 2: " << result << std::endl;
}


#if 0
#include <Python.h>

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
#endif

