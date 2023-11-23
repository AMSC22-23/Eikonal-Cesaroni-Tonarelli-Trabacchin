cmake_minimum_required(VERSION 3.20)
project(Eikonal_Cesaroni_Tonarelli_Trabacchin)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)
include_directories(src)
include_directories("/u/sw/toolchains/gcc-glibc/11.2.0/pkgs/eigen/3.3.9/include/eigen3")
include_directories("localProblem/include")
add_compile_options(-I${mkEigenInc})

add_executable(Eikonal_Cesaroni_Tonarelli_Trabacchin
        localProblem/DescentDirectionFactory.cpp
        localProblem/DescentDirections.cpp
        localProblem/LineSearchSolver.cpp
        localProblem/main_eikonal.cpp
        localProblem/solveEikonalLocalProblem.cpp
        src/vertex.h
        src/Lattice2DSolver.h src/Lattice2DMesh.h src/LatticeMesh.h src/utils/FillingLooper.h src/utils/CreatingLooper.h src/CircularList.h test/test_circularList_alt.cpp src/Mesh.h localProblem/include/DescentDirectionBase.hpp src/main.cpp)
