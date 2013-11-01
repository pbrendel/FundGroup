/*
 * File:   AKQReducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef AKQREDUCEDSCOMPLEXSUPPLIER_HPP
#define	AKQREDUCEDSCOMPLEXSUPPLIER_HPP

#include "AKQReducedSComplexSupplier.h"

#include "HomologyHelpers.h"
#include "SComplexFactory.h"

#include "Logger.h"

template <typename Traits>
AKQReducedSComplexSupplier<Traits>::AKQReducedSComplexSupplier(const char* filename)
{
    _complex = SComplexFactory<InputSComplex>::Load(filename);
    CreateAlgorithm();
}

template <typename Traits>
AKQReducedSComplexSupplier<Traits>::AKQReducedSComplexSupplier(DebugComplexType debugComplexType)
{
    _complex = SComplexFactory<InputSComplex>::Create(debugComplexType);
    CreateAlgorithm();
}

template <typename Traits>
void AKQReducedSComplexSupplier<Traits>::CreateAlgorithm()
{
    Logger::Begin(Logger::Details, "performing coreductions");
    _algorithm = AlgorithmPtr(new Algorithm(new Strategy(*_complex)));
    _algorithm->setStoreReducedCells(Logger::PrintCoreducedCellsCount());
    (*_algorithm)();
    Logger::End("coreductions finished");
    if (Logger::PrintCoreducedCellsCount())
    {
        Logger::Log(Logger::Details)<<"number of reduced pairs: "<<_algorithm->getReducedCells().size()<<std::endl;
        Logger::Log(Logger::Details)<<"number of extracted cells: "<<_algorithm->getExtractedCells().size()<<std::endl;
    }
}

template <typename Traits>
bool AKQReducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                  std::map<Cell,Chain>& _2Boundaries)
{
    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    int maxDim = static_cast<int>(outputComplex->getDim());
    cellsByDim.resize(maxDim + 1);
    for (int dim = 0; dim <= maxDim; dim++)
    {
        DimCells dimCells = outputComplex->iterators(1).dimCells(static_cast<Dim>(dim));
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
typename AKQReducedSComplexSupplier<Traits>::Chain
AKQReducedSComplexSupplier<Traits>::GetBoundary(const Cell& cell)
{
    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    Chain boundary;
    BdCells bdCells = outputComplex->iterators(1).bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    for( ; it != itEnd; ++it)
    {
        boundary[*it] = outputComplex->coincidenceIndex(cell, *it);
    }
    return boundary;
}

template <typename Traits>
void AKQReducedSComplexSupplier<Traits>::PrintDebug()
{
    Logger::Log(Logger::Debug)<<"homology signature:"<<std::endl<<_algorithm->getExtractedSignature()<<std::endl;
}

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_HPP */

