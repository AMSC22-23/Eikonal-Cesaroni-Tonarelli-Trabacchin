//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "SerialEikonalSolver.h"
#include <cmath>

int main2(){
    const std::string fileName = "../test/meshes/triangular_mesh_test.vtk";
    TriangularMesh<2> mesh (fileName);
    //std::cout<<mesh.toString() << std::endl;

    std::vector<int> boundary;
    boundary.push_back(0);

    typename Eikonal::Eikonal_traits<2 ,1>::MMatrix M;
    M << 1, 0,
            0, 1;

    SerialEikonalSolver<2,3> solver(mesh, boundary, M);
    solver.solve();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << i << " " <<"(" << mesh.getCoordinates(i)[0] << ", " << mesh.getCoordinates(i)[1]
        << ")"  << " " << solver.getSolutions()[i] << " " << sqrt(mesh.getCoordinates(i)[0] * mesh.getCoordinates(i)[0]
        + mesh.getCoordinates(i)[1]*mesh.getCoordinates(i)[1])   << std::endl;\
    }




    return 0;
}