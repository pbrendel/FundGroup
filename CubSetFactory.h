/*
 * File:   CubSetFactory.h
 * Author: Piotr Brendel
 */

#ifndef CUBSETFACTORY_H
#define	CUBSETFACTORY_H

#include "CubesSupplier.h"

template <typename CubSetT>
class CubSetFactory
{
public:

    typedef CubSetT                     CubSet;
    typedef CRef<CubSet>                CubSetPtr;
    typedef int                         Coord;
    enum
    {
        DIM = CubSet::theDim,
    };

    static CubSetPtr Load(const char* filename, bool shave);
    static CubSetPtr Create(DebugComplexType type, bool shave);

    template <typename CubCellSetPtr>
    static CubSetPtr ConvertCubCellSet(CubCellSetPtr cubCellSet, bool shave);

private:

    typedef typename CubesSupplier<Coord, DIM>::Cube    Cube;
    typedef typename CubesSupplier<Coord, DIM>::Cubes   Cubes;
    typedef typename CubesSupplier<Coord, DIM>::Bounds  Bounds;

    static CubSetPtr Create(Cubes& cubes, Bounds& bounds, bool shave);
};

#include "CubSetFactory.hpp"

#endif	/* CUBSETFACTORY_H */

