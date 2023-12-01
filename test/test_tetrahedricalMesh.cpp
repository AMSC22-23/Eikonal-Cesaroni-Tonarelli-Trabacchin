//
// Created by sabri on 01/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TetrahedricalMesh.h"
#include <cmath>
#include "../src/TetrahedricalEikonalsolver.h"

int main(){
    std::string fileName = "../test/output-mesh-cube-40.vtk";
    //TriangularMesh<2> mesh (fileName,1);
    TetrahedricalMesh<3> mesh (fileName);
    //std::cout<<mesh.toString() << std::endl;
    std::cout<<mesh.toString() << std::endl;
    std::vector<int> boundary;
    boundary.push_back(0);
    TetrahedricalEikonalSolver<3> solver (mesh, boundary);
    solver.solve();



    return 0;
}