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

template<int D>
class TriangularMesh {
public:
    static constexpr int vertices_per_triangle = 3;

    TriangularMesh(const std::string& mesh_file_path){
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
            std::vector<int> mapping_vector = removeDuplicatedVertices(1e-8);
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
                for(int j=0; j<3; j++){
                    for(int k=0; k<3; k++){
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
                            cont += 2;
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
           for(int i=index; i< (cont < ngh.size()-1 ? ngh[cont+1] : shapes.size()); i+=2){
               res+=std::to_string(shapes[i]) + " " + std::to_string (shapes[i+1]) + ", ";
               index = i+2;
           }
           cont++;
           if(cont==ngh.size()){
               break;
           }
           res += "\n";
       }
       return res;
    }

    std::string toString_alt(){
        std::string res = "";
        int odd = 0;
        for(int i = 0 ; i < ngh.size(); i++){
            res += "vertex " + std::to_string(i) + ": ";
            odd = 0;
            for(int j = ngh[i]; j < ( i != ngh.size() - 1 ? ngh[i+1] : shapes.size()); j++){
                if(!odd){
                    res += std::to_string(shapes[j]) + " ";
                } else {
                    res += std::to_string(shapes[j]) + ",  ";
                }
                odd = (odd + 1) % 2;
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
        for(int i = vertex; i < vertex + D; i++){
            coord[i-vertex] = geo[i];
        }
        return coord;
    }


private:
    std::vector<double> geo;
    std::vector<int> shapes;
    std::vector<int> ngh;

    std::vector<int> removeDuplicatedVertices(double tol) {
        std::vector<double> reduced_geo;
        std::vector<int> mapping_vector;
        mapping_vector.resize(geo.size() / D);
        int cont = 0;
        for(int i = 0; i < geo.size(); i += D) {
            bool found;
            int idx;
            std::tie(found, idx) = search_for_equivalent_vertices(reduced_geo, i/D, tol);
            if(found) {
                mapping_vector[i/D] = idx;
            } else {
                mapping_vector[i/D] = cont;
                cont++;
                for(int j = 0; j < D; j++) {
                    reduced_geo.push_back(geo[i+j]);
                }
            }
        }
        geo = reduced_geo;
        return mapping_vector;
    }

    std::tuple<bool, int> search_for_equivalent_vertices(const std::vector<double>& reduced_geo, int index, double tol) {
        std::array<double, D> node_to_search;
        for(int i = 0; i < D; i++) {
            node_to_search[i] = geo[D*index + i];
        }
        for(int i = 0; i < reduced_geo.size(); i+=D) {
            std::array<double, D> v1;
            for(int j = 0; j < D; j++) {
                v1[j] = reduced_geo[i+j];
            }
            if(getDist(v1, node_to_search) < tol) {
                return std::make_tuple(true, i/D);
            }

        }
        return std::make_tuple(false, 0);
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
