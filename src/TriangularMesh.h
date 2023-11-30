//
// Created by sabri on 23/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include <iostream>
#include <set>
#include <math.h>
#include <algorithm>
#include <tuple>
#include <cassert>
#include <numeric>

template<int D>
class TriangularMesh {
public:
    static constexpr int vertices_per_triangle = 3;

    TriangularMesh(const std::string& mesh_file_path, int n){
        std::ifstream mesh_file (mesh_file_path);
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
            std::vector<int> mapping_vector;
            if(n==1){
                mapping_vector = removeDuplicatedVertices(1e-8);
            } else {
                mapping_vector = removeDuplicateVertices_efficient(1e-8);
            }
            /**
             * debug
             */
             std::cout << "mapping: " << std::endl;
             for(int i = 0 ; i < mapping_vector.size(); i++) {
                 std::cout << mapping_vector[i] << " ";
             }
             std::cout << std::endl;
            vertices_number = geo.size()/D;
            mesh_file>>buffer;
            int triangle_number;
            mesh_file>>triangle_number;
            mesh_file>>buffer;
            std::vector<std::set<int>> sets;
            sets.resize(vertices_number);
            ngh.resize(vertices_number);
            for(int i=0; i<triangle_number; i++){
                mesh_file>>buffer;
                std::array<int,vertices_per_triangle> tmp;
                for(int j=0; j<vertices_per_triangle; j++){
                    mesh_file>>tmp[j];
                    tmp[j] = mapping_vector[tmp[j]];
                }
                for(int j=0; j < vertices_per_triangle; j++){
                    for(int k=0; k < vertices_per_triangle; k++){
                        if(j!=k){
                            sets[tmp[j]].insert(tmp[k]);
                        }
                    }
                }

            }
            int cont = 0;
            for(int i=0; i<vertices_number; i++) {
                ngh[i] = cont;
                for(const auto& x: sets[i]){
                    std::vector<int> tmp (std::min(sets[i].size(), sets[x].size() ), 0);
                    std::vector<int>::iterator end;
                    end = std::set_intersection(sets[i].begin(), sets[i].end(), sets[x].begin(), sets[x].end(), tmp.begin());
                    std::vector<int>::iterator it;

                    for(it=tmp.begin(); it!=end; it++){
                        if(*it > x){
                            shapes.push_back(x);
                            shapes.push_back(*it);
                            cont += vertices_per_triangle - 1;
                        }
                    }
                }
            }
            mesh_file.close();
        } else {
            std::cout << "opppppsssss" << std::endl;
        }
    }

    std::string toString() {
        int cont = 0;
        int index = 0;
        std::string res = "";
       while(true) {
           res+= "vertex " + std::to_string(cont) + ": " ;
           for(int i=index; i< (cont < ngh.size()-1 ? ngh[cont+1] : shapes.size()); i+= vertices_per_triangle - 1){
               res+=std::to_string(shapes[i]) + " " + std::to_string (shapes[i+1]) + ", ";
               index = i + vertices_per_triangle - 1;
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

    std::vector<int> getTriangles(int vertex){
        std::vector<int> triangles;
        for(int i = ngh[vertex]; i < (vertex != ngh.size() -1 ? ngh[vertex + 1] : shapes.size()); i++){
            triangles.push_back(shapes[i]);
        }
        return triangles;
    }

    std::array<double, D> getCoordinates(int vertex){
        std::array<double,D> coord;
        for(int i = D*vertex; i < D*vertex + D; i++){
            coord[i-D*vertex] = geo[i];
        }
        return coord;
    }


public:
    std::vector<double> geo;
    std::vector<int> shapes;
    std::vector<int> ngh;

    std::vector<int> removeDuplicatedVertices(double tol) {
        for(int i = 0; i < geo.size()/D; i++) {
            //std::cout << i << " " << geo[D*i] << " " << geo[D*i+1] << std::endl;
        }
        std::vector<double> reduced_geo;
        std::vector<int> mapping_vector;
        mapping_vector.resize(geo.size() / D);
        int cont = 0;
        for(int i = 0; i < mapping_vector.size(); i++) {
            auto [found, idx] = search_for_equivalent_vertices(reduced_geo, i, tol);
            if(found) {
                //std::cout << "found duplicated vertex: " << i << std::endl;
                mapping_vector[i] = idx;
            } else {
                mapping_vector[i] = cont;
                cont++;
                for(int j = 0; j < D; j++) {
                    reduced_geo.push_back(geo[D*i+j]);
                }
            }
        }
        geo = reduced_geo;
        return mapping_vector;
    }

    std::vector<int> removeDuplicateVertices_efficient(double tol){
        std::vector<int> pos;
        pos.resize(geo.size()/D);
        std::iota(pos.begin(), pos.end(),0);
        std::sort(pos.begin(), pos.end(), [&](std::size_t i, std::size_t j) { return verticesCompare(i,j)!=-1; });

        //debug
        std::cout << "sorted pos = ";
        for(int i = 0; i < pos.size(); i++){
            std::cout << pos[i] << " ";
        }
        std::cout << std::endl;

        int current_index = 1;
        int prec = 0;
        std::vector<int> same;
        std::vector<int> mapping_vector;
        std::vector<double> reduced_geo;
        reduced_geo.resize(0);
        mapping_vector.resize(geo.size()/D);
        while(true){
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
            if(current_index >= pos.size()){
                break;
            }
            same.clear();
            prec=current_index;
            current_index++;
        }
        geo = reduced_geo;
        return mapping_vector;
    }

    int verticesCompare(int i, int j){
        for(int k=0; k<D; k++){
            if(geo[D*i+k] < geo[D*j+k]){
                return 1;
            } else if (geo[D*i+k] > geo[D*j+k]){
                return -1;
            }
        }
        return 0;
    }

    std::tuple<bool, int> search_for_equivalent_vertices(const std::vector<double>& reduced_geo, int index, double tol) {
        std::array<double, D> node_to_search = getCoordinates(index);
        for(int i = 0; i < reduced_geo.size()/D; i++) {
            std::array<double, D> v1;
            for(int j = 0; j < D; j++) {
                v1[j] = reduced_geo[D*i+j];
            }
            if(getDist(v1, node_to_search) < tol) {
                return {true, i};
            }
        }
        return {false, 0};
    }

    double getDist(std::array<double, D> v1,std::array<double, D> v2) {
        double norm = 0;
        for(int i = 0; i < D; i++) {
            norm += (v1[i] - v2[i])*(v1[i] - v2[i]);
        }
        return sqrt(norm);
    }
};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGLEMESH_H
