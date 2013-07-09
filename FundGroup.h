/*
 * File:   FundGroup.h
 * Author: Piotr Brendel
 */

#ifndef FUNDGROUP_H
#define	FUNDGROUP_H

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

template <typename Traits>
class FundGroup
{
public:

    typedef typename Traits::SComplexType   SComplex;
    typedef boost::shared_ptr<SComplex>     SComplexPtr;

    FundGroup();
    FundGroup(SComplexPtr complex);

    friend std::ostream& operator<<(std::ostream &str, FundGroup g)
    {
        str<<g.ToString();
        return str;
    }

private:

    typedef typename Traits::FilterType     Filter;
    typedef boost::shared_ptr<Filter>       FilterPtr;
    typedef typename Traits::CellType       Cell;
    typedef std::set<Cell>                  Cells;
    typedef std::vector<Cells>              CellsByDim;
    typedef typename Traits::ChainType      Chain;
    //typedef std::pair<Cell, Cell>           ReductionPair;
    typedef std::pair<Cell, int>            RelatorComponent;
    typedef std::vector<RelatorComponent>   Relator;
    typedef std::vector<Relator>            Relators;

    SComplexPtr             _complex;
    FilterPtr               _filter;
    CellsByDim              _criticalCellsByDim;
    std::map<Cell, Chain>   _reduced2Boundaries;
    Cells                   _spanningTreeEdges;
    Relators                _relators;

    void Compute();
    void GetCellsData();
    void CreateSpanningTree();
    void ComputeRelators();
    std::string ToString();

    void PrintDebug();

};

#endif	/* FUNDGROUP_H */

