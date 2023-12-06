//
// Created by sabri on 01/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TetrahedricalMesh.h"
#include <cmath>
#include "../src/Eikonalsolver.h"
#include <ctime>
#include <chrono>

int main8(){
    std::string fileName = "../test/output-mesh-cube-40.vtk";
    TetrahedricalMesh<3> mesh (fileName);


    std::vector<int> boundary;
    boundary.push_back(0);
    EikonalSolver<3,4> solver (mesh, boundary);

    time_t start = time(0);
    auto start1 = std::chrono::high_resolution_clock::now();
    solver.solve();
    time_t stop = time(0);
    auto stop1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << i << " " <<"(" << mesh.getCoordinates(i)[0] << ", "
        << mesh.getCoordinates(i)[1] << ")"  << " " << solver.getSolutions()[i] << " "
        << sqrt(mesh.getCoordinates(i)[0]*mesh.getCoordinates(i)[0]
        + mesh.getCoordinates(i)[1]*mesh.getCoordinates(i)[1] +
        mesh.getCoordinates(i)[2] * mesh.getCoordinates(i)[2]  )   << std::endl;
    }
    std::cout << "Execution time = " << stop-start <<" or high resolution " <<
        std::chrono::duration_cast<std::chrono::microseconds>(stop1 - start1).count() << std::endl;

    return 0;
}