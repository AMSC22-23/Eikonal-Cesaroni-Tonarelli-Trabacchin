//
// Created by tomma on 02/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "EikonalSolver.h"
#include "ParallelEikonalSolver.h"
#include <cmath>
#include <chrono>

int main66(){
    const std::string fileName = "../test/triangulated_mesh_test.vtk";
    TriangularMesh<3> mesh (fileName);
    //std::cout<<mesh.toString() << std::endl;

    std::vector<int> boundary;
    boundary.push_back(0);
    EikonalSolver<3,3> serial_solver(mesh, boundary);
    ParallelEikonalSolver<3,3> solver(mesh, boundary,4);
    auto start1 = std::chrono::high_resolution_clock::now();
    serial_solver.solve();
    auto stop1 = std::chrono::high_resolution_clock::now();
    auto start2 = std::chrono::high_resolution_clock::now();
    solver.solve1();
    auto stop2 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << i << " " <<"(" << mesh.getCoordinates(i)[0] << ", " << mesh.getCoordinates(i)[1] <<", " << mesh.getCoordinates(i)[2]
                  << ")"  << " " << solver.getSolutions()[i] << " " << serial_solver.getSolutions()[i] << std::endl;
    }

    std::cout << "Execution time serial = " <<
              std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << std::endl;

    std::cout << "Execution time parallel = " <<
              std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;


    return 0;
}
