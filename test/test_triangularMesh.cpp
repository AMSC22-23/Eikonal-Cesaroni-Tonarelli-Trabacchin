//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
#include "TriangularEikonalSolver.h"
#include <cmath>

int main(){
    std::string fileName = "../test/triangular_mesh_test.vtx";
    //TriangularMesh<2> mesh (fileName,1);
    TriangularMesh<2> mesh1 (fileName,2);
    //std::cout<<mesh.toString() << std::endl;
    std::cout<<mesh1.toString() << std::endl;
    std::vector<int> boundary;
    boundary.push_back(0);
    TriangularEikonalSolver<2> solver(mesh1, boundary);
//    //std::cout << "geo" << std::endl;
//    //for(int i = 0; i < mesh.geo.size(); i+=2) {
//        //std::cout << mesh.geo[i] << " " << mesh.geo[i+1] << std::endl;
//    //}
    solver.solve();
    for(int i = 0; i < solver.getSolutions().size();i++) {
        std::cout << i << " " <<"(" << mesh1.getCoordinates(i)[0] << ", " << mesh1.getCoordinates(i)[1] << ")"  << " " << solver.getSolutions()[i] << " " << sqrt(mesh1.getCoordinates(i)[0]*mesh1.getCoordinates(i)[0] + mesh1.getCoordinates(i)[1]*mesh1.getCoordinates(i)[1])   << std::endl;\
    }




    return 0;
}