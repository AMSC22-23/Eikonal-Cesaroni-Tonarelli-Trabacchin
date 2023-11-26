//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "TriangularEikonalSolver.h"

int main(){
    std::string fileName = "../test/test_mesh.vtk";
    TriangularMesh<2> mesh (fileName);
    std::vector<int> boundary;
    boundary.push_back(0);
    TriangularEikonalSolver<2> solver(mesh, boundary);
    solver.solve();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << "(" << mesh.getCoordinates(i)[0] << ", " << mesh.getCoordinates(i)[1] << ")"  << " " << solver.getSolutions()[i] << std::endl;\
    }
    //std::cout << mesh.toString_alt() << std::endl;



    return 0;
}