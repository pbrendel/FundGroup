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

template <typename Traits>
AKQReducedSComplexSupplier<Traits>::AKQReducedSComplexSupplier(const char* filename)
{
    _complex = SComplexFactory<InputSComplex>::Load(filename);
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
    _logger.Begin(FGLogger::Details, "performing coreductions");
    _algorithm = AlgorithmPtr(new Algorithm(new Strategy(*_complex)));
    _logger.Log(FGLogger::Details) << "algorithm started" << std::endl;
    size_t reducedCount = (*_algorithm)();
    _logger.Log(FGLogger::Details) << "algorithm ended" << std::endl;
    if (_logger.PrintCoreducedCellsCount())
    {
        _logger.Log(FGLogger::Details)<<"number of reduced pairs: "<<reducedCount<<std::endl;
        _logger.Log(FGLogger::Details)<<"number of extracted cells: "<<_algorithm->getExtractedSignature().size()<<std::endl;
    }
}

template <typename Traits>
bool AKQReducedSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                  std::map<Id, Chain>& _2Boundaries)
{
    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    int maxDim = static_cast<int>(outputComplex->getDim());
    cellsByDim.resize(std::max(3, maxDim + 1)); //always dim 0, 1, 2
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
    AKQHomotopicPaths<AKQReducedSComplexSupplier<Traits> > homotopicPaths(this, _algorithm->getStrategy());
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
template <typename ComplexType>
std::list<std::pair<typename ComplexType::Id, int> >
AKQReducedSComplexSupplier<Traits>::GetOrdered2Boundary(ComplexType* complex,
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
void AKQReducedSComplexSupplier<Traits>::PrintDebug()
{
    _logger.Log(FGLogger::Debug)<<"extracted signature:"<<std::endl<<_algorithm->getExtractedSignature()<<std::endl;

    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    std::vector<int> betti = HomologyHelpers<Traits>::GetHomologySignature(outputComplex);
    _logger.Log(FGLogger::Debug)<<"homology signature:"<<std::endl;
    for (size_t i = 0; i < betti.size(); ++i)
    {
        _logger.Log(FGLogger::Debug)<<"H_"<<i<<" = Z^"<<betti[i]<<std::endl;
    }
}

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_HPP */
