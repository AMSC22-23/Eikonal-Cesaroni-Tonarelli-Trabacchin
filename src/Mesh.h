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
#include <set>
#include <algorithm>
#include <numeric>

template <int D>
class Mesh {

public:

    std::string toString() {
        int cont = 0;
        int index = 0;
        std::string res = "";
        while(true) {
            res+= "vertex " + std::to_string(cont) + ": " ;
            for(int i=index; i< (cont < ngh.size()-1 ? ngh[cont+1] : shapes.size()); i+= vertices_per_shape - 1){
                res+=std::to_string(shapes[i]) + " " + std::to_string (shapes[i+1]) + ", ";
                index = i + vertices_per_shape - 1;
            }
            cont++;
            if(cont==ngh.size()){
                break;
            }
            res += "\n";
        }
        return res;
    }

    int getNumberVertices() const{
        return geo.size() / D;
    }

    std::vector<int> getNeighbors(int vertex){
        std::set<int> neighbors;
        for(int i = ngh[vertex]; i < (vertex != ngh.size() -1 ? ngh[vertex + 1] : shapes.size()); i++){
            neighbors.insert(shapes[i]);
        }
        std::vector<int> res(neighbors.begin(), neighbors.end());
        return res;
    }

    std::vector<int> getShapes(int vertex){
        std::vector<int> shapes_v;
        for(int i = ngh[vertex]; i < (vertex != ngh.size() -1 ? ngh[vertex + 1] : shapes.size()); i++){
            shapes_v.push_back(shapes[i]);
        }
        return shapes_v;
    }

    std::array<double, D> getCoordinates(int vertex){
        std::array<double,D> coord;
        for(int i = D * vertex; i < D * vertex + D; i++){
            coord[i - D * vertex] = geo[i];
        }
        return coord;
    }


protected:
    std::vector<int> removeDuplicateVertices(double tol){
        std::vector<int> pos;
        pos.resize(geo.size()/D);
        std::iota(pos.begin(), pos.end(),0);
        std::sort(pos.begin(), pos.end(), [&](std::size_t i, std::size_t j) {assert(i < pos.size() && j < pos.size()); return verticesCompare(i,j)==1; });

        int current_index = 0;
        int prec;
        std::vector<int> same;
        std::vector<int> mapping_vector;
        std::vector<double> reduced_geo;
        reduced_geo.resize(0);
        mapping_vector.resize(geo.size()/D);

        while(current_index < pos.size()){
            prec=current_index;
            current_index++;
            same.push_back(pos[prec]);
            while(true){
                if( current_index < pos.size() && verticesCompare(pos[prec], pos[current_index]) == 0){
                    same.push_back(pos[current_index]);
                    current_index++;
                } else{
                    for(int j : same){
                        mapping_vector[j]=(int)reduced_geo.size()/D;
                    }
                    for(int i=0; i<D; i++){
                        reduced_geo.push_back(geo[same[0]*D+i]);
                    }
                    break;
                }
            }
            same.clear();
        }
        geo = reduced_geo;
        return mapping_vector;
    }

    int verticesCompare(int i, int j){
        for(int k = 0; k < D; k++){
            if(geo[D * i + k] < geo[D * j + k]){
                return 1;
            } else if (geo[D * i + k] > geo[D * j + k]){
                return -1;
            }
        }
        return 0;
    }

    std::vector<std::set<int>> init_mesh(const std::string& mesh_file_path, int vertices_per_shape_) {
        vertices_per_shape = vertices_per_shape_;
        std::ifstream mesh_file (mesh_file_path);
        std::vector<std::set<int>> sets;
        if(mesh_file.is_open()) {
            std::string buffer;
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            std::getline(mesh_file, buffer);
            mesh_file>>buffer;
            int vertices_number;
            mesh_file>>vertices_number;
            mesh_file>>buffer;
            geo.resize(vertices_number*D);
            for(int i=0; i<vertices_number*D; i++){
                mesh_file>>geo[i];
            }
            std::vector<int> mapping_vector = removeDuplicateVertices(1e-8);


            vertices_number = geo.size()/D;
            mesh_file>>buffer;
            int triangle_number;
            mesh_file>>triangle_number;
            mesh_file>>buffer;
            sets.resize(vertices_number);
            ngh.resize(vertices_number);
            for(int i=0; i<triangle_number; i++){
                mesh_file>>buffer;
                std::vector<int> tmp;
                tmp.resize(vertices_per_shape);
                for(int j=0; j<vertices_per_shape; j++){
                    mesh_file>>tmp[j];
                    tmp[j] = mapping_vector[tmp[j]];
                }
                for(int j=0; j < vertices_per_shape; j++){
                    for(int k=0; k < vertices_per_shape; k++){
                        if(j!=k){
                            sets[tmp[j]].insert(tmp[k]);
                        }
                    }
                }

            }
            mesh_file.close();
        } else {
            std::cout << "Couldn't open mesh file." << std::endl;
        }
        return sets;
    }

    std::vector<double> geo;
    std::vector<int> shapes;
    std::vector<int> ngh;
    int vertices_per_shape = 0;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_MESH_H
