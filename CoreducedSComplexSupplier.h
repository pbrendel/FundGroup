/*
 * File:   CoreducedSComplexSupplier.h
 * Author: Piotr Brendel
 */

#ifndef COREDUCEDSCOMPLEXSUPPLIER_H
#define	COREDUCEDSCOMPLEXSUPPLIER_H

#include <map>
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "DebugComplexType.h"

template <typename Traits>
class CoreducedSComplexSupplier
{
public:

    typedef Traits                          HomologyTraits;
    typedef typename Traits::SComplexType   SComplex;
    typedef typename Traits::CellType       Cell;
    typedef std::set<Cell>                  Cells;
    typedef std::vector<Cells>              CellsByDim;
    typedef typename Traits::ChainType      Chain;

    CoreducedSComplexSupplier(const char* filename);
    CoreducedSComplexSupplier(DebugComplexType debugComplexType);
    bool GetCells(CellsByDim& cellsByDim, std::map<Cell, Chain>& _2Boundaries);
    Chain GetBoundary(const Cell& cell);

    void PrintDebug();

private:

    void CreateFilter();

    typedef boost::shared_ptr<SComplex>     SComplexPtr;
    typedef typename Traits::FilterType     Filter;
    typedef boost::shared_ptr<Filter>       FilterPtr;

    SComplexPtr _complex;
    FilterPtr   _filter;
};

#include "CoreducedSComplexSupplier.hpp"

#endif	/* COREDUCEDSCOMPLEXSUPPLIER_H */

