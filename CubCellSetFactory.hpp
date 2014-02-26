/*
 * File:   CubCellSetFactory.hpp
 * Author: Piotr Brendel
 */

#ifndef CUBCELLSETFACTORY_HPP
#define	CUBCELLSETFACTORY_HPP

#include "CubCellSetFactory.h"

#include <capd/bitSet/BitmapT.hpp>
#include <capd/bitSet/EuclBitSetT.h>
#include <capd/repSet/ECellMDCodeT.h>

#include "FGLogger.h"

template <typename CubCellSetT>
typename CubCellSetFactory<CubCellSetT>::CubCellSetPtr
CubCellSetFactory<CubCellSetT>::Load(const char* filename, bool shave)
{
    Cubes cubes;
    Bounds bounds;
    CubesSupplier<Coord, DIM>::Load(filename, cubes, bounds);
    return Create(cubes, bounds, shave);
}

template <typename CubCellSetT>
typename CubCellSetFactory<CubCellSetT>::CubCellSetPtr
CubCellSetFactory<CubCellSetT>::Create(DebugComplexType type, bool shave)
{
    Cubes cubes;
    Bounds bounds;
    CubesSupplier<Coord, DIM>::Create(type, cubes, bounds);
    return Create(cubes, bounds, shave);
}

template <typename CubCellSetT>
typename CubCellSetFactory<CubCellSetT>::CubCellSetPtr
CubCellSetFactory<CubCellSetT>::Create(Cubes& cubes, Bounds& bounds, bool shave)
{
    FGLogger logger;
    // recalculating into RedHom CubCellSet internal format
    logger.Begin(FGLogger::Details, "Creating CubCellSet");
    std::vector<int> cubCellSetBounds(DIM);
    for (int i = 0; i < DIM; i++)
    {
        logger.Log(FGLogger::Debug)<<bounds[i].Size()<<std::endl;
        cubCellSetBounds[i] = 2 * bounds[i].Size() + 1;
    }
    CubCellSetPtr cubCellSet = CubCellSetPtr(new CubCellSet(&cubCellSetBounds[0], true));

    // renormalizing and adding cubes
    int cube[DIM];
    size_t count = cubes.size();
    for (size_t i = 0; i < count; i++)
    {
        Cube c = cubes[i];
        for (size_t j = 0; j < DIM; j++)
        {
            // recalculating into RedHom CubCellSet internal format
            cube[j] = 2 * c[j] - bounds[j]._min + 1;
        }
        cubCellSet().insert(&cube[0]);
    }
    logger.End();

    logger.Begin(FGLogger::Details, "creating lower dimensional cubes");
    cubCellSet().fillWithSubEmbDimCells();
    logger.End();

    if (shave)
    {
        logger.Begin(FGLogger::Details, "shaving");
        count = static_cast<size_t>(cubCellSet().shave());
        if (logger.PrintShavedCellsCount())
        {
            logger.Log(FGLogger::Details)<<"shaved "<<count<<" cubes"<<std::endl;
        }
        logger.End();
    }

    return cubCellSet;
}

#endif	/* CUBCELLSETFACTORY_HPP */
