# cppml: library for ml in c++

## Prerequisites
* A compiler for c++ 17 or higher (MSVC definietly works)
* CMake 3.8+
  
## Build instructions
First do:
```bash
git clone https://github.com/mzelawski/cppml.git
```
In your project file. Then you have to add this line inside the CMakeLists.txt file:
```cmake
target_link_libraries(your_project PRIVATE cppml)
```
