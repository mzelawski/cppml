# cppml: library for ml in c++

## Prerequisites
* A compiler for c++ 17 or higher (MSVC definietly works)
* CMake 3.8+
  
## Build instructions
First do
```bash
git clone https://github.com/mzelawski/cppml.git
```
in your project directory. Then you have to add this line inside the global CMakeLists.txt file:
```cmake
add_subdirectory(cppml)
```
After that you also have to add this line to your source CMakeLists.txt file:
```cmake
target_link_libraries(your_project PRIVATE cppml)
```
