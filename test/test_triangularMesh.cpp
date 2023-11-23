//
// Created by sabri on 23/11/2023.
//
#include "TriangularMesh.h"
#include <string>
#include <iostream>

int main(){
    std::string fileName = "triangular_mesh_test.vtx";
    TriangularMesh<2> mesh (fileName);
    std::cout << mesh.toString() << std::endl;


    return 0;
}