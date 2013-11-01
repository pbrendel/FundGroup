/*
 * File:   CubCellSetFactory.h
 * Author: Piotr Brendel
 */

#ifndef CUBCELLSETFACTORY_H
#define	CUBCELLSETFACTORY_H

#include "CubesSupplier.h"

template <typename CubCellSetT>
class CubCellSetFactory
{
public:

    typedef CubCellSetT                     CubCellSet;
    typedef CRef<CubCellSet>                CubCellSetPtr;
    typedef typename CubCellSet::EuclSet    EuclSet;
    typedef size_t                          Coord;
    enum
    {
        DIM = CubCellSet::DIM,
    };

    static CubCellSetPtr Load(const char* filename, bool shave);
    static CubCellSetPtr Create(DebugComplexType type, bool shave);

private:

    typedef typename CubesSupplier<Coord, DIM>::Cube    Cube;
    typedef typename CubesSupplier<Coord, DIM>::Cubes   Cubes;
    typedef typename CubesSupplier<Coord, DIM>::Bounds  Bounds;

    static CubCellSetPtr Create(Cubes& cubes, Bounds& bounds, bool shave);
};

#include "CubCellSetFactory.hpp"

#endif	/* CUBCELLSETFACTORY_H */

