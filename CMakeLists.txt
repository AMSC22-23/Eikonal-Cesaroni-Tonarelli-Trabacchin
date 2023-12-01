cmake_minimum_required(VERSION 3.20)
project(Eikonal_Cesaroni_Tonarelli_Trabacchin)

set(CMAKE_CXX_STANDARD 20)
include_directories(src)
include_directories("/u/sw/toolchains/gcc-glibc/11.2.0/pkgs/eigen/3.3.9/include/eigen3")
include_directories("localProblem/include")
add_compile_options(-I${mkEigenInc})
add_compile_definitions(DIMENSION=3)



add_executable(Eikonal_Cesaroni_Tonarelli_Trabacchin
        test/test_triangularMesh.cpp localProblem/solveEikonalLocalProblem.cpp localProblem/LineSearchSolver.cpp localProblem/DescentDirections.cpp
        localProblem/DescentDirectionFactory.cpp test/test_tetrahedricalMesh.cpp
        src/TetrahedricalMesh.h)
