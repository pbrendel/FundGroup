/*
 * File:   FundGroup.hpp
 * Author: Piotr Brendel
 */

#ifndef FUNDGROUP_HPP
#define	FUNDGROUP_HPP

#include "FundGroup.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

#include "HomologyHelpers.hpp"
#include "SComplexFactory.hpp"

template <typename Traits>
FundGroup<Traits>::FundGroup()
{
    _complex = SComplexFactory<SComplex>::Create();
    Compute();

}

template <typename Traits>
FundGroup<Traits>::FundGroup(const char *filename)
{
    _complex = SComplexFactory<SComplex>::Load(filename);
    Compute();
}

template <typename Traits>
FundGroup<Traits>::FundGroup(FundGroup::SComplexPtr complex) : _complex(complex)
{
    Compute();
}

template <typename Traits>
void FundGroup<Traits>::Compute()
{
    _filter = HomologyHelpers<Traits>::CreateFilter(_complex, false, false, false);
    GetCellsData();
    if (_cellsByDim[0].size() > 1)
    {
        CreateSpanningTree();
    }
    ComputeRelators();
    PrintDebug();
}

template <typename Traits>
void FundGroup<Traits>::GetCellsData()
{
    // critical cells
    _cellsByDim.resize(_filter->topDim() + 1);
    for (int dim = 0; dim <= _filter->topDim(); dim++)
    {
        std::vector<Chain> chains = _filter->baseHomologyChains()[dim];
        for (int i = 0; i < chains.size(); i++)
        {
            for (typename Chain::iterator it = chains[i].begin(); it != chains[i].end(); ++it)
            {
                Cell cell = it->first;
                _cellsByDim[dim].insert(cell);
            }
        }
    }

    // if there are some 2-cells, take its boundaries
    if (_cellsByDim.size() > 2)
    {
        Cells _2cells = _cellsByDim[2];
        for (typename Cells::iterator it = _2cells.begin(); it != _2cells.end(); ++it)
        {
            Cell cell = *it;
            Chain boundary = _filter->getBoundary(cell);
            _filter->reduce(boundary);
            _reduced2Boundaries[cell] = boundary;
        }
    }
}

template <typename Traits>
void FundGroup<Traits>::CreateSpanningTree()
{
    Cells _1cells = _cellsByDim[1];
    Cells verts;
    Cells &edges = _spanningTreeEdges;
    verts.insert(*_cellsByDim[0].begin());

    bool found = true;
    while (found)
    {
        found = false;
        for (typename Cells::iterator it = _1cells.begin(); it != _1cells.end(); ++it)
        {
            Chain boundary = _filter->getBoundary(*it);
            _filter->reduce(boundary);
            if (boundary.size() == 2)
            {
                typename Chain::iterator cit = boundary.begin();
                Cell v0 = cit->first;
                cit++;
                Cell v1 = cit->first;
                if (std::find(verts.begin(), verts.end(), v0) != verts.end() &&
                    std::find(verts.begin(), verts.end(), v1) == verts.end())
                {
                    verts.insert(v1);
                    edges.insert(*it);
                    found = true;
                }
                else if (std::find(verts.begin(), verts.end(), v0) == verts.end() &&
                         std::find(verts.begin(), verts.end(), v1) != verts.end())
                {
                    verts.insert(v0);
                    edges.insert(*it);
                    found = true;
                }
            }
            else
            {
                assert(false);
            }
        }
    }
    assert(verts.size() == _cellsByDim[0].size());

    // removing used tree edges from 1-cells
    // corresponds to quoting group generated by 1-cells
    // by group generated by edges of spanning tree (relators)
    for (typename Cells::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        _cellsByDim[1].erase(*it);
    }

    // TODO!
    // if we assume that space may be disconnected
    // we need to remove "unused" cells
}

template <typename Traits>
void FundGroup<Traits>::ComputeRelators()
{
    for (typename std::map<Cell, Chain>::iterator it = _reduced2Boundaries.begin();
                                                  it != _reduced2Boundaries.end();
                                                  ++it)
    {
        Cells &edges = _spanningTreeEdges;
        Relator r;
        Chain boundary = it->second;
        for (typename Chain::iterator jt = boundary.begin(); jt != boundary.end(); ++jt)
        {
            // if edge is not contained in the spanning tree
            // add it as relator with proper sign
            // TODO:
            // if we assume that space may be disconnected then
            // we need to check if 2-boundary is in current connected component
            // (for example by checking if a vertex is contained in spanning tree)
            if (std::find(edges.begin(), edges.end(), jt->first) == edges.end())
            {
                r.push_back(RelatorComponent(jt->first, jt->second));
            }
        }
        _relators.push_back(r);
    }
}

template <typename Traits>
std::string FundGroup<Traits>::ToString()
{
    std::ostringstream str;
    if (_cellsByDim.size() < 2 || _cellsByDim[1].size() == 0)
    {
        return "Trivial group";
    }
    str<<"Generators: [";
    Cells &_1cells = _cellsByDim[1];
    std::map<Cell, char> symbols;
    char c = 'a';
    int index = 0;
    for (typename Cells::iterator it = _1cells.begin(); it != _1cells.end(); it++)
    {
        symbols[*it] = c;
        str<<c++;
        if (++index < _1cells.size())
        {
            str<<", ";
        }
    }
    str<<"]"<<std::endl;;
    str<<"Relators:"<<std::endl;
    for (typename Relators::iterator it = _relators.begin(); it != _relators.end(); ++it)
    {
        Relator r = *it;
        index = 0;
        for (typename Relator::iterator jt = r.begin(); jt != r.end(); ++jt)
        {
            str<<symbols[jt->first];
            if (jt->second != 1)
            {
                str<<"^"<<jt->second;
            }
            if (++index < r.size())
            {
                str<<"*";
            }
        }
        str<<std::endl;
    }
    return str.str();
}

template <typename Traits>
void FundGroup<Traits>::PrintDebug()
{
    typedef HomologyHelpers<Traits> Homology;

    std::vector<int> betti = Homology::GetBettiNumbers(_filter);
    std::cout<<"betti numbers of given complex:"<<std::endl;
    for (int i = 0; i < betti.size(); i++)
    {
        std::cout<<"B["<<i<<"] = "<<betti[i]<<std::endl;
    }

    std::cout<<"homology chains:"<<std::endl;
    for (int i = 0; i <= _filter->topDim(); i++)
    {
        std::cout<<"dim "<<i<<":"<<std::endl;
        for ( int j = 0; j < _filter->baseHomologyChains()[i].size(); j++)
        {
            std::cout<<_filter->baseHomologyChains()[i][j]<<", "<<std::endl;
        }
        std::cout<<std::endl;
    }

    std::cout<<"cells:"<<std::endl;
    for (int i = 0; i < _cellsByDim.size(); i++)
    {
        std::cout<<"dim: "<<i<<std::endl;
        for (typename Cells::iterator it = _cellsByDim[i].begin(); it != _cellsByDim[i].end(); ++it)
        {
            std::cout<<*it<<std::endl;
        }
    }

    std::cout<<"2 boundaries:"<<std::endl;
    for (typename std::map<Cell, Chain>::iterator it = _reduced2Boundaries.begin();
                                                  it != _reduced2Boundaries.end();
                                                  ++it)
    {
        std::cout<<"cell: "<<it->first<<std::endl;
        for (typename Chain::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        {
            std::cout<<jt->first<<std::endl;
        }
    }
}

#endif	/* FUNDGROUP_HPP */

