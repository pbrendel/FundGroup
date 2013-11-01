/*
 * File:   CollapsedAKQReducedCubSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_HPP
#define	COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_HPP

#include "CollapsedAKQReducedCubSComplexSupplier.h"

#include "CubSetFactory.h"
#include "HomologyHelpers.h"
#include "SComplexFactory.h"

#include "Logger.h"

template <typename Traits>
CollapsedAKQReducedCubSComplexSupplier<Traits>::CollapsedAKQReducedCubSComplexSupplier(const char* filename)
{
    CubSetPtr cubSet = CubSetFactory<CubSet>::Load(filename, true);
    CreateComplex(cubSet, true);
    CreateAlgorithm();
}

template <typename Traits>
CollapsedAKQReducedCubSComplexSupplier<Traits>::CollapsedAKQReducedCubSComplexSupplier(DebugComplexType type)
{
    CubSetPtr cubSet = CubSetFactory<CubSet>::Create(type, true);
    CreateComplex(cubSet, true);
    CreateAlgorithm();
}

template <typename Traits>
void CollapsedAKQReducedCubSComplexSupplier<Traits>::CreateComplex(CubSetPtr cubSet, bool collapse)
{
    if (collapse)
    {
        Logger::Begin(Logger::Details, "computing acyclic subspace");

        // acyclic subspace algorithm setup
        typedef typename CubSet::CubSetT_CubSetT_Ptr AccSubAlgorithm;
        AccSubAlgorithm accSubAlgorithm = &CubSet::acyclicSubspaceBI;
        CubSet::neighbAcyclicBI = &CubSet::neighbAcyclicLT;

        // creating copy of original cubical set
        CubSet acyclicSubset(cubSet());

        // compute acyclic subspace
        (cubSet().*accSubAlgorithm)(acyclicSubset);
        Logger::Log(Logger::Details)<<"computed acyclic subset size: "<<acyclicSubset.cardinality()<<std::endl;
        Logger::Log(Logger::Details)<<"cubes left: "<<cubSet().cardinality()<<std::endl;

        Logger::End();
    }

    Logger::Begin(Logger::Details, "creating SComplex from cubical set");
    _complex = SComplexFactory<InputSComplex>::Create(cubSet);
    Logger::End();
}

template <typename Traits>
void CollapsedAKQReducedCubSComplexSupplier<Traits>::CreateAlgorithm()
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
bool CollapsedAKQReducedCubSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
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
typename CollapsedAKQReducedCubSComplexSupplier<Traits>::Chain
CollapsedAKQReducedCubSComplexSupplier<Traits>::GetBoundary(const Cell& cell)
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
void CollapsedAKQReducedCubSComplexSupplier<Traits>::PrintDebug()
{
    Logger::Log(Logger::Debug)<<"homology signature:"<<std::endl<<_algorithm->getExtractedSignature()<<std::endl;
}

#endif	/* COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_HPP */

