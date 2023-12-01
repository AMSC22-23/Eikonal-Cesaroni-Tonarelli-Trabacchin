//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "TriangularEikonalSolver.h"
#include <cmath>

int main1(){
    const std::string fileName = "../test/triangular_mesh_test.vtk";
    TriangularMesh<2> mesh (fileName);
    std::cout<<mesh.toString() << std::endl;

    std::vector<int> boundary;
    boundary.push_back(0);
    TriangularEikonalSolver<2> solver(mesh, boundary);
    solver.solve();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << i << " " <<"(" << mesh.getCoordinates(i)[0] << ", " << mesh.getCoordinates(i)[1]
        << ")"  << " " << solver.getSolutions()[i] << " " << sqrt(mesh.getCoordinates(i)[0] * mesh.getCoordinates(i)[0]
        + mesh.getCoordinates(i)[1]*mesh.getCoordinates(i)[1])   << std::endl;\
    }




    return 0;
}