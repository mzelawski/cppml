set(SOURCES
    cppml.cpp
    Vector.cpp
    Matrix.cpp
    math_utils.cpp
    LinearRegression.cpp
    Random.cpp
    KMeans.cpp
    RidgeRegression.cpp
    KDTree.cpp
    BallTree.cpp
    KNN.cpp
)

add_library(cppml STATIC ${SOURCES})

target_include_directories(cppml PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)