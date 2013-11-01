/*
 * File:   CubSetFactory.hpp
 * Author: Piotr Brendel
 */

#ifndef CUBSETFACTORY_HPP
#define	CUBSETFACTORY_HPP

#include "CubSetFactory.h"

#include <capd/bitSet/BitmapT.hpp>
#include <capd/cubSet/acyclicConfigs.hpp>
#include <capd/bitSet/EuclBitSetT.hpp>

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
typename CubSetFactory<CubSetT>::CubSetPtr
CubSetFactory<CubSetT>::Create(Cubes& cubes, Bounds& bounds, bool shave)
{
   // recalculating into RedHom CubSet internal format
    Logger::Begin(Logger::Details, "Creating CubSet");
    std::vector<int> cubSetBounds(DIM);
    for (int i = 0; i < DIM; i++)
    {
        Logger::Log(Logger::Debug)<<bounds[i].Size()<<std::endl;
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
    Logger::End();

    if (shave)
    {
        Logger::Begin(Logger::Details, "loading acyclic configs");
        readAcyclicConfigs();
        Logger::End();
        Logger::Begin(Logger::Details, "shaving");
        cubSet().shaveBI();
        if (Logger::PrintShavedCellsCount())
        {
            count = count - static_cast<size_t>(cubSet().cardinality());
            Logger::Log(Logger::Details)<<"shaved "<<count<<" cubes"<<std::endl;
        }
        Logger::End();
    }

    return cubSet;
}

#endif	/* CUBSETFACTORY_HPP */

