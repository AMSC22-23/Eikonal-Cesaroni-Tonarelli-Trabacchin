//
// Created by tomma on 02/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "../src/SerialEikonalSolver.h"
#include "../src/ParallelEikonalSolver.h"
#include <cmath>
#include <chrono>

int main(){
    const std::string fileName = "../test/input_meshes/triangulated/moai.vtk";
    TriangularMesh<3> mesh (fileName);

    std::vector<int> boundary;
    boundary.push_back(mesh.getNearestVertex(std::array<double,3>({0,0,0})));

    typename Eikonal::Eikonal_traits<3, 1>::MMatrix M;
    M << 1, 0, 0,
         0, 3, 0,
         0, 0, 7;

    SerialEikonalSolver<3,3> serial_solver(mesh, boundary, M);
    ParallelEikonalSolver<3,3> solver(mesh, boundary, M, 4);

    std::cout << "started serial\n";
    auto start1 = std::chrono::high_resolution_clock::now();
    serial_solver.solve();
    auto stop1 = std::chrono::high_resolution_clock::now();
    std::cout << "finished serial\n";

    std::cout << "started parallel\n";
    auto start2 = std::chrono::high_resolution_clock::now();
    solver.solve();
    auto stop2 = std::chrono::high_resolution_clock::now();
    std::cout << "finished parallel\n";

    std::cout << "Execution time serial = " <<
              std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << std::endl;

    std::cout << "Execution time parallel = " <<
              std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;

    solver.getSolutionsVTK("output-moai");

    return 0;
}
