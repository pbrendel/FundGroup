/*
 * File:   AKQReducedSComplexSupplier.h
 * Author: Piotr Brendel
 */

#ifndef AKQREDUCEDSCOMPLEXSUPPLIER_H
#define	AKQREDUCEDSCOMPLEXSUPPLIER_H

#include <map>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <redHom/algorithm/Coreduction.hpp>

#include "DebugComplexType.h"

template <typename Traits>
class AKQReducedSComplexSupplier
{
    typedef typename Traits::InputSComplexType  InputSComplex;
    typedef boost::shared_ptr<InputSComplex>    InputSComplexPtr;
    typedef typename Traits::OutputSComplexType OutputSComplex;
    typedef typename Traits::ScalarType         Scalar;
    typedef typename Traits::IntType            Int;

    typedef typename OutputSComplex::Dim        Dim;
    typedef typename OutputSComplex::ColoredIterators::Iterators::DimCells DimCells;
    typedef typename OutputSComplex::ColoredIterators::Iterators::BdCells  BdCells;

    typedef AKQReduceStrategy<InputSComplex>            Strategy;
    typedef CoreductionAlgorithm<Strategy, Scalar, Int> Algorithm;
    typedef boost::shared_ptr<Algorithm>                AlgorithmPtr;

public:

    typedef Traits                              HomologyTraits;
    typedef typename OutputSComplex::Cell       Cell;
    typedef std::set<Cell>                      Cells;
    typedef std::vector<Cells>                  CellsByDim;
    typedef typename Traits::ChainType          Chain;

    AKQReducedSComplexSupplier(const char* filename);
    AKQReducedSComplexSupplier(DebugComplexType type);
    bool GetCells(CellsByDim& cellsByDim, std::map<Cell, Chain>& _2Boundaries);
    Chain GetBoundary(const Cell& cell);

    void PrintDebug();

private:

    void CreateAlgorithm();

    InputSComplexPtr    _complex;
    AlgorithmPtr        _algorithm;
};

#include "AKQReducedSComplexSupplier.hpp"

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_H */

