/*
 * File:   NotReducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef NOTREDUCEDSCOMPLEXSUPPLIER_HPP
#define	NOTREDUCEDSCOMPLEXSUPPLIER_HPP

#include "NotReducedSComplexSupplier.h"

#include "SComplexFactory.h"

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
NotReducedSComplexSupplier<Traits>::NotReducedSComplexSupplier(InputSComplexPtr inputSComplex)
    : _complex(inputSComplex)
{
}

template <typename Traits>
bool NotReducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                  std::map<Id, Chain>& _2Boundaries)
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
            assert(it->getDim() == dim);
            cellsByDim[dim].insert(it->getId());
        }
    }

    // if there are some 2-cells, take its boundaries
    _2Boundaries.clear();
    if (cellsByDim.size() > 2)
    {
        Cells& _2cells = cellsByDim[2];
        typename Cells::iterator it = _2cells.begin();
        typename Cells::iterator itEnd = _2cells.end();
        for ( ; it != itEnd; ++it)
        {
            std::list<std::pair<Id, int> > bdList = GetOrdered2Boundary(_complex.get(), *it);
            _2Boundaries[*it].assign(bdList.begin(), bdList.end());
        }
    }
    return cellsByDim.size() > 0;
}

template <typename Traits>
typename NotReducedSComplexSupplier<Traits>::Chain
NotReducedSComplexSupplier<Traits>::GetBoundary(const Id& cellId)
{
    typedef typename OutputSComplex::Cell Cell;
    Cell cell = (*_complex)[cellId];
    assert(cell.getColor() == 1);

    Chain boundary;
    BdCells bdCells = _complex->iterators(1).bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    for( ; it != itEnd; ++it)
    {
        assert(it->getColor() == 1);
        int ci = _complex->coincidenceIndex(cell, *it);
        assert(ci != 0);
        assert(ci == -1 || ci == 1);
        boundary.push_back(std::pair<Id, int>(it->getId(), ci));
    }
    return boundary;
}

template <typename Traits>
template <typename ComplexType>
std::list<std::pair<typename ComplexType::Id, int> >
NotReducedSComplexSupplier<Traits>::GetOrdered2Boundary(ComplexType* complex,
                                            const typename ComplexType::Id& cellId)
{
    typedef typename ComplexType::Id Id;
    typedef typename ComplexType::Cell Cell;
    typedef typename ComplexType::Iterators::BdCells BdCells;
    Cell cell = (*complex)[cellId];
    std::list<std::pair<Id, int> > boundary;
    BdCells bdCells = complex->iterators().bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    for( ; it != itEnd; ++it)
    {
        int ci = complex->coincidenceIndex(cell, *it);
        assert(ci != 0);
        boundary.push_back(std::pair<Id, int>(it->getId(), ci));
    }
    HomologyHelpers<Traits>::Reorder2Boundary(complex, boundary);
    return boundary;
}

template <typename Traits>
void NotReducedSComplexSupplier<Traits>::PrintDebug()
{
}

#endif	/* NOTREDUCEDSCOMPLEXSUPPLIER_HPP */
