/*
 * File:   AKQReducedSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef AKQREDUCEDSCOMPLEXSUPPLIER_HPP
#define	AKQREDUCEDSCOMPLEXSUPPLIER_HPP

#include "AKQReducedSComplexSupplier.h"

#include "AKQHomotopicPaths.h"
#include "SComplexFactory.h"

#include "HomologyHelpers.h"
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
AKQReducedSComplexSupplier<Traits>::AKQReducedSComplexSupplier(InputSComplexPtr inputSComplex)
    : _complex(inputSComplex)
{
    CreateAlgorithm();
}

template <typename Traits>
void AKQReducedSComplexSupplier<Traits>::CreateAlgorithm()
{
    HapCWComplexExporter<InputSComplex> exporter;
    exporter.CollectComplexData(_complex);

    _logger.Begin(FGLogger::Details, "performing coreductions");
    _algorithm = AlgorithmPtr(new Algorithm(new Strategy(*_complex)));
    //    _algorithm->setStoreReducedCells(_logger.PrintCoreducedCellsCount());
    _logger.Log(FGLogger::Details) << "alg started" << std::endl;
    (*_algorithm)();
    _logger.Log(FGLogger::Details) << "alg ended" << std::endl;
    if (_logger.PrintCoreducedCellsCount())
    {
      //        _logger.Log(FGLogger::Details)<<"number of reduced pairs: "<<_algorithm->getReducedCells().size()<<std::endl;
      //  _logger.Log(FGLogger::Details)<<"number of extracted cells: "<<_algorithm->getExtractedCells().size()<<std::endl;
    }

    exporter.CollectVectorFieldData(_complex, _algorithm->getStrategy());
    exporter.ExportData("export.txt");
}

template <typename Traits>
bool AKQReducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                  std::map<Id, Chain>& _2Boundaries)
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
            cellsByDim[dim].insert(it->getId());
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
AKQReducedSComplexSupplier<Traits>::GetBoundary(const Id& cellId)
{
    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    Cell cell = (*outputComplex)[cellId];
    Chain boundary;
    BdCells bdCells = outputComplex->iterators(1).bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    for( ; it != itEnd; ++it)
    {
        int ci = outputComplex->coincidenceIndex(cell, *it);
        assert(ci != 0);
        boundary.push_back(std::pair<Id, int>(it->getId(), ci));
    }
    return boundary;
}

template <typename Traits>
void AKQReducedSComplexSupplier<Traits>::PrintDebug()
{
    _logger.Log(FGLogger::Debug)<<"extracted signature:"<<std::endl<<_algorithm->getExtractedSignature()<<std::endl;

    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    std::vector<int> betti = HomologyHelpers<Traits>::GetHomologySignature(outputComplex);

    _logger.Log(FGLogger::Debug)<<"homology signature:"<<std::endl;
    for (int i = 0; i < betti.size(); ++i)
    {
        _logger.Log(FGLogger::Debug)<<"H_"<<i<<" = Z^"<<betti[i]<<std::endl;
    }

    for (int dim = 0; dim <= 2; ++dim)
    {
        DimCells dimCells = outputComplex->iterators(1).dimCells(static_cast<Dim>(dim));
        typename DimCells::iterator it = dimCells.begin();
        typename DimCells::iterator itEnd = dimCells.end();
        for ( ; it != itEnd; ++it)
        {
            Cell cell = *it;
            _logger.Log(FGLogger::Debug)<< " Cell " << cell.getId() << " in dimension: " << cell.getDim() << std::endl;
            BdCells bdCells = outputComplex->iterators(1).bdCells(cell);
            typename BdCells::iterator it2 = bdCells.begin();
            typename BdCells::iterator it2End = bdCells.end();
            for ( ; it2 != it2End; ++it2)
            {
                int ci = outputComplex->coincidenceIndex(cell, *it2);
                _logger.Log(FGLogger::Debug)<< "   bd " << it2->getId() << " ci: " << ci << std::endl;
            }
        }
    }
}

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_HPP */

