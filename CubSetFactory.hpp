/*
 * File:   CubSetFactory.hpp
 * Author: Piotr Brendel
 */

#ifndef CUBSETFACTORY_HPP
#define	CUBSETFACTORY_HPP

#include "CubSetFactory.h"

#include <capd/bitSet/BitmapT.hpp>
#include <capd/cubSet/acyclicConfigs.hpp>
#include <capd/bitSet/EuclBitSetT.h>

#include "FGLogger.h"

template <typename CubSetT>
typename CubSetFactory<CubSetT>::CubSetPtr
CubSetFactory<CubSetT>::Load(const char* filename, bool shave)
{
    Cubes cubes;
    Bounds bounds;
    CubesSupplier<Coord, DIM>::Load(filename, cubes, bounds);
    return Create(cubes, bounds, shave);
}

template <typename CubSetT>
typename CubSetFactory<CubSetT>::CubSetPtr
CubSetFactory<CubSetT>::Create(DebugComplexType type, bool shave)
{
    Cubes cubes;
    Bounds bounds;
    CubesSupplier<Coord, DIM>::Create(type, cubes, bounds);
    return Create(cubes, bounds, shave);
}

template <typename CubSetT>
template <typename CubCellSetPtr>
typename CubSetFactory<CubSetT>::CubSetPtr
CubSetFactory<CubSetT>::ConvertCubCellSet(CubCellSetPtr cubCellSet, bool shave)
{
    int maxDim = cubCellSet().embDim();
    assert(maxDim == DIM);

    int dimensions[maxDim];
    int tmpCoords[maxDim];
    int coords[maxDim];
    int totalCount = 1;
    for (int dim = 0; dim < maxDim; dim++)
    {
        // CubCellSet full cubes are stored at position
        // [2 * x_i + 1, ... ] where [x_i, ... ] are its actual coords
        int dimSize = (cubCellSet().getUnpaddedWidth(dim) - 1) / 2;
        totalCount *= dimSize;
        dimensions[dim] = dimSize;
        coords[dim] = 0;
        tmpCoords[dim] = 1;
    }

    CubSetPtr cubSet = CubSetPtr(new CubSet(dimensions));

    typedef typename CubSet::BitIterator Iterator;
    Iterator srcIt = Iterator(cubCellSet(), tmpCoords);
    Iterator dstIt = Iterator(cubSet(), coords);

    for (int i = 0; i < totalCount; i++)
    {
        // copy bit
        if (srcIt.getBit())
        {
            dstIt.setBit();
        }

        // increment coord
        int dim = 0;
        bool ok = false;
        while (!ok && dim < maxDim)
        {
            coords[dim]++;
            srcIt.incInDir(dim, 2);
            dstIt.incInDir(dim);
            if (coords[dim] < dimensions[dim])
            {
                ok = true;
            }
            else
            {
                srcIt.decInDir(dim, dimensions[dim] * 2);
                dstIt.decInDir(dim, dimensions[dim]);
                coords[dim] = 0;
                dim++;
            }
        }
    }

    return cubSet;
}

template <typename CubSetT>
typename CubSetFactory<CubSetT>::CubSetPtr
CubSetFactory<CubSetT>::Create(Cubes& cubes, Bounds& bounds, bool shave)
{
    FGLogger logger;
    // recalculating into RedHom CubSet internal format
    logger.Begin(FGLogger::Details, "Creating CubSet");
    std::vector<int> cubSetBounds(DIM);
    for (int i = 0; i < DIM; i++)
    {
        logger.Log(FGLogger::Debug)<<bounds[i].Size()<<std::endl;
        cubSetBounds[i] = bounds[i].Size();
    }

    CubSetPtr cubSet = CubSetPtr(new CubSet(&cubSetBounds[0]));

    // renormalizing and adding cubes
    int cube[DIM];
    size_t count = cubes.size();
    for (size_t i = 0; i < count; i++)
    {
        Cube c = cubes[i];
        for (size_t j = 0; j < DIM; j++)
        {
            // recalculating into RedHom CubSet internal format
            cube[j] = c[j] - bounds[j]._min;
        }
        cubSet().insert(&cube[0]);
    }
    cubSet().addEmptyCollar();
    logger.End();

    if (shave)
    {
        logger.Begin(FGLogger::Details, "loading acyclic configs");
        readAcyclicConfigs();
        logger.End();
        logger.Begin(FGLogger::Details, "shaving");
        cubSet().shaveBI();
        if (logger.PrintShavedCellsCount())
        {
            count = count - static_cast<size_t>(cubSet().cardinality());
            logger.Log(FGLogger::Details)<<"shaved "<<count<<" cubes"<<std::endl;
        }
        logger.End();
    }

    return cubSet;
}

#endif	/* CUBSETFACTORY_HPP */
