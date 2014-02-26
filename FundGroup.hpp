/*
 * File:   FundGroup.hpp
 * Author: Piotr Brendel
 */

#ifndef FUNDGROUP_HPP
#define	FUNDGROUP_HPP

#include "FundGroup.h"

#include <cassert>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>

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
    _logger.Begin(FGLogger::Details, "getting cells data");
    _complexSupplier->GetCells(_cellsByDim, _2Boundaries);
    _logger.End();
    if (_cellsByDim[0].size() > 1)
    {
        _logger.Begin(FGLogger::Details, "creating spanning tree");
        CreateSpanningTree();
        _logger.End();
    }
    _logger.Begin(FGLogger::Details, "computing relators");
    ComputeRelators();
    //SimplifyRelators();
    _logger.End();
    PrintDebug();
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::CreateSpanningTree()
{
    std::list<Cell> _1cells(_cellsByDim[1].begin(), _cellsByDim[1].end());
    _cellsByDim[1].clear();
    Cells verts;
    Cells &edges = _spanningTreeEdges;
    verts.insert(*_cellsByDim[0].begin());

    bool found = true;
    while (found)
    {
        found = false;
        typename std::list<Cell>::iterator it = _1cells.begin();
        typename std::list<Cell>::iterator itEnd = _1cells.end();
        while (it != itEnd)
        {
            Chain boundary = _complexSupplier->GetBoundary(*it);
            if (boundary.size() == 2)
            {
                typename Chain::iterator cit = boundary.begin();
                Cell v0 = cit->first;
                cit++;
                Cell v1 = cit->first;
                bool v0Found = verts.find(v0) != verts.end();
                bool v1Found = verts.find(v1) != verts.end();
                if (v0Found && !v1Found)
                {
                    verts.insert(v1);
                    edges.insert(*it);
                    it = _1cells.erase(it);
                    found = true;
                }
                else if (!v0Found && v1Found)
                {
                    verts.insert(v0);
                    edges.insert(*it);
                    it = _1cells.erase(it);
                    found = true;
                }
                else if (v0Found && v1Found)
                {
                    _cellsByDim[1].insert(*it);
                    it = _1cells.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                _logger.Log(FGLogger::Assert)<<"boundary.size =  "<<boundary.size()<<std::endl;
                assert(false);
            }
        }
    }
    assert(verts.size() == _cellsByDim[0].size());

    _cellsByDim[1].insert(_1cells.begin(), _1cells.end());

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
            if (edges.find(jt->first) == edges.end())
            {
                r.push_back(RelatorComponent(jt->first, jt->second));
            }
        }
        if (r.size() > 0)
        {
            _relators.push_back(r);
        }
    }
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::SimplifyRelators()
{
    std::set<Cell>& generators = _cellsByDim[1];
    size_t relatorsCount = _relators.size();
    std::vector<bool> unusedRelators(relatorsCount);
    bool reduced = true;
    while (reduced)
    {
        reduced = false;
        for (size_t i = 0; i < relatorsCount; i++)
        {
            if (unusedRelators[i])
            {
                continue;
            }
            Relator r = _relators[i];
            size_t reducedCount = 0;
            typename Relator::iterator generatorToReduce;
            for (typename Relator::iterator it = r.begin(); it != r.end(); ++it)
            {
                if (generators.find(it->first) == generators.end())
                {
                    reducedCount++;
                }
                else
                {
                    generatorToReduce = it;
                }
            }
            if (reducedCount >= r.size() - 1)
            {
                if (reducedCount == r.size() - 1)
                {
                    generators.erase(generatorToReduce->first);
                }
                unusedRelators[i] = true;
                reduced = true;
            }
        }
        _logger.Log(FGLogger::Debug)<<"generators left: "<<generators.size()<<std::endl;
    }
    Relators newRelators;
    for (size_t i = 0; i < relatorsCount; i++)
    {
        if (unusedRelators[i])
        {
            continue;
        }
        Relator r;
        for (typename Relator::iterator it = _relators[i].begin();
                                        it != _relators[i].end();
                                        ++it)
        {
            if (generators.find(it->first) != generators.end())
            {
                r.push_back(*it);
            }
        }
        assert(r.size() > 1);
        newRelators.push_back(r);
    }
    _relators.assign(newRelators.begin(), newRelators.end());
}

