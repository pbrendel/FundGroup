/*
 * File:   NotReducedSComplexSupplier.h
 * Author: Piotr Brendel
 */

#ifndef NOTREDUCEDSCOMPLEXSUPPLIER_H
#define	NOTREDUCEDSCOMPLEXSUPPLIER_H

#include <map>
#include <boost/shared_ptr.hpp>

#include "DebugComplexType.h"

template <typename Traits>
class NotReducedSComplexSupplier
{
public:

    typedef typename Traits::SComplexType       InputSComplex;
    typedef typename Traits::SComplexType       OutputSComplex;

private:

    typedef boost::shared_ptr<InputSComplex>    InputSComplexPtr;
    typedef typename Traits::ScalarType         Scalar;
    typedef typename Traits::IntType            Int;

    typedef typename OutputSComplex::Dim        Dim;
    typedef typename OutputSComplex::ColoredIterators::Iterators::DimCells DimCells;
    typedef typename OutputSComplex::ColoredIterators::Iterators::BdCells  BdCells;

public:

    typedef Traits                              HomologyTraits;
    typedef typename OutputSComplex::Id         Id;
    typedef std::set<Id>                        Cells;
    typedef std::vector<Cells>                  CellsByDim;
    typedef std::vector<std::pair<Id, int> >  Chain;

    NotReducedSComplexSupplier(const char* filename);
    NotReducedSComplexSupplier(InputSComplexPtr inputSComplex);
    NotReducedSComplexSupplier(DebugComplexType type);
    bool GetCells(CellsByDim& cellsByDim, std::map<Id, Chain>& _2Boundaries);
    Chain GetBoundary(const Id& cellId);

    void PrintDebug();

private:

    InputSComplexPtr    _complex;
};

#include "NotReducedSComplexSupplier.hpp"

#endif	/* NOTREDUCEDSCOMPLEXSUPPLIER_H */
