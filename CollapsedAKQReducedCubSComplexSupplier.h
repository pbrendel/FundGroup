/*
 * File:   CollapsedAKQReducedCubSComplexSupplier.h
 * Author: Piotr Brendel
 */

#ifndef COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_H
#define	COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_H

#include <map>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <redHom/algorithm/Coreduction.hpp>
#include <redHom/complex/scomplex/SComplex.hpp>

#include "DebugComplexType.h"

template <typename Traits>
class CollapsedAKQReducedCubSComplexSupplier
{
public:

    typedef typename Traits::GeneralSComplexType    InputSComplex;
    typedef typename Traits::GeneralSComplexType    OutputSComplex;

private:

    typedef typename Traits::CubCoordType       Coord;
    typedef typename Traits::CubSetType         CubSet;
    typedef typename Traits::CubSetPtrType      CubSetPtr;
    typedef typename Traits::CubCellSetType     CubCellSet;
    typedef typename Traits::CubCellSetPtrType  CubCellSetPtr;
    enum
    {
        DIM = Traits::DIM,
    };

    // OutputSComplexType is a SComplex parametrized with DefaultTraits
    typedef boost::shared_ptr<InputSComplex>        InputSComplexPtr;
    typedef typename Traits::ScalarType             Scalar;
    typedef typename Traits::IntType                Int;

    typedef typename InputSComplex::Id              Id;
    typedef typename InputSComplex::Dim             Dim;
    typedef typename InputSComplex::Dims            Dims;
    typedef typename InputSComplex::KappaMap        KappaMap;
    typedef typename InputSComplex::KappaMapEntry   KappaMapEntry;

    typedef typename OutputSComplex::Iterators::DimCells DimCells;
    typedef typename OutputSComplex::Iterators::BdCells  BdCells;

    typedef AKQReduceStrategy<InputSComplex>            Strategy;
    typedef CoreductionAlgorithm<Strategy, Scalar, Int> Algorithm;
    typedef boost::shared_ptr<Algorithm>                AlgorithmPtr;

public:

    typedef Traits                              HomologyTraits;
    typedef typename OutputSComplex::Cell       Cell;
    typedef std::set<Cell>                      Cells;
    typedef std::vector<Cells>                  CellsByDim;
    typedef std::vector<std::pair<Cell, int> >  Chain;

    CollapsedAKQReducedCubSComplexSupplier(const char* filename);
    CollapsedAKQReducedCubSComplexSupplier(DebugComplexType type);
    bool GetCells(CellsByDim& cellsByDim, std::map<Cell, Chain>& _2Boundaries);
    Chain GetBoundary(const Cell& cell);

    void PrintDebug();

private:

    void CreateComplex(CubSetPtr cubSet);
    void CreateAlgorithm();
    Chain GetOriginalHomotopicBoundary(const Cell& cell);

    InputSComplexPtr    _complex;
    AlgorithmPtr        _algorithm;

    typedef typename CubCellSet::BitCoordIterator   BitCoordIterator;
    typedef typename CubCellSet::PointCoordIterator PointCoordIterator;
    struct CellDescriptor
    {
        size_t                          _index;
        size_t                          _dim;
        std::vector<CellDescriptor*>    _faces;
        std::vector<int>                _coefficients;

        CellDescriptor(size_t index, size_t dim)
            : _index(index)
            , _dim(dim)
        {}
    };

    typedef std::vector<CellDescriptor*>                CellsDescriptors;
    typedef std::map<BitCoordIterator, CellDescriptor*> CellsMap;
    typedef std::map<size_t, CellsMap>                  CellsMapByDim;

    CellDescriptor                                      *_nullSetCell;
    CellsDescriptors                                    _allCells;
    CellsMapByDim                                       _cellsMapByDim;
    std::map<size_t, size_t>                            _cellsCountByDim;

    void CreateKappaMapFromQuotient(CubCellSetPtr cubCellSet, Dims& dims, KappaMap& kappaMap);
    CellDescriptor* AddCell(CubCellSetPtr cubCellSet, BitCoordIterator& it, size_t dim);
    CellDescriptor* CreateCell(CubCellSetPtr cubCellSet, BitCoordIterator& it, size_t dim);
};

#include "CollapsedAKQReducedCubSComplexSupplier.hpp"

#endif	/* COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_H */

