//
// Created by sabri on 01/12/2023.
//

#include <string>
#include <iostream>
#include "../src/TetrahedricalMesh.h"
#include "TriangularEikonalSolver.h"
#include <cmath>

int main(){
    std::string fileName = "../test/triangular_mesh_test.vtx";
    //TriangularMesh<2> mesh (fileName,1);
    TetrahedricalMesh<3> mesh (fileName);
    //std::cout<<mesh.toString() << std::endl;
    std::cout<<mesh.toString() << std::endl;
    std::vector<int> boundary;
    boundary.push_back(0);
    // TriangularEikonalSolver<2> solver(mesh1, boundary);
//    //std::cout << "geo" << std::endl;
//    //for(int i = 0; i < mesh.geo.size(); i+=2) {
//        //std::cout << mesh.geo[i] << " " << mesh.geo[i+1] << std::endl;
//    //}
//    solver.solve();
//    for(int i = 0; i < solver.getSolutions().size();i++) {
//        std::cout << i << " " <<"(" << mesh1.getCoordinates(i)[0] << ", " << mesh1.getCoordinates(i)[1] << ")"  << " " << solver.getSolutions()[i] << " " << sqrt(mesh1.getCoordinates(i)[0]*mesh1.getCoordinates(i)[0] + mesh1.getCoordinates(i)[1]*mesh1.getCoordinates(i)[1])   << std::endl;\
//    }




    return 0;
}