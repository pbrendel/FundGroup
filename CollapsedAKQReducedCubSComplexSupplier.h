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

//temp!!!

#include <redHom/complex/cubical/CubSComplex.hpp>

#include "DebugComplexType.h"

template <typename Traits>
class CollapsedAKQReducedCubSComplexSupplier
{
    typedef typename Traits::CoordType          Coord;
    typedef typename Traits::CubSetType         CubSet;
    typedef typename Traits::CubSetPtrType      CubSetPtr;
    typedef typename Traits::CubCellSetType     CubCellSet;
    typedef typename Traits::CubCellSetPtrType  CubCellSetPtr;
    enum
    {
        DIM = Traits::DIM,
    };

    //typedef SComplex                            SComplexType;
    typedef CubSComplex<DIM>                    InputSComplex;
    typedef boost::shared_ptr<InputSComplex>    InputSComplexPtr;
    typedef typename Traits::OutputSComplexType OutputSComplex;
    typedef typename Traits::ScalarType         Scalar;

    typedef typename OutputSComplex::Dim        Dim;
    typedef typename OutputSComplex::ColoredIterators::Iterators::DimCells DimCells;
    typedef typename OutputSComplex::ColoredIterators::Iterators::BdCells  BdCells;

    typedef AKQReduceStrategy<InputSComplex>        Strategy;
    typedef CoreductionAlgorithm<Strategy, Scalar>  Algorithm;
    typedef boost::shared_ptr<Algorithm>            AlgorithmPtr;

public:

    typedef Traits                              HomologyTraits;
    typedef typename OutputSComplex::Cell       Cell;
    typedef std::set<Cell>                      Cells;
    typedef std::vector<Cells>                  CellsByDim;
    typedef typename Traits::ChainType          Chain;

    CollapsedAKQReducedCubSComplexSupplier(const char* filename);
    CollapsedAKQReducedCubSComplexSupplier(DebugComplexType type);
    bool GetCells(CellsByDim& cellsByDim, std::map<Cell, Chain>& _2Boundaries);
    Chain GetBoundary(const Cell& cell);

    void PrintDebug();

private:

    void CreateComplex(CubSetPtr cubSet, bool collapse);
    void CreateAlgorithm();

    InputSComplexPtr    _complex;
    AlgorithmPtr        _algorithm;
};

#include "CollapsedAKQReducedCubSComplexSupplier.hpp"

#endif	/* COLLAPSEDAKQREDUCEDCUBSCOMPLEXSUPPLIER_H */

