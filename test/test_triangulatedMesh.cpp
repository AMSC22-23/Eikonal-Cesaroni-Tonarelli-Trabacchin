//
// Created by tomma on 02/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "EikonalSolver.h"
#include <cmath>

int main6(){
    const std::string fileName = "../test/triangulated_mesh_test.vtk";
    TriangularMesh<3> mesh (fileName);
    //std::cout<<mesh.toString() << std::endl;

    std::vector<int> boundary;
    boundary.push_back(0);
    EikonalSolver<3,3> solver(mesh, boundary);
    solver.solve();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << i << " " <<"(" << mesh.getCoordinates(i)[0] << ", " << mesh.getCoordinates(i)[1] <<", " << mesh.getCoordinates(i)[2]
                  << ")"  << " " << solver.getSolutions()[i] << " " << sqrt(mesh.getCoordinates(i)[0] * mesh.getCoordinates(i)[0]
                                                                            + mesh.getCoordinates(i)[1]*mesh.getCoordinates(i)[1] + mesh.getCoordinates(i)[2]*mesh.getCoordinates(i)[2]) << std::endl;
    }




    return 0;
}
