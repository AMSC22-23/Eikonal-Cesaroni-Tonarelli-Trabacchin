//
// Created by tomma on 04/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
#define eikonal_tol 1e-8
#include "Mesh.h"
#include "DoubleCircularList.h"
#include <float.h>
#include "../localProblem_alt2/include/Phi.hpp"
#include "../localProblem_alt2/include/solveEikonalLocalProblem.hpp"
template<int D, int N>
class ParallelEikonalSolver{
public:
    ParallelEikonalSolver(Mesh<D>& mesh, std::vector<int>& boundary_vertices,int threads_number) :
    mesh(mesh), boundary_vertices(boundary_vertices), threads_number(threads_number){
        solutions.resize(mesh.getNumberVertices()*2, 1000);
        for(auto bv : boundary_vertices){
            solutions[bv] = 0;
            solutions[bv+mesh.getNumberVertices()]=0;
        }
        p_out = 1;
    }

    void solve(){
        for(auto bv : boundary_vertices){
            active_list.push_back(bv);
        }


    }
private:
    Mesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    std::vector<int> active_list;
    std::vector<double> solutions;
    double velocity = 1;
    int threads_number;
    int p_out;

    double get_solution(int vertex, const std::map<int, double>& vertex_to_solution){
        auto it = vertex_to_solution.find(vertex);
        if(it != vertex_to_solution.end()){
            return it->second;
        } else {
            return solutions[(p_out==1 ? 0 : 1)*solutions.size()/2+vertex];
        }

    }

    void solve_step(int thread_id){
        std::map<int,double> vertex_to_solution;
        DoubleCircularList local_list;
        for(int i=thread_id; i<active_list.size(); i+=threads_number){
            local_list.add(active_list[i]);
        }
        while(!local_list.isEmpty()){
            Node* node = local_list.getNext();
            int v = node -> data;
            double old_solution = get_solution(v, vertex_to_solution);
            double new_solution = update(v, vertex_to_solution);
            solutions[p_out*solutions.size()/2+v] = new_solution;
            //write in map
            vertex_to_solution.insert(std::pair<int, double> (v, new_solution));

            if(std::abs(old_solution - new_solution) < eikonal_tol) {
                std::vector<int> v_neighbours = mesh.getNeighbors(v);
                for (auto b: v_neighbours) {
                    if (!local_list.isPresent(b)) {
                        double old_solution_b = get_solution(v, vertex_to_solution);
                        double new_solution_b = update(b, vertex_to_solution);
                        if (old_solution_b > new_solution_b) {
                            solutions[p_out*solutions.size()/2+b] = new_solution_b;
                            // write in map
                            vertex_to_solution.insert(std::pair<int, double> (b, new_solution_b));
                            local_list.add(b);
                        }
                    }
                }
                local_list.remove(node);
            }

        }


    }

    double solveLocalProblem(std::array<std::array<double, D>, N> coordinates, std::array<double, N - 1> solutions_base){

        using VectorExt = typename Eikonal::Eikonal_traits<D, N - 2>::VectorExt;

        typename Eikonal::Eikonal_traits<D,N - 2>::MMatrix M;
        for(int i = 0; i < D; i++) {
            for(int j = 0; j < D; j++) {
                if(i!=j) {
                    M.coeffRef(i,j) = 0;
                } else {
                    M.coeffRef(i,j) = velocity;
                }
            }
        }
        VectorExt values;
        for(int i = 0; i < N - 1; i++) {
            values[i] = solutions_base[i];
        }
        Eikonal::SimplexData<D, N> simplex{coordinates, M};

        Eikonal::solveEikonalLocalProblem<N, D> localSolver{simplex,values};

        auto sol = localSolver();

        assert(sol.status == 0);
        return sol.value;
    }


    double update(int vertex, const std::map<int, double> vertex_to_solution) {
        std::vector<int> triangles = mesh.getShapes(vertex);
        std::vector<double> solutions;
        int number_of_vertices = mesh.getVerticesPerShape();
        solutions.resize(triangles.size() / D, DBL_MAX);
        for(int i = 0; i < triangles.size(); i += number_of_vertices - 1){
            std::array<std::array<double, D>, N> coordinates;
            std::array<double, N - 1> solutions_base;
            for(int j = 0; j < number_of_vertices - 1; j++) {
                coordinates[j] = mesh.getCoordinates(triangles[i + j]);
            }
            coordinates[number_of_vertices - 1] = mesh.getCoordinates(vertex);

            for(int j = 0; j < number_of_vertices - 1; j++) {
                solutions_base[j] = get_solution(triangles[i + j], vertex_to_solution);
            }
            solutions.push_back(solveLocalProblem(coordinates, solutions_base));
        }

        double min = *std::min_element(solutions.begin(), solutions.end());
        return min;
    }
};

#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_PARALLELEIKONALSOLVER_H
