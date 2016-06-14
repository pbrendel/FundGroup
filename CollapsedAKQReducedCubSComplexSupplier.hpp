/*
 * File:   CollapsedAKQReducedCubSComplexSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_HPP
#define	COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_HPP

#include "CollapsedAKQReducedCubSComplexSupplier.h"

#include "AKQHomotopicPaths.h"
#include "CubSetFactory.h"
#include "SComplexFactory.h"
#include <capd/cubSet/CubSetT.hpp>

template <typename Traits>
CollapsedAKQReducedCubSComplexSupplier<Traits>::CollapsedAKQReducedCubSComplexSupplier(const char* filename)
{
    CubSetPtr cubSet = CubSetFactory<CubSet>::Load(filename, true);
    CreateComplex(cubSet);
    CreateAlgorithm();
}

template <typename Traits>
CollapsedAKQReducedCubSComplexSupplier<Traits>::CollapsedAKQReducedCubSComplexSupplier(DebugComplexType type)
{
    CubSetPtr cubSet = CubSetFactory<CubSet>::Create(type, true);
    CreateComplex(cubSet);
    CreateAlgorithm();
}

template <typename Traits>
CollapsedAKQReducedCubSComplexSupplier<Traits>::CollapsedAKQReducedCubSComplexSupplier(CubSComplexPtr cubSComplex)
{
    _logger.Begin(FGLogger::Details, "converting CubCellSet -> CubSet");
    CubSetPtr cubSet = CubSetFactory<CubSet>::ConvertCubCellSet(cubSComplex->getCubCellSet(), true);
    _logger.End();
    CreateComplex(cubSet);
    CreateAlgorithm();
}

template <typename Traits>
void CollapsedAKQReducedCubSComplexSupplier<Traits>::CreateComplex(CubSetPtr cubSet)
{
    _logger.Begin(FGLogger::Details, "computing acyclic subspace");

    // acyclic subspace algorithm setup
    typedef typename CubSet::CubSetT_CubSetT_Ptr AccSubAlgorithm;
    AccSubAlgorithm accSubAlgorithm = &CubSet::acyclicSubspaceBI;
    CubSet::neighbAcyclicBI = &CubSet::neighbAcyclicLT;

    // creating copy of original cubical set
    CubSet acyclicCubSubset(cubSet());

    // compute acyclic subset
    // acyclic subset is being removed from the original set
    (cubSet().*accSubAlgorithm)(acyclicCubSubset);

    _logger.End();
    _logger.Log(FGLogger::Details)<<"computed acyclic subset size: "<<acyclicCubSubset.cardinality()<<std::endl;
    _logger.Log(FGLogger::Details)<<"cubes left: "<<cubSet().cardinality()<<std::endl;

    _logger.Begin(FGLogger::Details, "computing acyclic subspace intersection with neighbourhood");
    // creating wrap of "non-reduced"
    CubSet wrapped(cubSet());
    wrapped.wrap();
    // important are only these cubes in the acyclic subset, which intersect the difference
    acyclicCubSubset *= wrapped;
    // adding acyclic subspace to original set
    cubSet() += acyclicCubSubset;
    _logger.End();

    _logger.Begin(FGLogger::Details, "constructing CubCellSet of the difference");
    // constructing CubCellSets for each CubSet and computing the difference
    CubCellSetPtr cubCellSet(new CubCellSet(cubSet()));
    CubCellSet acyclicCubCellSubset(acyclicCubSubset);
    cubCellSet() -= acyclicCubCellSubset;
    _logger.End();

    _logger.Begin(FGLogger::Details, "creating kappa-map for quotient space");
    Dims dims;
    KappaMap kappaMap;
    CreateKappaMapFromQuotient(cubCellSet, dims, kappaMap);
    _logger.End();

    _logger.Begin(FGLogger::Details, "creating SComplex from kappa-map");
    _complex = SComplexFactory<InputSComplex>::Create(dims, kappaMap);
    _logger.End();
}

template <typename Traits>
void CollapsedAKQReducedCubSComplexSupplier<Traits>::CreateAlgorithm()
{
    _logger.Begin(FGLogger::Details, "performing coreductions");
    _algorithm = AlgorithmPtr(new Algorithm(new Strategy(*_complex)));
    size_t reducedCount = (*_algorithm)();
    _logger.End("coreductions finished");
    if (_logger.PrintCoreducedCellsCount())
    {
        _logger.Log(FGLogger::Details)<<"number of reduced pairs: "<<reducedCount<<std::endl;
        _logger.Log(FGLogger::Details)<<"number of extracted cells: "<<_algorithm->getExtractedSignature().size()<<std::endl;
    }
}

template <typename Traits>
bool CollapsedAKQReducedCubSComplexSupplier<Traits>::GetCells(CellsByDim& cellsByDim,
                                                         std::map<Id, Chain>& _2Boundaries)
{
    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    int maxDim = static_cast<int>(outputComplex->getDim());
    cellsByDim.resize(maxDim + 1);
    for (int dim = 0; dim <= maxDim; dim++)
    {
        DimCells dimCells = outputComplex->iterators().dimCells(static_cast<Dim>(dim));
        typename DimCells::iterator it = dimCells.begin();
        typename DimCells::iterator itEnd = dimCells.end();
        for ( ; it != itEnd; ++it)
        {
            cellsByDim[dim].insert(it->getId());
        }
    }

    // if there are some 2-cells, take its (homotopic) boundaries
    AKQHomotopicPaths<CollapsedAKQReducedCubSComplexSupplier<Traits> > homotopicPaths(this, _algorithm->getStrategy());
    _2Boundaries.clear();
    if (cellsByDim.size() > 2)
    {
        Cells _2cells = cellsByDim[2];
        typename Cells::iterator it = _2cells.begin();
        typename Cells::iterator itEnd = _2cells.end();
        for ( ; it != itEnd; ++it)
        {
            _2Boundaries[*it] = homotopicPaths.GetHomotopicBoundary(*it);
        //    _2Boundaries[*it] = GetBoundary(*it);
        }
    }
    return cellsByDim.size() > 0;
}

template <typename Traits>
typename CollapsedAKQReducedCubSComplexSupplier<Traits>::Chain
CollapsedAKQReducedCubSComplexSupplier<Traits>::GetBoundary(const Id& cellId)
{
    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    Cell cell = (*outputComplex)[cellId];
    Chain boundary;
    BdCells bdCells = outputComplex->iterators().bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    for ( ; it != itEnd; ++it)
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
CollapsedAKQReducedCubSComplexSupplier<Traits>::GetOrdered2Boundary(ComplexType* complex,
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
    return boundary;
}

template <typename Traits>
void CollapsedAKQReducedCubSComplexSupplier<Traits>::CreateKappaMapFromQuotient(
                                                         CubCellSetPtr cubCellSet,
                                                         Dims& dims,
                                                         KappaMap& kappaMap)
{
    _allCells.clear();
    _cellsMapByDim.clear();

    _nullSetCell = new CellDescriptor(0, 0);
    _allCells.push_back(_nullSetCell);
    _cellsCountByDim[0] = 1;

    size_t maxDim = static_cast<size_t>(cubCellSet().embDim());
    PointCoordIterator it = PointCoordIterator(cubCellSet().begin());
    BitCoordIterator itEnd = cubCellSet().end();
    for ( ; it < itEnd; ++it)
    {
        if (it.ownDim() == maxDim)
        {
            CreateCell(cubCellSet, it, maxDim);
        }
    }

    std::map<size_t, size_t> cellsIndicesOffsets;
    size_t totalCellsCount = 0;
    for (size_t i = 0; i <= maxDim; i++)
    {
        if (i > 0)
        {
            cellsIndicesOffsets[i] = cellsIndicesOffsets[i - 1] + _cellsCountByDim[i - 1];
        }
        totalCellsCount += _cellsCountByDim[i];
        _logger.Log(FGLogger::Debug)<<_cellsCountByDim[i]<<" cells in dim "<<i;
        _logger.Log(FGLogger::Debug)<<" with offset "<<cellsIndicesOffsets[i]<<std::endl;
    }
    _logger.Log(FGLogger::Debug)<<"total cells generated: "<<totalCellsCount<<std::endl;

    dims.resize(totalCellsCount);
    kappaMap.clear();

    typename CellsDescriptors::iterator jt = _allCells.begin();
    typename CellsDescriptors::iterator jtEnd = _allCells.end();
    for ( ; jt != jtEnd; ++jt)
    {
        CellDescriptor* cell = *jt;
        size_t index = cell->_index + cellsIndicesOffsets[cell->_dim];
        size_t faceIndexOffset = cellsIndicesOffsets[cell->_dim - 1];
        dims[index] = cell->_dim;
        std::vector<int>::iterator cIt = cell->_coefficients.begin();
        for (typename CellsDescriptors::iterator fIt = cell->_faces.begin();
                                                 fIt != cell->_faces.end();
                                                 ++fIt)
        {
            CellDescriptor* face = *fIt;
            kappaMap.push_back(KappaMapEntry(static_cast<Id>(index),
                                             static_cast<Id>(face->_index + faceIndexOffset),
                                             *cIt));
            cIt++;
        }
    }

    jt = _allCells.begin();
    for ( ; jt != jtEnd; ++jt)
    {
        delete *jt;
    }
}

template <typename Traits>
typename CollapsedAKQReducedCubSComplexSupplier<Traits>::CellDescriptor*
CollapsedAKQReducedCubSComplexSupplier<Traits>::AddCell(CubCellSetPtr cubCellSet,
                                                        BitCoordIterator& it,
                                                        size_t dim)
{
    CellDescriptor* cell = _cellsMapByDim[dim][it];
    if (cell != 0)
    {
        return cell;
    }
    return CreateCell(cubCellSet, it, dim);
}

template <typename Traits>
typename CollapsedAKQReducedCubSComplexSupplier<Traits>::CellDescriptor*
CollapsedAKQReducedCubSComplexSupplier<Traits>::CreateCell(CubCellSetPtr cubCellSet,
                                                           BitCoordIterator& it,
                                                           size_t dim)
{
    size_t index = _cellsCountByDim[dim]++;
    CellDescriptor* cell = new CellDescriptor(index, dim);
    _allCells.push_back(cell);
    _cellsMapByDim[dim][it] = cell;
    std::vector<BitCoordIterator> faces;
    std::vector<int> coefficients;
    if (dim == 2) // hack to obtain proper order of 2boundary
    {
        cubCellSet().getOrdered2Faces(it, faces, coefficients);
    }
    else
    {
        cubCellSet().getFaces(it, faces, coefficients);
    }
    typename std::vector<BitCoordIterator>::iterator fIt = faces.begin();
    typename std::vector<BitCoordIterator>::iterator fItEnd = faces.end();
    std::vector<int>::iterator cIt = coefficients.begin();
    for ( ; fIt != fItEnd; ++fIt)
    {
        cell->_faces.push_back(AddCell(cubCellSet, *fIt, dim - 1));
        cell->_coefficients.push_back(*cIt);
        cIt++;
    }
    // adding null point cell to each "non complete" 1-cell boundary
    if (dim == 1 && cell->_faces.size() < 2)
    {
        if (cell->_faces.size() == 0)
        {
            //cell->_faces.push_back(_nullSetCell);
            //cell->_faces.push_back(_nullSetCell);
            //cell->_coefficients.push_back(1);
            //cell->_coefficients.push_back(0);
        }
        else if (cell->_coefficients[0] == 1)
        {
            cell->_faces.push_back(_nullSetCell);
            cell->_coefficients.push_back(-1);
        }
        else // if (cell->_coefficients[0] == -1)
        {
            cell->_faces.push_back(cell->_faces[0]);
            cell->_coefficients.push_back(-1);
            cell->_faces[0] = _nullSetCell;
            cell->_coefficients[0] = 1;
        }
    }
    return cell;
}

template <typename Traits>
void CollapsedAKQReducedCubSComplexSupplier<Traits>::PrintDebug()
{
    _logger.Log(FGLogger::Debug)<<"extracted signature:"<<std::endl<<_algorithm->getExtractedSignature()<<std::endl;

    OutputSComplex* outputComplex = _algorithm->getStrategy()->getOutputComplex();
    std::vector<int> betti = HomologyHelpers<Traits>::GetHomologySignature(outputComplex);
    _logger.Log(FGLogger::Debug)<<"homology signature:"<<std::endl;
    for (int i = 0; i < betti.size(); ++i)
    {
        _logger.Log(FGLogger::Debug)<<"H_"<<i<<" = Z^"<<betti[i]<<std::endl;
    }
}

#endif	/* COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_HPP */
