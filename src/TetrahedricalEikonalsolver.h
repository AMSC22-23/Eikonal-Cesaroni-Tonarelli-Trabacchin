//
// Created by sabri on 01/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TETRAHEDRICALEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TETRAHEDRICALEIKONALSOLVER_H
#define eikonal_tol 1e-10
#include "TetrahedricalMesh.h"
#include "DoubleCircularList.h"
#define DIMENSION 3
#include <float.h>
#include "Phi.hpp"
#include "../localProblem/include/solveEikonalLocalProblem.hpp"

template<int D>
class TetrahedricalEikonalSolver {

public:
    TetrahedricalEikonalSolver(TetrahedricalMesh<D>& mesh, std::vector<int>& boundary_vertices) :
            mesh(mesh), boundary_vertices(boundary_vertices) {
        solutions_out.resize(mesh.getNumberVertices(), 1000);
        solutions_in.resize(mesh.getNumberVertices(), 1000);
        for(auto bv : boundary_vertices){
            solutions_out[bv] = 0;
            solutions_in[bv] = 0;
        }
    };

    void solve(){
        int count = 0;
        for(auto bv : boundary_vertices){
            for(auto neighbor : mesh.getNeighbors(bv)){
                active_list.add(neighbor);
            }
        }
        while(!active_list.isEmpty()){
            Node* node = active_list.getNext();
            int v = node -> data;
            double old_solution = solutions_in[v];
            double new_solution = update(v);
            solutions_in[v] = new_solution;

            if(std::abs(old_solution - new_solution) < eikonal_tol) {
                std::vector<int> v_neighbours = mesh.getNeighbors(v);
                for (auto b: v_neighbours) {
                    if (!active_list.isPresent(b)) {
                        double old_solution_b = solutions_in[b];
                        double new_solution_b = update(b);
                        if (old_solution_b > new_solution_b) {
                            solutions_in[b] = new_solution_b;
                            active_list.add(b);

                        }
                    }
                }
                active_list.remove(node);
            }
        }
    }

    std::vector<double>& getSolutions(){
        return solutions_in;
    }

private:
    TetrahedricalMesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    DoubleCircularList active_list;
    std::vector<double> solutions_in;
    std::vector<double> solutions_out;
    double velocity = 1;

    double update(int vertex) {
        std::vector<int> triangles = mesh.getShapes(vertex);
        std::vector<double> solutions;
        solutions.resize(triangles.size() / 3, DBL_MAX);
        for(int i = 0; i < triangles.size(); i += 3){
            std::array<std::array<double, D>, 4> coordinates;
            std::array<double, 3> solutions_base;

            coordinates[0] = mesh.getCoordinates(triangles[i]);
            coordinates[1] = mesh.getCoordinates(triangles[i+1]);
            coordinates[2] = mesh.getCoordinates(triangles[i+2]);
            coordinates[3] = mesh.getCoordinates(vertex);


            solutions_base[0] = solutions_in[triangles[i]];
            solutions_base[1] = solutions_in[triangles[i+1]];
            solutions_base[2] = solutions_in[triangles[i+2]];

            solutions.push_back(solveLocalProblem(coordinates, solutions_base));
        }

        double min = *std::min_element(solutions.begin(), solutions.end());
        //solutions_out[vertex] = min;
        //solutions_in[vertex] = min;
        return min;
    }

    double solveLocalProblem(std::array<std::array<double, D>, 4> coordinates, std::array<double, 3> solutions_base){
        constexpr std::size_t PHDIM=3;

        using Point = Eikonal::Eikonal_traits<PHDIM>::Point;
        using VectorExt = Eikonal::Eikonal_traits<PHDIM>::VectorExt;
        Point p1, p2, p3, p4;
        for(int i = 0; i < D; i++){
            p1(i) = coordinates[0][i];
            p2(i) = coordinates[1][i];
            p3(i) = coordinates[2][i];
            p4(i) = coordinates[3][i];
        }

        Eikonal::Eikonal_traits<PHDIM>::MMatrix M;
        M << velocity, 0.0, 0.0,
                0.0, velocity,0.0,
                0.0, 0.0, velocity;
        VectorExt values;
        values << solutions_base[0], solutions_base[1], solutions_base[2];          //values of u at the base
        Eikonal::SimplexData<PHDIM> simplex{{p1, p2, p3, p4}, M};

        Eikonal::solveEikonalLocalProblem<PHDIM> localSolver{simplex,values};

        auto sol = localSolver();

        assert(sol.status == 0);
        return sol.value;
    }

};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TETRAHEDRICALEIKONALSOLVER_H
