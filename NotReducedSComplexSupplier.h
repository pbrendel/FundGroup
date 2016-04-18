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
    typedef typename OutputSComplex::ColoredIterators::DimCells DimCells;
    typedef typename OutputSComplex::ColoredIterators::BdCells  BdCells;

public:

    typedef Traits                              HomologyTraits;
    typedef typename OutputSComplex::Id         Id;
    typedef std::set<Id>                        Cells;
    typedef std::vector<Cells>                  CellsByDim;
    typedef std::vector<std::pair<Id, int> >    Chain;

    NotReducedSComplexSupplier(const char* filename);
    NotReducedSComplexSupplier(DebugComplexType type);
    NotReducedSComplexSupplier(InputSComplexPtr inputSComplex);
    
    bool GetCells(CellsByDim& cellsByDim, std::map<Id, Chain>& _2Boundaries);
    Chain GetBoundary(const Id& cellId);

    template <typename ComplexType>
    std::list<std::pair<typename ComplexType::Id, int> >
    GetOrdered2Boundary(ComplexType* complex, const typename ComplexType::Id& cellId);

    void PrintDebug();

private:

    InputSComplexPtr    _complex;
};

#include "NotReducedSComplexSupplier.hpp"

#endif	/* NOTREDUCEDSCOMPLEXSUPPLIER_H */
