# Advent of Code 2024 - C++ Solutions

## 🚀 Overview  

This repository contains my solutions for Advent of Code 2024, implemented in C++. As someone with experience in C and Python, I am using AoC as a hands-on way to improve my C++ skills while solving fun and challenging problems.

📖 **Blogs:** 
- [Days 1 to 8](https://abd-01.github.io/posts/2025-01-04-Advent-of-Code-2024-Day-1-to-8/)  


## 🔧 Setup & Compilation  

### Input Files  

Before running the solutions, you need to download the input files for each day. There are two ways to organize them:  

#### 1. Per-Day Directory Structure  

Place each input file in its corresponding day's folder as shown below. The CMake script will automatically copy them to the build/ directory when you build the project.

```
|-- Day01_Historian_Hysteria
|   `-- input.txt
|-- Day02_Red-Nosed_Reports
|   `-- input.txt
|-- Day03_Mull_It_Over
|   `-- input.txt
|   .
|   .
|   .
|-- DayN_XXX_XXX
|   `-- input.txt
```

A warning will be shown if now corresponding input file is found.

#### 2. Single `build/` Directory  
Alternatively, you can place all input files directly inside the `build/` directory with the naming format:  

```
build/input1.txt
build/input2.txt
build/input3.txt
.
.
.
build/inputN.txt
```

### Build and Run Instructions  

#### 1. Clone the Repository  

```bash
git clone https://github.com/ABD-01/AoC2024.git
cd AoC2024
```

#### 2. Create a `build/` Directory  

```bash
mkdir build
cd build
```

#### 3. Configure the Project with CMake  

```bash
cmake ..
```

#### 4. Build the Executables  

```bash
make
```

#### 5. Run the Solutions  

- **Run all solutions**  

  ```bash
  make run
  ```

- **Run a specific day's solution** (e.g., Day 2)  

  ```bash
  make run2
  ```

Each day's executable will automatically read the input file (input{day}.txt) from the build/ directory, where CMake has placed it during the build process.

## 📌 Contributions  
If you'd like to contribute, feel free to suggest improvements, alternative solutions or criticism via issues or pull requests.

## 📜 License  

This project is open-source and available under the [MIT License](./LICENSE).
