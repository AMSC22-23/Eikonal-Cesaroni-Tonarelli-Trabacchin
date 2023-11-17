//
// Created by sabri on 17/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_VERTEX_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_VERTEX_H
#include <array>
#include <cmath>
#include <numeric>
template <int N>
class Vertex {
public:
    Vertex() {
        for(int i = 0; i < N; i++) {
            this->coord[i] = 0.0;
        }
    }

    Vertex(const std::array<double, N> &input) {
        this->coord = input;
    }

    double norm() const {
        double sum = 0;
        for(int i = 0; i < N; i++) {
            sum += this->coord[i] * this->coord[i];
        }
        return sqrt(sum);
    }

    double operator* (const Vertex<N>& other) const {
        return std::inner_product(this->coord.begin(), this->coord.end(), other.coord.begin());
    }

    Vertex& operator+ (const Vertex<N>& other) const {
        Vertex sum;
        for(int i = 0; i < N; i++) {
            sum.coord [i] = this->coord[i] + other.coord[i];
        }
        return sum;
    }

    Vertex& operator- (const Vertex<N>& other) const {
        Vertex diff;
        for(int i = 0; i < N; i++) {
            diff.coord [i] = this->coord[i] - other.coord[i];
        }
        return diff;
    }

    double operator[] (int index) const{
        return this->coord[index];
    }

private:
    std::array<double, N> coord;
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_VERTEX_H
