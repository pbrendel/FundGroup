/*
 * File:   CubesSupplier.h
 * Author: Piotr Brendel
 */

#ifndef CUBESSUPPLIER_H
#define	CUBESSUPPLIER_H

#include <vector>

#include "DebugComplexType.h"

template <typename T, int DIM>
class CubesSupplier
{
public:

    struct Bound
    {
        T _min;
        T _max;

        Bound();
        Bound(T size);
        Bound(T min, T max);

        void Update(T coord);
        size_t Size();
    };

    typedef T                                       Coord;
    typedef std::vector<T>                          Cube;
    typedef std::vector<Cube>                       Cubes;
    typedef std::vector<Bound>                      Bounds;

    static void Load(const char* filename, Cubes& cubes, Bounds& bounds);
    static void Create(DebugComplexType type, Cubes& cubes, Bounds& bounds);

private:

    static void FillSphere2(Cubes& cubes, Bounds& bounds);
    static void FillSphere3(Cubes& cubes, Bounds& bounds);
    static void FillTorus(Cubes& cubes, Bounds& bounds);
    static void FillSkeleton(Cubes& cubes, Bounds& bounds);
    static void FillCustom0(Cubes& cubes, Bounds& bounds);
};

#include "CubesSupplier.hpp"

#endif	/* CUBESSUPPLIER_H */

