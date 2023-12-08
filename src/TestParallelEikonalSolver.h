//
// Created by tomma on 07/12/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TESTPARALLELEIKONALSOLVER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TESTPARALLELEIKONALSOLVER_H
//
// Created by tomma on 04/12/2023.
//

#define eikonal_tol_par 1e-4
#include "Mesh.h"
#include "DoubleCircularList.h"
#include <float.h>
#include "../localProblem_alt2/include/Phi.hpp"
#include "../localProblem_alt2/include/solveEikonalLocalProblem.hpp"
#include "omp.h"
#include <algorithm>

template<int D, int N>
class TestParallelEikonalSolver{
public:
    TestParallelEikonalSolver(Mesh<D>& mesh, std::vector<int>& boundary_vertices,int threads_number) :
            mesh(mesh), boundary_vertices(boundary_vertices), threads_number(threads_number){
        solutions.resize(mesh.getNumberVertices()*2, 1000);
        for(auto bv : boundary_vertices){
            solutions[bv] = 0;
            solutions[bv+mesh.getNumberVertices()]=0;
        }
        p_out = 0;
        p_in = 0;
    }

    void solve(){
        std::vector<int> present(solutions.size()/2);
        std::fill(present.begin(), present.end(), 0);
        int ACTIVE_LIST_LENGTH = solutions.size()/2;
        active_list.resize(ACTIVE_LIST_LENGTH);
        int activeListIndex = 0;
        for(auto bv : boundary_vertices){
            for(auto neighbor : mesh.getNeighbors(bv)){
                active_list[activeListIndex] = neighbor;
                present[neighbor] = 1;
                activeListIndex++;
            }
        }

        #pragma omp parallel default(none) shared(present, std::cout, active_list, solutions, p_out, p_in, activeListIndex, ACTIVE_LIST_LENGTH) num_threads(threads_number)
        {
            int address;
            std::map<int,double> vertex_to_solution;
            DoubleCircularList local_list;

            int thread_id = omp_get_thread_num();

            while(activeListIndex > 0) {
                for(int i=thread_id; i<activeListIndex; i+=threads_number){
                    local_list.add(active_list[i]);
                }
                #pragma omp barrier
                if(thread_id == threads_number - 1) {
                    activeListIndex = 0;
                    std::fill(present.begin() + thread_id*present.size()/threads_number, present.end() , 0);
                }
                else {
                    std::fill(present.begin() + thread_id*present.size()/threads_number, present.begin() + (thread_id + 1)*present.size()/threads_number, 0);
                }

                #pragma omp barrier
                vertex_to_solution.clear();
                while (!local_list.isEmpty()) {
                    Node *node = local_list.getNext();
                    int v = node->data;
                    double old_solution;
                    old_solution = getLocalSolution(v, vertex_to_solution);
                    double new_solution = update(v, vertex_to_solution);
                    address = p_out * solutions.size() / 2 + v;
                    //#pragma omp atomic write relaxed
                    //solutions[address] = new_solution;
                    writeLocalSolution(address, new_solution);
                    vertex_to_solution[v] = new_solution;
                    if (std::abs(old_solution - new_solution) < eikonal_tol_par) {
                        std::vector<int> v_neighbours = mesh.getNeighbors(v);
                        for (auto b: v_neighbours) {
                            int isPresent;
                            #pragma omp atomic capture relaxed
                            isPresent = present[b]++;
                            if (isPresent == 0) {
                                double old_solution_b;
                                old_solution_b = getLocalSolution(b, vertex_to_solution);
                                double new_solution_b = update(b, vertex_to_solution);
                                if (old_solution_b > new_solution_b) {
                                    address = p_out * solutions.size() / 2 + b;
                                    //#pragma omp atomic write relaxed
                                    //solutions[address] = new_solution_b;
                                    writeLocalSolution(address, new_solution_b);
                                    vertex_to_solution[b] = new_solution_b;
                                    int oldIndex;
                                    #pragma omp atomic capture relaxed
                                    oldIndex = activeListIndex++;
                                    active_list[oldIndex] = b;
                                }
                            }
                        }
                        local_list.remove(node);
                    }
                }
                #pragma omp barrier
            }
        }
    }

