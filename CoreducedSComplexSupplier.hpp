/*
 * File:   CoreducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef COREDUCEDSCOMPLEXSUPPLIER_HPP
#define	COREDUCEDSCOMPLEXSUPPLIER_HPP

#include "CoreducedSComplexSupplier.h"

#include <iostream>

#include "HomologyHelpers.hpp"
#include "SComplexFactory.h"

template <typename Traits>
CoreducedSComplexSupplier<Traits>::CoreducedSComplexSupplier(const char* filename)
{
    _complex = SComplexFactory<SComplex>::Load(filename);
    CreateFilter();
}

template <typename Traits>
CoreducedSComplexSupplier<Traits>::CoreducedSComplexSupplier(DebugComplexType debugComplexType)
{
    _complex = SComplexFactory<SComplex>::Create(debugComplexType);
    CreateFilter();
}

template <typename Traits>
void CoreducedSComplexSupplier<Traits>::CreateFilter()
{
    _filter = HomologyHelpers<Traits>::CreateFilter(_complex, false, false, false);
}

template <typename Traits>
bool CoreducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                 std::map<Cell, Chain>& _2Boundaries)
{
    int maxDim = static_cast<int>(_filter->topDim());
    cellsByDim.resize(maxDim + 1);
    for (int dim = 0; dim <= maxDim; dim++)
    {
        std::vector<Chain> chains = _filter->baseHomologyChains()[dim];
        for (int i = 0; i < chains.size(); i++)
        {
            typename Chain::iterator it = chains[i].begin();
            typename Chain::iterator itEnd = chains[i].end();
            for ( ; it != itEnd; ++it)
            {
                Cell cell = it->first;
                cellsByDim[dim].insert(cell);
            }
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
typename CoreducedSComplexSupplier<Traits>::Chain
CoreducedSComplexSupplier<Traits>::GetBoundary(const Cell& cell)
{
    Chain boundary = _filter->getBoundary(cell);
    _filter->reduce(boundary);
    return boundary;
}

template <typename Traits>
void CoreducedSComplexSupplier<Traits>::PrintDebug()
{
    typedef HomologyHelpers<Traits> Homology;

    std::vector<int> betti = Homology::GetBettiNumbers(_complex);
    std::cout<<"betti numbers of given complex:"<<std::endl;
    for (int i = 0; i < betti.size(); i++)
    {
        std::cout<<"B["<<i<<"] = "<<betti[i]<<std::endl;
    }

    std::cout<<"homology chains:"<<std::endl;
    for (int i = 0; i <= _filter->topDim(); i++)
    {
        std::cout<<"dim "<<i<<":"<<std::endl;
        for ( int j = 0; j < _filter->baseHomologyChains()[i].size(); j++)
        {
            std::cout<<_filter->baseHomologyChains()[i][j]<<", "<<std::endl;
        }
        std::cout<<std::endl;
    }
}

#endif	/* COREDUCEDSCOMPLEXSUPPLIER_HPP */

