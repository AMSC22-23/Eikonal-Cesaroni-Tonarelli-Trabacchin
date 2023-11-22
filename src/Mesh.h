//
// Created by tomma on 22/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
template <int D, int N>
class Mesh {
    Mesh(const std::string& mesh_file_path) {
        std::ifstream mesh_file (mesh_file_path);
        if(mesh_file.is_open()) {
            int cont = 0;
            std::string buffer;
            double value;
            //first four lines are ignored, maybe as todo a sanity check
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            mesh_file >> buffer; // keyword POINTS ignored
            mesh_file >> vertices_number;
            geo.resize(vertices_number * D);
            for(int i = 0; i < vertices_number; i++) {
                for(int j = 0; j < D; j++) {
                    mesh_file >> value;
                    geo[cont + j] = value;
                }
                cont += 3;
            }
            ngh.resize(vertices_number, std::vector<int>());
            int elements_number;
            mesh_file >> buffer; //ignoring CELLS keyword
            mesh_file >> elements_number;
            mesh_file >> buffer; //ignoring next int, is irrelevant
            for(int i = 0; i < elements_number; i++) {
                mesh_file >> buffer;
                std::array<int, N> temp;
                for(int j = 0; j < N; j++) {
                    mesh_file >> temp[j];
                }
                for(int j = 0; j < N; j++) {
                    for(int q = 0; q < N; q++) {
                        if(j != q) {
                            ngh[temp[j]].push_back(temp[q]);
                        }
                    }
                }
            }
            //the rest of the file can be ignored
            mesh_file.close();
        }
    }

    int getVerticesNumber() const{
        return vertices_number;
    }

    std::vector<double>& getGeo() {
        return geo;
    }

    std::vector<std::vector<int>> getNeighbours() {
        return ngh;
    }

private:
    int vertices_number;
    std::vector<double> geo;
    std::vector<std::vector<int>> ngh; //todo it is possible to have a vector of double, at the price of an additional vector with indices for each vertex
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
