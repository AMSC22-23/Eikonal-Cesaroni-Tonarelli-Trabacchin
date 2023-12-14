//
// Created by sabri on 01/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TetrahedricalMesh.h"
#include <cmath>
#include "../src/SerialEikonalSolver.h"
#include "../src/ParallelEikonalSolver.h"
#include <ctime>
#include <chrono>

int main(){
    std::string fileName = "../test/input_meshes/cube-5.vtk";
    TetrahedricalMesh<3> mesh (fileName);

    std::vector<int> boundary;
    boundary.push_back(0);

    typename Eikonal::Eikonal_traits<3 ,2>::MMatrix M;
    M << 1, 0, 0,
        0, 1, 0,
        0, 0, 1;

    SerialEikonalSolver<3,4> serial_solver(mesh, boundary, M);
    ParallelEikonalSolver<3,4> solver(mesh, boundary, M, 4);
    std::cout << "started serial" << std::endl;
    auto start1 = std::chrono::high_resolution_clock::now();
    serial_solver.solve();
    auto stop1 = std::chrono::high_resolution_clock::now();
    std::cout << "finished serial" << std::endl;

    auto start2 = std::chrono::high_resolution_clock::now();
    solver.solve();
    auto stop2 = std::chrono::high_resolution_clock::now();

    std::cout << "Execution time serial = " <<
              std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << std::endl;

    std::cout << "Execution time parallel = " <<
              std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;

    std::cout << "speed up = " << (double)std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() / (double)std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;

    solver.getSolutionsVTK("output2");

    return 0;
}