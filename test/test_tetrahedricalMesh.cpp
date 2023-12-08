//
// Created by sabri on 01/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TetrahedricalMesh.h"
#include <cmath>
#include "../src/Eikonalsolver.h"
#include "../src/ParallelEikonalSolver.h"
#include "../src/TestParallelEikonalSolver.h"
#include <ctime>
#include <chrono>

int main(){
    std::string fileName = "../test/output-mesh-cube-40.vtk";
    TetrahedricalMesh<3> mesh (fileName);

    std::vector<int> boundary;
    boundary.push_back(0);

    EikonalSolver<3,4> serial_solver(mesh, boundary);
    TestParallelEikonalSolver<3,4> solver(mesh, boundary, 32);
    std::cout << "started serial" << std::endl;
    auto start1 = std::chrono::high_resolution_clock::now();
    serial_solver.solve();
    auto stop1 = std::chrono::high_resolution_clock::now();

    std::cout << "finished serial" << std::endl;
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

    std::cout << "speed up = " << (double)std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() / (double)std::chrono::duration_cast<std::chrono::microseconds>(stop2 - start2).count() << std::endl;



    return 0;
}