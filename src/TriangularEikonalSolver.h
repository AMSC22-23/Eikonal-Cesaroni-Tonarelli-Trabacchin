//
// Created by tomma on 24/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H

#define eikonal_tol 1e-6

#include "TriangularMesh.h"
#include "CircularList.h"
#include <float.h>
#include "Phi.hpp"
#include "../localProblem/include/solveEikonalLocalProblem.hpp"

template<int D>
class TriangularEikonalSolver {

public:
    TriangularEikonalSolver(TriangularMesh<D>& mesh, std::vector<int>& boundary_vertices) :
            mesh(mesh), boundary_vertices(boundary_vertices) {
        solutions_out.resize(mesh.getNumberVertices(), 10);
        solutions_in.resize(mesh.getNumberVertices(), 10);
        for(auto bv : boundary_vertices){
            solutions_out[bv] = 0;
            solutions_in[bv] = 0;
        }
    };

    void solve(){
        for(auto bv : boundary_vertices){
            for(auto neighbor : mesh.getNeighbors(bv)){
                active_list.add(neighbor);
                //std::cout << "added init points: " << neighbor << std::endl;
            }
        }

        while(!active_list.isEmpty()){
            int v = active_list.getNext();
            double old_solution = solutions_in[v];
            double new_solution = update(v);
            solutions_in[v] = new_solution;
            if(std::abs(old_solution - new_solution) < eikonal_tol) {
                std::vector<int> v_neighbours = mesh.getNeighbors(v);
                for(auto b : v_neighbours) {
                    if(!active_list.isPresent(b)) {
                        //std::cout << "not present: " << b << std::endl;
                        double old_solution_b = solutions_in[b];
                        double new_solution_b = update(b);
                        if(old_solution_b > new_solution_b) {
                            solutions_in[b] = new_solution_b;
                            active_list.add(b);
                            //std::cout << "add" << std::endl;
                            //std::cout << "added: " << b << std::endl;
                        }
                    }
                    else {
                        //std::cout << "present: " << b << std::endl;
                    }
                }
                active_list.remove();
                //std::cout << "remove" << std::endl;
                //std::cout << "removed " << v <<std::endl;
            }


        }
    }

    std::vector<double>& getSolutions(){
        return solutions_in;
    }

private:
    TriangularMesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    CircularList active_list;
    std::vector<double> solutions_in;
    std::vector<double> solutions_out;
    double velocity = 1;

    double update(int vertex) {
        std::vector<int> triangles = mesh.getTriangles(vertex);
        std::vector<double> solutions;
        solutions.resize(triangles.size() / 2, DBL_MAX);
        for(int i = 0; i < triangles.size(); i += 2){
            std::array<std::array<double, D>, 3> coordinates;
            std::array<double, 2> solutions_base;

            coordinates[0] = mesh.getCoordinates(triangles[i]);
            coordinates[1] = mesh.getCoordinates(triangles[i+1]);
            coordinates[2] = mesh.getCoordinates(vertex);

            solutions_base[0] = solutions_in[triangles[i]];
            solutions_base[1] = solutions_in[triangles[i+1]];

            solutions.push_back(solveLocalProblem(coordinates, solutions_base));
        }

        double min = *std::min_element(solutions.begin(), solutions.end());
        //solutions_out[vertex] = min;
        //solutions_in[vertex] = min;
        return min;
    }

    double solveLocalProblem(std::array<std::array<double, D>, 3> coordinates, std::array<double, 2> solutions_base){
        constexpr std::size_t PHDIM=2;

        using Point = Eikonal::Eikonal_traits<PHDIM>::Point;
        using VectorExt = Eikonal::Eikonal_traits<PHDIM>::VectorExt;
        Point p1, p2, p3;
        for(int i = 0; i < D; i++){
            p1(i) = coordinates[0][i];
            p2(i) = coordinates[1][i];
            p3(i) = coordinates[2][i];
        }

        Eikonal::Eikonal_traits<PHDIM>::MMatrix M;
        M << velocity, 0.0,
             0.0, velocity;
        VectorExt values;
        values << solutions_base[0], solutions_base[1];          //values of u at the base
        Eikonal::SimplexData<PHDIM> simplex{{p1, p2, p3}, M};

        Eikonal::solveEikonalLocalProblem<PHDIM> localSolver{simplex,values};

        auto sol = localSolver();

        assert(sol.status == 0);
        return sol.value;
    }

};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TRIANGULAREIKONALSOLVER_H
