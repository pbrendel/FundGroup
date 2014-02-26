/*
 * File:   AKQReducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef AKQREDUCEDSCOMPLEXSUPPLIER_HPP
#define	AKQREDUCEDSCOMPLEXSUPPLIER_HPP

#include "AKQReducedSComplexSupplier.h"

#include "AKQHomotopicPaths.h"
#include "SComplexFactory.h"

#include "HapCWComplexExporter.h"

template <typename Traits>
AKQReducedSComplexSupplier<Traits>::AKQReducedSComplexSupplier(const char* filename)
{
    _complex = SComplexFactory<InputSComplex>::Load(filename);


    for ( int i = 0; i <= 3; i++)
    {
        int count = 0;
        typename InputSComplex::ColoredIterators::Iterators::DimCells dimCells = _complex->iterators(1).dimCells(i);
        typename InputSComplex::ColoredIterators::Iterators::DimCells::iterator it = dimCells.begin();
        typename InputSComplex::ColoredIterators::Iterators::DimCells::iterator itEnd = dimCells.end();
        for ( ; it != itEnd; ++it)
        {
            count++;
        }
        _logger.Log(FGLogger::Debug)<<"number of cells in dim "<<i<<" = "<<count<<std::endl;
    }


    CreateAlgorithm();
}

template <typename Traits>
AKQReducedSComplexSupplier<Traits>::AKQReducedSComplexSupplier(DebugComplexType type)
{
    _complex = SComplexFactory<InputSComplex>::Create(type);
    CreateAlgorithm();
}

template <typename Traits>
void AKQReducedSComplexSupplier<Traits>::CreateAlgorithm()
{
    HapCWComplexExporter<InputSComplex> exporter;
    exporter.CollectComplexData(_complex);

    _logger.Begin(FGLogger::Details, "performing coreductions");
    _algorithm = AlgorithmPtr(new Algorithm(new Strategy(*_complex)));
    _algorithm->setStoreReducedCells(_logger.PrintCoreducedCellsCount());
    (*_algorithm)();
    _logger.End("coreductions finished");
    if (_logger.PrintCoreducedCellsCount())
    {
        _logger.Log(FGLogger::Details)<<"number of reduced pairs: "<<_algorithm->getReducedCells().size()<<std::endl;
        _logger.Log(FGLogger::Details)<<"number of extracted cells: "<<_algorithm->getExtractedCells().size()<<std::endl;
    }

    exporter.CollectVectorFieldData(_complex, _algorithm->getStrategy());
    exporter.ExportData("export.txt");
}

template <typename Traits>
bool AKQReducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                  std::map<Cell, Chain>& _2Boundaries)
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
    AKQHomotopicPaths<AKQReducedSComplexSupplier<Traits> > homotopicPaths(_algorithm->getStrategy());
    _2Boundaries.clear();
    if (cellsByDim.size() > 2)
    {
        Cells _2cells = cellsByDim[2];
        typename Cells::iterator it = _2cells.begin();
        typename Cells::iterator itEnd = _2cells.end();
        for ( ; it != itEnd; ++it)
        {
            _2Boundaries[*it] = homotopicPaths.GetHomotopicBoundary(*it);
            //_2Boundaries[*it] = GetBoundary(*it);
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
        int ci = outputComplex->coincidenceIndex(cell, *it);
        assert(ci != 0);
        boundary.push_back(std::pair<Cell, int>(*it, ci));
    }
    return boundary;
}

template <typename Traits>
void AKQReducedSComplexSupplier<Traits>::PrintDebug()
{
    _logger.Log(FGLogger::Debug)<<"homology signature:"<<std::endl<<_algorithm->getExtractedSignature()<<std::endl;
}

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_HPP */

