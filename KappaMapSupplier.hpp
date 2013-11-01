/*
 * File:   KappaMapSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef KAPPAMAPSUPPLIER_HPP
#define	KAPPAMAPSUPPLIER_HPP

#include "KappaMapSupplier.h"

#include <fstream>

#include "Logger.h"

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::Load(const char* filename,
                                               Dims& dims,
                                               KappaMap& kappaMap)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    dims.clear();
    kappaMap.clear();
    size_t topDim = 0;
    size_t totalCellsCount = 0;
    size_t kappaMapSize = 0;

    Logger::Begin(Logger::Details, "reading kappa map");
    input>>topDim;
    Logger::Log(Logger::Details)<<"top dim: "<<topDim<<std::endl;
    for (int dim = 0; dim <= topDim; dim++)
    {
        int cellsCount = 0;
        input>>cellsCount;
        Logger::Log(Logger::Details)<<cellsCount<<" cells in dim "<<dim<<std::endl;
        totalCellsCount += cellsCount;
        for (int i = 0; i < cellsCount; i++)
        {
            dims.push_back(static_cast<Dim>(dim));
        }
        // for every cell there is 2 boundary cells in each dimension
        kappaMapSize += dim * cellsCount * 2;
    }
    Logger::Log(Logger::Details)<<"total cells count = "<<totalCellsCount<<std::endl;

    for (int i = 0; i < kappaMapSize; i++)
    {
        int cell;
        int boundary;
        int index;
        input>>cell;
        input>>boundary;
        input>>index;
        kappaMap.push_back(KappaMapEntry(static_cast<Id>(cell),
                                         static_cast<Id>(boundary),
                                         static_cast<Index>(index)));
    }
    input.close();
    Logger::End("data read successfully");
}

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::Create(DebugComplexType type,
                                                 Dims& dims,
                                                 KappaMap& kappaMap)
{
    dims.clear();
    kappaMap.clear();
    switch (type)
    {
        case DCT_Sphere2:
            FillSphere2(dims, kappaMap);
            break;
        case DCT_Sphere3:
            FillSphere3(dims, kappaMap);
            break;
        case DCT_Torus:
            FillTorus(dims, kappaMap);
            break;
        case DCT_Skeleton:
            FillSkeleton(dims, kappaMap);
            break;
        case DCT_Custom0:
            FillCustom0(dims, kappaMap);
            break;
        default:
            throw std::logic_error("not implemented");
    }
}

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::FillSphere2(Dims& dims, KappaMap& kappaMap)
{
    // filling with 2-sphere (edges of a triangle)
    //                index - label
    dims.push_back(Dim(0)); // 0 - [0]
    dims.push_back(Dim(0)); // 1 - [1]
    dims.push_back(Dim(0)); // 2 - [2]
    dims.push_back(Dim(1)); // 3 - [0, 1]
    dims.push_back(Dim(1)); // 4 - [1, 2]
    dims.push_back(Dim(1)); // 5 - [0, 2]
    kappaMap.push_back(KappaMapEntry(3, 0, -1));
    kappaMap.push_back(KappaMapEntry(3, 1, 1));
    kappaMap.push_back(KappaMapEntry(4, 1, -1));
    kappaMap.push_back(KappaMapEntry(4, 2, 1));
    kappaMap.push_back(KappaMapEntry(5, 2, -1));
    kappaMap.push_back(KappaMapEntry(5, 0, 1));
}

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::FillSphere3(Dims& dims, KappaMap& kappaMap)
{
    // filling with 3-sphere (an empty tetrahedron)
    //                index - label
    dims.push_back(Dim(0)); // 0 - [0]
    dims.push_back(Dim(0)); // 1 - [1]
    dims.push_back(Dim(0)); // 2 - [2]
    dims.push_back(Dim(0)); // 3 - [3]
    dims.push_back(Dim(1)); // 4 - [0, 1]
    dims.push_back(Dim(1)); // 5 - [0, 2]
    dims.push_back(Dim(1)); // 6 - [0, 3]
    dims.push_back(Dim(1)); // 7 - [1, 2]
    dims.push_back(Dim(1)); // 8 - [1, 3]
    dims.push_back(Dim(1)); // 9 - [2, 3]
    dims.push_back(Dim(2)); // 10 - [0, 1, 2]
    dims.push_back(Dim(2)); // 11 - [0, 1, 3]
    dims.push_back(Dim(2)); // 12 - [0, 2, 3]
    dims.push_back(Dim(2)); // 13 - [1, 2, 3]
    kappaMap.push_back(KappaMapEntry(4, 0, -1));
    kappaMap.push_back(KappaMapEntry(4, 1, 1));
    kappaMap.push_back(KappaMapEntry(5, 0, -1));
    kappaMap.push_back(KappaMapEntry(5, 2, 1));
    kappaMap.push_back(KappaMapEntry(6, 0, -1));
    kappaMap.push_back(KappaMapEntry(6, 3, 1));
    kappaMap.push_back(KappaMapEntry(7, 1, -1));
    kappaMap.push_back(KappaMapEntry(7, 2, 1));
    kappaMap.push_back(KappaMapEntry(8, 1, -1));
    kappaMap.push_back(KappaMapEntry(8, 3, 1));
    kappaMap.push_back(KappaMapEntry(9, 2, -1));
    kappaMap.push_back(KappaMapEntry(9, 3, 1));
    kappaMap.push_back(KappaMapEntry(10, 4, 1));
    kappaMap.push_back(KappaMapEntry(10, 5, -1));
    kappaMap.push_back(KappaMapEntry(10, 7, 1));
    kappaMap.push_back(KappaMapEntry(11, 4, 1));
    kappaMap.push_back(KappaMapEntry(11, 6, -1));
    kappaMap.push_back(KappaMapEntry(11, 8, 1));
    kappaMap.push_back(KappaMapEntry(12, 5, 1));
    kappaMap.push_back(KappaMapEntry(12, 6, -1));
    kappaMap.push_back(KappaMapEntry(12, 9, 1));
    kappaMap.push_back(KappaMapEntry(13, 7, 1));
    kappaMap.push_back(KappaMapEntry(13, 8, -1));
    kappaMap.push_back(KappaMapEntry(13, 9, 1));
}

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::FillTorus(Dims& dims, KappaMap& kappaMap)
{
    throw std::logic_error("not implemented");
}

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::FillSkeleton(Dims& dims, KappaMap& kappaMap)
{
    // filling with "edges of tetrahedron" in R^3
    dims.push_back(Dim(0)); // 0 - [0]
    dims.push_back(Dim(0)); // 1 - [1]
    dims.push_back(Dim(0)); // 2 - [2]
    dims.push_back(Dim(0)); // 3 - [3]
    dims.push_back(Dim(1)); // 4 - [0, 1]
    dims.push_back(Dim(1)); // 5 - [0, 2]
    dims.push_back(Dim(1)); // 6 - [0, 3]
    dims.push_back(Dim(1)); // 7 - [1, 2]
    dims.push_back(Dim(1)); // 8 - [1, 3]
    dims.push_back(Dim(1)); // 9 - [2, 3]
    kappaMap.push_back(KappaMapEntry(4, 0, -1));
    kappaMap.push_back(KappaMapEntry(4, 1, 1));
    kappaMap.push_back(KappaMapEntry(5, 0, -1));
    kappaMap.push_back(KappaMapEntry(5, 2, 1));
    kappaMap.push_back(KappaMapEntry(6, 0, -1));
    kappaMap.push_back(KappaMapEntry(6, 3, 1));
    kappaMap.push_back(KappaMapEntry(7, 1, -1));
    kappaMap.push_back(KappaMapEntry(7, 2, 1));
    kappaMap.push_back(KappaMapEntry(8, 1, -1));
    kappaMap.push_back(KappaMapEntry(8, 3, 1));
    kappaMap.push_back(KappaMapEntry(9, 2, -1));
    kappaMap.push_back(KappaMapEntry(9, 3, 1));
}

template <typename IdT, typename IndexT, typename DimT>
void KappaMapSupplier<IdT, IndexT, DimT>::FillCustom0(Dims& dims, KappaMap& kappaMap)
{
    throw std::logic_error("not implemented");
}

#endif	/* KAPPAMAPSUPPLIER_HPP */

