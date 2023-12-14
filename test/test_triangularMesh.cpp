//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "SerialEikonalSolver.h"
#include <cmath>

int main35(){
    const std::string fileName = "../test/input_meshes/triangular/square.vtk";
    TriangularMesh<2> mesh (fileName);


    std::cout<< mesh.toString() << std::endl;

    std::vector<int> boundary;
    boundary.push_back(mesh.getNearestVertex(std::array<double, 2>({0.5,0.5})));

    typename Eikonal::Eikonal_traits<2 ,1>::MMatrix M;
    M << 1, 0.5,
            0.5, 2;

    SerialEikonalSolver<2,3> solver(mesh, boundary, M);
    solver.solve();

    solver.getSolutionsVTK("output-triangular");

    return 0;
}