//
// Created by tomma on 20/11/2023.
//

#ifndef EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CREATINGLOOPER_H
#define EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CREATINGLOOPER_H
#include <vector>

template< typename T, int ...Ints>
struct CreatingLooper;

template< typename T, int I, int ...N>
struct CreatingLooper<T,I,N...> {
    constexpr auto operator()() {
        CreatingLooper<T, N...> looper;
        std::vector<decltype(looper())> newVec;
        newVec.resize(I);
        for(int i = 0; i < I; i++) {
            auto vec = looper();
            newVec[i] = vec;
        }
        return newVec;
    }
};

template< typename T, int I>
struct CreatingLooper<T, I> {
    constexpr auto operator()() {
        std::vector<T> newVec;
        newVec.resize(I);
        return newVec;
    }
};
#endif //EIKONAL_CESARONI_TONARELLI_TRABACCHIN_CREATINGLOOPER_H
