//
// Created by Melanie Tonarelli on 11/12/23.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H

#include "Mesh.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <climits>
#include "../localProblem_alt2/include/Eikonal_traits.hpp"

template<int D, int N>
class EikonalSolver {
public:

    explicit EikonalSolver(Mesh<D>& mesh) : mesh(mesh) {};

    virtual void solve() = 0;

    virtual std::vector<double> getSolutions() = 0;

    void getSolutionsVTK(const std::string& output_file_name){
        std::ofstream output_file(output_file_name);

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

        output_file.close();
    }

protected:
    Mesh<D>& mesh;
    std::vector<double> solutions;

    int reorder_solutions(std::array<std::array<double, D>, N>& coordinates, std::array<double, N - 1>& sol) {
        double tol = 1e-30;
        double mult = 1e-9;

        for(int i = 0; i < N - 1; i++) {
            for(int j = i + 1; j < N - 1; j++) {
                if(std::abs(sol[i] - sol[j]) < tol) {
                    sol[i] *= (1-mult) * sol[i];
                }
            }
        }

        auto min_sol_value = std::numeric_limits<double>::max();
        int min_sol_index = 0;
        for(int i = 0; i < N - 1; i++) {
            if(sol[i] < min_sol_value) {
                min_sol_value = sol[i];
                min_sol_index = i;
            }
        }
        std::swap(sol[min_sol_index], sol[N-2]);
        std::swap(coordinates[min_sol_index], coordinates[N-2]);


        return min_sol_index;
    }




};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_EIKONALSOLVER_H
