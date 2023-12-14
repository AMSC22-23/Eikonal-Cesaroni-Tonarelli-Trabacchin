//
// Created by Melanie Tonarelli on 11/12/23.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H

#include "Mesh.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../localProblem_alt2/include/Eikonal_traits.hpp"

template<int D, int N>
class EikonalSolver {
public:

    explicit EikonalSolver(Mesh<D>& mesh) : mesh(mesh) {};

    virtual void solve() = 0;

    virtual std::vector<double> getSolutions() = 0;

    void getSolutionsVTK(){
        const std::string output_file_name = "../test/output_meshes/output.vtk";
        getSolutionsVTK(output_file_name);
    }

    void getSolutionsVTK(const std::string& output_file_name){
        std::string fileName = "../test/output_meshes/" + output_file_name + ".vtk";
        std::ofstream output_file(fileName);

        std::string input = mesh.getFilenameInputMesh();
        std::ifstream input_file(input);

        std::string line;
        if (input_file && output_file) {
            while (std::getline(input_file, line)) {
                output_file << line << "\n";
            }
        }
        else {
            printf("Cannot read File");
        }
        input_file.close();

        output_file << "POINT_DATA " << mesh.getOriginalNumberOfVertices() << std::endl;
        output_file << "SCALARS solution double 1" << std::endl;
        output_file << "LOOKUP_TABLE default" << std::endl;
        for(int i = 0; i < mesh.getOriginalNumberOfVertices(); i++){
            double solution = solutions[mesh.getMapVertex(i)];
            output_file << solution << " ";
        }
        output_file << std::endl;
        output_file.flush();
        std::cout << "Output vtk file located in folder test/output_meshes\n";

        output_file.close();
    }

protected:
    Mesh<D>& mesh;
    std::vector<double> solutions;

};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
