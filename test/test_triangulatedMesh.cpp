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

int main45(){
    const std::string fileName = "../test/meshes/test_mesh.vtk";
    TriangularMesh<2> mesh (fileName);
    //std::cout<<mesh.toString() << std::endl;

    std::vector<int> boundary;
    boundary.push_back(0);

    typename Eikonal::Eikonal_traits<2, 1>::MMatrix M;
    M << 1, 0, 0,
            0, 1, 0,
            0, 0, 1;

    SerialEikonalSolver<2,3> serial_solver(mesh, boundary, M);
    ParallelEikonalSolver<2,3> solver(mesh, boundary, M, 4);

    auto start1 = std::chrono::high_resolution_clock::now();
    serial_solver.solve();
    auto stop1 = std::chrono::high_resolution_clock::now();

    auto start2 = std::chrono::high_resolution_clock::now();
    solver.solve();
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
