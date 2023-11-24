//
// Created by sabri on 23/11/2023.
//
#include <string>
#include <iostream>
#include "../src/TriangularMesh.h"
int main(){
    std::string fileName = "../test/triangular_mesh_test.vtx";
    TriangularMesh<2> mesh (fileName);
    std::cout << mesh.toString_alt() << std::endl;



    return 0;
}