template <typename ComplexSupplierType>
std::string FundGroup<ComplexSupplierType>::ToString()
{
    std::ostringstream str;
    if (_cellsByDim.size() < 2 || _cellsByDim[1].size() == 0)
    {
        return "Trivial group";
    }

    std::map<Cell, int> symbols;

    {
        str<<"Generators: [";
        Cells &_1cells = _cellsByDim[1];
        int c = 1;
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
                str<<"f"<<symbols[jt->first];
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
void FundGroup<ComplexSupplierType>::ExportHapProgram(const char* filename)
{
    std::ofstream output(filename);
    if (!output.is_open())
    {
        return;
    }

    std::map<Cell, int> symbols;
    {
        Cells &_1cells = _cellsByDim[1];
        int c = 1;
        int index = 0;
        typename Cells::iterator it = _1cells.begin();
        typename Cells::iterator itEnd = _1cells.end();
        for ( ; it != itEnd; ++it)
        {
            symbols[*it] = c++;
        }
    }

    {
        output<<"F:=FreeGroup("<<_cellsByDim[1].size()<<");"<<std::endl;
        output<<"g:=GeneratorsOfGroup(F);"<<std::endl;
        output<<"rels:=[];"<<std::endl;

        typename Relators::iterator it = _relators.begin();
        typename Relators::iterator itEnd = _relators.end();
        for ( ; it != itEnd; ++it)
        {
            Relator r = *it;
            int index = 0;
            typename Relator::iterator jt = r.begin();
            typename Relator::iterator jtEnd = r.end();
            output<<"w:=";
            for ( ; jt != jtEnd; ++jt)
            {
                output<<"g["<<symbols[jt->first]<<"]";
                if (jt->second != 1)
                {
                    output<<"^"<<jt->second;
                }
                if (++index < r.size())
                {
                    output<<"*";
                }
            }
            output<<"; Add(rels,w);"<<std::endl;
        }

        output<<"G:=PresentationFpGroup(F/rels);"<<std::endl;
        output<<"SimplifyPresentation(G);"<<std::endl;
        output<<"P:=FpGroupPresentation(G);"<<std::endl;
        output<<"R:=RelatorsOfFpGroup(P);"<<std::endl;
        output<<"L:=LowIndexSubgroupsFpGroup(P,10);;"<<std::endl;
        output<<"K:=List(L,AbelianInvariants);;"<<std::endl;
        output<<"Sort(K);"<<std::endl;
    }

    output.close();
}

template <typename ComplexSupplierType>
void FundGroup<ComplexSupplierType>::PrintDebug()
{
    _complexSupplier->PrintDebug();

    _logger.Log(FGLogger::Debug)<<"cells:"<<std::endl;
    for (int i = 0; i < _cellsByDim.size(); i++)
    {
        _logger.Log(FGLogger::Debug)<<"dim: "<<i<<std::endl;
        for (typename Cells::iterator it = _cellsByDim[i].begin(); it != _cellsByDim[i].end(); ++it)
        {
            _logger.Log(FGLogger::Debug)<<*it<<std::endl;
        }
    }

    _logger.Log(FGLogger::Debug)<<"homotopic 2 boundaries:"<<std::endl;
    for (typename std::map<Cell, Chain>::iterator it = _2Boundaries.begin();
                                                  it != _2Boundaries.end();
                                                  ++it)
    {
        _logger.Log(FGLogger::Debug)<<"cell: "<<it->first<<std::endl;
        for (typename Chain::iterator jt = it->second.begin(); jt != it->second.end(); ++jt)
        {
            _logger.Log(FGLogger::Debug)<<jt->first<<" "<<jt->second<<std::endl;
        }
    }
}

#endif	/* FUNDGROUP_HPP */

