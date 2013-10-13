/*
 * File:   FundGroup.hpp
 * Author: Piotr Brendel
 */

#ifndef FUNDGROUP_HPP
#define	FUNDGROUP_HPP

#include "FundGroup.h"

#include <cassert>
#include <sstream>
#include <vector>

#include "SComplexFactory.hpp"

#include "Logger.h"

template <typename ComplexSupplierType>
FundGroup<ComplexSupplierType>::FundGroup(const char *filename)
{
    _complexSupplier = ComplexSupplierPtr(new ComplexSupplier(filename));
    Compute();
}

template <typename ComplexSupplierType>
FundGroup<ComplexSupplierType>::FundGroup(DebugComplexType debugComplexType)
{
    _complexSupplier = ComplexSupplierPtr(new ComplexSupplier(debugComplexType));
    Compute();
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::Compute()
{
    Logger::Begin(Logger::Details, "getting cells data");
    _complexSupplier->GetCells(_cellsByDim, _2Boundaries);
    Logger::End();
    if (_cellsByDim[0].size() > 1)
    {
        Logger::Begin(Logger::Details, "creating spanning tree");
        CreateSpanningTree();
        Logger::End();
    }
    Logger::Begin(Logger::Details, "computing relators");
    ComputeRelators();
    Logger::End();
    PrintDebug();
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::CreateSpanningTree()
{
    Cells _1cells = _cellsByDim[1];
    Cells verts;
    Cells &edges = _spanningTreeEdges;
    verts.insert(*_cellsByDim[0].begin());

    bool found = true;
    while (found)
    {
        found = false;
        typename Cells::iterator it = _1cells.begin();
        typename Cells::iterator itEnd = _1cells.end();
        for ( ; it != itEnd; ++it)
        {
            Chain boundary = _complexSupplier->GetBoundary(*it);
            if (boundary.size() == 2)
            {
                typename Chain::iterator cit = boundary.begin();
                Cell v0 = cit->first;
                cit++;
                Cell v1 = cit->first;
                bool v0Found = std::find(verts.begin(), verts.end(), v0) != verts.end();
                bool v1Found = std::find(verts.begin(), verts.end(), v1) != verts.end();
                if (v0Found && !v1Found)
                {
                    verts.insert(v1);
                    edges.insert(*it);
                    found = true;
                }
                else if (!v0Found && v1Found)
                {
                    verts.insert(v0);
                    edges.insert(*it);
                    found = true;
                }
            }
            else
            {
                Logger::Log(Logger::Assert)<<"boundary.size =  "<<boundary.size()<<std::endl;
                assert(false);
            }
        }
    }
    assert(verts.size() == _cellsByDim[0].size());

    // removing used tree edges from 1-cells
    // corresponds to quoting group generated by 1-cells
    // by group generated by edges of spanning tree (relators)
    _cellsByDim[1].erase(edges.begin(), edges.end());

    // TODO!
    // if we assume that space may be disconnected
    // we need to remove "unused" cells
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::ComputeRelators()
{
    typename std::map<Cell, Chain>::iterator it = _2Boundaries.begin();
    typename std::map<Cell, Chain>::iterator itEnd = _2Boundaries.end();
    for ( ; it != itEnd; ++it)
    {
        Cells &edges = _spanningTreeEdges;
        Relator r;
        Chain boundary = it->second;
        typename Chain::iterator jt = boundary.begin();
        typename Chain::iterator jtEnd = boundary.end();
        for ( ; jt != jtEnd; ++jt)
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

template <typename ComplexSupplierType>
std::string FundGroup<ComplexSupplierType>::ToString()
{
    std::ostringstream str;
    if (_cellsByDim.size() < 2 || _cellsByDim[1].size() == 0)
    {
        return "Trivial group";
    }

    std::map<Cell, char> symbols;

    {
        str<<"Generators: [";
        Cells &_1cells = _cellsByDim[1];
        char c = 'a';
        int index = 0;
        typename Cells::iterator it = _1cells.begin();
        typename Cells::iterator itEnd = _1cells.end();
        for ( ; it != itEnd; ++it)
        {
            symbols[*it] = c;
            str<<c++;
            if (++index < _1cells.size())
            {
                str<<", ";
            }
        }
        str<<"]"<<std::endl;;
    }

    {
        str<<"Relators:"<<std::endl;
        typename Relators::iterator it = _relators.begin();
        typename Relators::iterator itEnd = _relators.end();
        for ( ; it != itEnd; ++it)
        {
            Relator r = *it;
            int index = 0;
            typename Relator::iterator jt = r.begin();
            typename Relator::iterator jtEnd = r.end();
            for ( ; jt != jtEnd; ++jt)
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
    }

    return str.str();
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::PrintDebug()
{
    _complexSupplier->PrintDebug();

    Logger::Log(Logger::Debug)<<"cells:"<<std::endl;
    for (int i = 0; i < _cellsByDim.size(); i++)
    {
        Logger::Log(Logger::Debug)<<"dim: "<<i<<std::endl;
        for (typename Cells::iterator it = _cellsByDim[i].begin(); it != _cellsByDim[i].end(); ++it)
        {
            Logger::Log(Logger::Debug)<<*it<<std::endl;
        }
    }

    Logger::Log(Logger::Debug)<<"2 boundaries:"<<std::endl;
    for (typename std::map<Cell, Chain>::iterator it = _2Boundaries.begin();
                                                  it != _2Boundaries.end();
                                                  ++it)
    {
        Logger::Log(Logger::Debug)<<"cell: "<<it->first<<std::endl;
        for (typename Chain::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        {
            Logger::Log(Logger::Debug)<<jt->first<<std::endl;
        }
    }
}

#endif	/* FUNDGROUP_HPP */

