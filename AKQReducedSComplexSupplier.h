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
#include <redHom/algorithm/COAKQ.hpp>

#include "DebugComplexType.h"

template <typename Traits>
class AKQReducedSComplexSupplier
{
public:

    typedef Traits                          HomologyTraits;
    typedef typename Traits::SComplexType   SComplex;
    typedef typename Traits::CellType       Cell;
    typedef std::set<Cell>                  Cells;
    typedef std::vector<Cells>              CellsByDim;
    typedef typename Traits::ChainType      Chain;
    typedef typename Traits::ScalarType     Scalar;

    AKQReducedSComplexSupplier(const char* filename);
    AKQReducedSComplexSupplier(DebugComplexType debugComplexType);
    bool GetCells(CellsByDim& cellsByDim, std::map<Cell, Chain>& _2Boundaries);
    Chain GetBoundary(const Cell& cell);

    void PrintDebug();

private:

    void CreateAlgorithm();

    typedef boost::shared_ptr<SComplex>     SComplexPtr;
    typedef typename SComplex::Dim          Dim;
    typedef typename SComplex::ColoredIterators::Iterators::DimCells DimCells;
    typedef typename SComplex::ColoredIterators::Iterators::BdCells  BdCells;

    typedef COAKQStrategy<SComplex, SComplex>   Strategy;
    typedef COAKQAlgorithm<Strategy>            Algorithm;
    typedef boost::shared_ptr<Algorithm>        AlgorithmPtr;

    SComplexPtr     _complex;
    AlgorithmPtr    _algorithm;
};

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_H */