    void solve1(){
        std::vector<int> present(solutions.size()/2);
        std::fill(present.begin(), present.end(), 0);
        int ACTIVE_LIST_LENGTH = solutions.size()/2;
        active_list.resize(ACTIVE_LIST_LENGTH);
        int activeListIndex = 0;
        for(auto bv : boundary_vertices){
            for(auto neighbor : mesh.getNeighbors(bv)){
                active_list[activeListIndex] = neighbor;
                present[neighbor] = 1;
                activeListIndex++;
            }
        }

#pragma omp parallel default(none) shared(present, std::cout, active_list, solutions, p_out, p_in, activeListIndex, ACTIVE_LIST_LENGTH) num_threads(threads_number)
        {
            int address;
            std::map<int,double> vertex_to_solution;
            DoubleCircularList local_list;

            int thread_id = omp_get_thread_num();

            while(activeListIndex > 0) {
                for(int i=thread_id; i<activeListIndex; i+=threads_number){
                    local_list.add(active_list[i]);
                }
                #pragma omp barrier
                if(thread_id == threads_number - 1) {
                    activeListIndex = 0;
                    std::fill(present.begin() + thread_id*present.size()/threads_number, present.end() , 0);
                }
                else {
                    std::fill(present.begin() + thread_id*present.size()/threads_number, present.begin() + (thread_id + 1)*present.size()/threads_number, 0);
                }

                #pragma omp barrier
                vertex_to_solution.clear();
                while (!local_list.isEmpty()) {
                    Node *node = local_list.getNext();
                    int v = node->data;
                    double old_solution;
                    old_solution = getLocalSolution(v, vertex_to_solution);
                    double new_solution = update(v, vertex_to_solution);
                    address = p_out * solutions.size() / 2 + v;
                    //#pragma omp atomic write relaxed
                    writeLocalSolution(v, new_solution);
                    //solutions[address] = new_solution;
                    vertex_to_solution[v] = new_solution;
                    if (std::abs(old_solution - new_solution) < eikonal_tol_par) {
                        std::vector<int> v_neighbours = mesh.getNeighbors(v);
                        for (auto b: v_neighbours) {
                            int isPresent;
                            #pragma omp atomic capture relaxed
                            isPresent = present[b]++;
                            if (isPresent == 0) {
                                double old_solution_b;
                                old_solution_b = getLocalSolution(b, vertex_to_solution);
                                double new_solution_b = update(b, vertex_to_solution);
                                if (old_solution_b > new_solution_b) {
                                    address = p_out * solutions.size() / 2 + b;
                                    #pragma omp atomic write relaxed
                                    solutions[address] = new_solution_b;
                                    vertex_to_solution[b] = new_solution_b;
                                    int oldIndex;
#pragma omp atomic capture relaxed
                                    oldIndex = activeListIndex++;
                                    active_list[oldIndex] = b;
                                }
                            }
                        }
                        local_list.remove(node);
                    }
                }
#pragma omp barrier
            }
        }
    }



    std::vector<double> getSolutions() {
        std::vector<double> sol;
        sol.resize(solutions.size()/2);
        for(int i = 0; i < sol.size(); i++) {
            sol[i] = solutions[p_out*sol.size() + i];
        }
        return sol;
    }


private:
    Mesh<D>& mesh;
    std::vector<int>& boundary_vertices;
    std::vector<int> active_list;
    std::vector<double> solutions;
    double velocity = 1;
    int threads_number;
    int p_out;
    int p_in;

    void writeLocalSolution(int address, double value) {
        #pragma omp atomic write relaxed
        solutions[address] = value;

    }

    void writeLocalSolutionEfficient(int address, double value) {
        double snap;
        snap = solutions[address];
        do{

            if(snap < value) {
                break;
            }
        }while(!__atomic_compare_exchange(&solutions[address],&snap, &value, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED));
    }

    double getLocalSolution(int vertex, const std::map<int, double>& vertex_to_solution){
        auto it = vertex_to_solution.find(vertex);
        if(it != vertex_to_solution.end()){
            return it->second;
        } else {
            double res;
            int address = p_in*solutions.size()/2+vertex;
            #pragma omp atomic read relaxed
            res = solutions[address];
            return res;
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


    double update(int vertex, const std::map<int, double>& vertex_to_solution) {
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
                solutions_base[j] = getLocalSolution(triangles[i+j], vertex_to_solution);
            }
            solutions.push_back(solveLocalProblem(coordinates, solutions_base));
        }

        double min = *std::min_element(solutions.begin(), solutions.end());
        return min;
    }
};


#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_TESTPARALLELEIKONALSOLVER_H
