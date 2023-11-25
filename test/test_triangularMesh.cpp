//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "TriangularEikonalSolver.h"

int main(){
    std::string fileName = "../test/triangular_mesh_test.vtx";
    TriangularMesh<2> mesh (fileName);
    std::vector<int> boundary;
    boundary.push_back(0);
    TriangularEikonalSolver<2> solver(mesh, boundary);
    solver.solve();
    std::cout << mesh.toString_alt() << std::endl;



    return 0;
}