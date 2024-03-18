cmake_minimum_required(VERSION 3.20)
project(Eikonal_Cesaroni_Tonarelli_Trabacchin)

set(CMAKE_CXX_STANDARD 20)
include_directories(src)
include_directories(test) #to remove
include_directories("/apps/USE/easybuild/release/2023.1/software/Eigen/3.4.0-GCCcore-12.3.0/include/Eigen")
#include_directories(${EIGEN})
include_directories("localProblem/include")
#add_compile_options(-I${mkEigenInc})


add_executable(tetrahedrical
        test/test_tetrahedricalMesh.cpp
        )

add_executable(triangular
        test/test_triangularMesh.cpp
)

add_executable(triangulated
        test/test_triangulatedMesh.cpp
)

add_executable(test
test/test_local_solver.cpp)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
