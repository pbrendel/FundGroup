/*
 * File:   NotReducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef NOTREDUCEDSCOMPLEXSUPPLIER_HPP
#define	NOTREDUCEDSCOMPLEXSUPPLIER_HPP

#include "NotReducedSComplexSupplier.h"

#include "SComplexFactory.h"

#include "Logger.h"

template <typename Traits>
NotReducedSComplexSupplier<Traits>::NotReducedSComplexSupplier(const char* filename)
{
    _complex = SComplexFactory<InputSComplex>::Load(filename);
}

template <typename Traits>
NotReducedSComplexSupplier<Traits>::NotReducedSComplexSupplier(DebugComplexType type)
{
    _complex = SComplexFactory<InputSComplex>::Create(type);
}

template <typename Traits>
bool NotReducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                  std::map<Cell, Chain>& _2Boundaries)
{
    int maxDim = static_cast<int>(_complex->getDim());
    cellsByDim.resize(maxDim + 1);
    for (int dim = 0; dim <= maxDim; dim++)
    {
        DimCells dimCells = _complex->iterators(1).dimCells(static_cast<Dim>(dim));
        typename DimCells::iterator it = dimCells.begin();
        typename DimCells::iterator itEnd = dimCells.end();
        for ( ; it != itEnd; ++it)
        {
            cellsByDim[dim].insert(*it);
        }
    }

    // if there are some 2-cells, take its boundaries
    _2Boundaries.clear();
    if (cellsByDim.size() > 2)
    {
        Cells _2cells = cellsByDim[2];
        typename Cells::iterator it = _2cells.begin();
        typename Cells::iterator itEnd = _2cells.end();
        for ( ; it != itEnd; ++it)
        {
            _2Boundaries[*it] = GetBoundary(*it);
        }
    }
    return cellsByDim.size() > 0;
}

template <typename Traits>
typename NotReducedSComplexSupplier<Traits>::Chain
NotReducedSComplexSupplier<Traits>::GetBoundary(const Cell& cell)
{
    Chain boundary;
    BdCells bdCells = _complex->iterators(1).bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    for( ; it != itEnd; ++it)
    {
        int ci = _complex->coincidenceIndex(cell, *it);
        assert(ci != 0);
        boundary.push_back(std::pair<Cell, int>(*it, ci));
    }
    return boundary;
}

template <typename Traits>
void NotReducedSComplexSupplier<Traits>::PrintDebug()
{
}

#endif	/* NOTREDUCEDSCOMPLEXSUPPLIER_HPP */

