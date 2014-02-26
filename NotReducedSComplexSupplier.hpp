/*
 * File:   NotReducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef NOTREDUCEDSCOMPLEXSUPPLIER_HPP
#define	NOTREDUCEDSCOMPLEXSUPPLIER_HPP

#include "NotReducedSComplexSupplier.h"

#include "SComplexFactory.h"

#include "HapCWComplexExporter.h"

template <typename Traits>
NotReducedSComplexSupplier<Traits>::NotReducedSComplexSupplier(const char* filename)
{
    _complex = SComplexFactory<InputSComplex>::Load(filename);

    HapCWComplexExporter<InputSComplex> exporter;
    exporter.CollectComplexData(_complex);
    exporter.GenerateTrivialVectorField(_complex);
    exporter.ExportData("export.txt");

    // typedef ShaveAlgorithmFactory<InputSComplex> ShaveFactory;
    // boost::shared_ptr<typename ShaveFactory::DefaultAlgorithm> shave = ShaveFactory().createDefault(*_complex);
    // (*shave)();
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
            _2Boundaries[*it] = GetBoundary(*it);
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
void NotReducedSComplexSupplier<Traits>::PrintDebug()
{
}

#endif	/* NOTREDUCEDSCOMPLEXSUPPLIER_HPP */
