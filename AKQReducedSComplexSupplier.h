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
#include <capd/complex/Coreduction.h>
#include <capd/complex/AKQStrategy.hpp>

#include "DebugComplexType.h"
#include "FGLogger.h"

template <typename Traits>
class AKQReducedSComplexSupplier
{
public:

    typedef typename Traits::SComplexType           InputSComplex;
    typedef typename Traits::GeneralSComplexType    OutputSComplex;
    typedef typename Traits::ScalarType             Scalar;

private:

    typedef boost::shared_ptr<InputSComplex>    InputSComplexPtr;
    typedef typename Traits::IntType            Int;

    typedef typename OutputSComplex::Dim        Dim;
    typedef typename OutputSComplex::ColoredIterators::DimCells DimCells;
    typedef typename OutputSComplex::ColoredIterators::BdCells  BdCells;

    typedef capd::complex::AKQReduceStrategy<InputSComplex, OutputSComplex, Scalar> Strategy;
    typedef capd::complex::Coreduction<Strategy, Scalar, Int> Algorithm;
    typedef boost::shared_ptr<Algorithm>        AlgorithmPtr;

public:

    typedef Traits                              HomologyTraits;
    typedef typename OutputSComplex::Cell       Cell;
    typedef typename OutputSComplex::Id         Id;
    typedef std::set<Id>                        Cells;
    typedef std::vector<Cells>                  CellsByDim;
    typedef std::vector<std::pair<Id, int> >    Chain;

    AKQReducedSComplexSupplier(const char* filename);
    AKQReducedSComplexSupplier(DebugComplexType type);
    AKQReducedSComplexSupplier(InputSComplexPtr inputSComplex);

    bool GetCells(CellsByDim& cellsByDim, std::map<Id, Chain>& _2Boundaries);
    Chain GetBoundary(const Id& cellId);

    template <typename ComplexType>
    std::list<std::pair<typename ComplexType::Id, int> >
    GetOrdered2Boundary(ComplexType* complex, const typename ComplexType::Id& cellId);

    void PrintDebug();

private:

    void CreateAlgorithm();

    InputSComplexPtr    _complex;
    AlgorithmPtr        _algorithm;
    FGLogger            _logger;
};

#include "AKQReducedSComplexSupplier.hpp"

#endif	/* AKQREDUCEDSCOMPLEXSUPPLIER_H */
