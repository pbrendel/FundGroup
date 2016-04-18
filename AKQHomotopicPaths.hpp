/*
 * File:   AKQHomotopicPaths.hpp
 * Author: Piotr Brendel
 */

#ifndef AKQHOMOTOPICPATHS_HPP
#define	AKQHOMOTOPICPATHS_HPP

#include "AKQHomotopicPaths.h"

template <typename Supplier>
AKQHomotopicPaths<Supplier>::AKQHomotopicPaths(Supplier* complexSupplier,
                                               Strategy* strategy)
    : _complexSupplier(complexSupplier)
    , _strategy(strategy)
{
    assert(_complexSupplier != 0);
    assert(_strategy != 0);
    _originalComplex = &_strategy->getComplex();
    _outputComplex = _strategy->getOutputComplex();
    ComputeAcesMap();
}

template <typename Supplier>
void AKQHomotopicPaths<Supplier>::ComputeAcesMap()
{
    OutputCellId id = OutputCellId(0);
    BOOST_FOREACH(InputCellId ace, _strategy->aces)
    {
        _acesMap.insert(typename AcesMap::value_type(ace, id++));
    }
}

template <typename Supplier>
typename AKQHomotopicPaths<Supplier>::OutputChain
AKQHomotopicPaths<Supplier>::GetHomotopicBoundary(const OutputCellId& cellId)
{
    // cell needs to be an ace
    assert(_acesMap.right.find(cellId) != _acesMap.right.end());
    // we get an original cell
    InputCell originalCell = (*_originalComplex)[_acesMap.right.at(cellId)];
    assert(_strategy->akq[originalCell.getId()] == Strategy::ACE);

    // we take its boundary cells and create a homotopic path
    Path bdPath = _complexSupplier->GetOrdered2Boundary(_originalComplex, originalCell.getId());
    Path homotopicPath;
    for (typename Path::iterator jt = bdPath.begin(); jt != bdPath.end(); ++jt)
    {
        Path tmpPath;
        GetHomotopicPath(*jt, tmpPath);
        homotopicPath.insert(homotopicPath.end(), tmpPath.begin(), tmpPath.end());
    }

    // finally we need to map original complex cells' ids to the output complex cells' ids
    OutputChain boundary;
    for (typename Path::iterator jt = homotopicPath.begin(); jt != homotopicPath.end(); ++jt)
    {
        // only ACES can be present in the final homotopic path
        assert(_strategy->akq[jt->first] == Strategy::ACE);
        assert(_acesMap.left.find(jt->first) != _acesMap.left.end());
        OutputCell ace = (*_outputComplex)[_acesMap.left.at(jt->first)];
        int ci = jt->second;//_outputComplex->coincidenceIndex(cell, ace);
        assert(ci != 0);
        boundary.push_back(std::pair<OutputCellId, int>(ace.getId(), ci));
    }
    return boundary;
}

template <typename Supplier>
void AKQHomotopicPaths<Supplier>::GetHomotopicPath(const Path& path, Path& outPath)
{
    outPath.clear();
    typename Path::const_iterator it = path.begin();
    typename Path::const_iterator itEnd = path.end();
    for ( ; it != itEnd; it++)
    {
        Path tmpPath;
        GetHomotopicPath(*it, tmpPath);

        // reducing words
        bool reduced = false;
        while (!reduced && outPath.size() > 0 && tmpPath.size() > 0)
        {
            if (outPath.back().first == tmpPath.front().first)
            {
                int sum = outPath.back().second + tmpPath.front().second;
                if (sum == 0)
                {
                    // removing last element from current list
                    typename Path::iterator last = outPath.end();
                    last--;
                    outPath.erase(last);
                    tmpPath.erase(tmpPath.begin());
                }
                else
                {
                    outPath.back().second = sum;
                    tmpPath.erase(tmpPath.begin());
                }
            }
            else
            {
                reduced = true;
            }
        }

        // if there's still something left just add it at the end
        outPath.insert(outPath.end(), tmpPath.begin(), tmpPath.end());
    }
}

template <typename Supplier>
void AKQHomotopicPaths<Supplier>::GetHomotopicPath(const PathCell& cell, Path& outPath)
{
    AKQType type = _strategy->akq[cell.first];
    if (type == Strategy::KING)
    {
        // no path
        outPath.clear();
    }
    else if (type == Strategy::ACE)
    {
        // critical cell
        outPath.clear();
        outPath.push_back(cell);
    }
    else
    {
        assert(type == Strategy::QUEEN);
        PathsMapIterator it = GetQueenHomotopicPath(cell.first);
        assert(it != _queenPaths.end());
        if (cell.second > 0)
        {
            outPath.assign(it->second.begin(), it->second.end());
        }
        else
        {
            ReverseNegate(it->second, outPath);
        }
    }
}

template <typename Supplier>
typename AKQHomotopicPaths<Supplier>::PathsMapIterator
AKQHomotopicPaths<Supplier>::GetQueenHomotopicPath(InputCellId cellId)
{
    // we can only compute homotopic path for QUEEN cell
    assert(_strategy->akq[cellId] == Strategy::QUEEN);

    // if the path has been already computed, we return it
    PathsMapIterator hpIt = _queenPaths.find(cellId);
    if (hpIt != _queenPaths.end())
    {
        return hpIt;
    }

    Path prevCells;
    Path nextCells;
    Path* currentCells = &prevCells;
    int orientation = 0;

    // then, we take a boundary of KING cell and compute
    // alternative path for traversing given QUEEN cell
    Path boundary = _complexSupplier->GetOrdered2Boundary(_originalComplex, _strategy->kerKing[cellId]);
    typename Path::iterator it = boundary.begin();
    typename Path::iterator itEnd = boundary.end();
    for ( ; it != itEnd; ++it)
    {
        int ci = it->second;
        assert(ci != 0);
        if (it->first == cellId)
        {
            orientation = ci;
            currentCells = &nextCells;
        }
        else
        {
            currentCells->push_back(*it);
        }
    }
    assert(orientation != 0);

    // based on orientation of QUEEN cell we join two paths ("from" it and "to")
    Path bdPath;
    if (orientation > 0)
    {
        Path tmp;
        ReverseNegate(prevCells, tmp);
        bdPath.insert(bdPath.end(), tmp.begin(), tmp.end());
        ReverseNegate(nextCells, tmp);
        bdPath.insert(bdPath.end(), tmp.begin(), tmp.end());
    }
    else
    {
        bdPath.insert(bdPath.end(), nextCells.begin(), nextCells.end());
        bdPath.insert(bdPath.end(), prevCells.begin(), prevCells.end());
    }

    // finally we need to compute homotopic paths for every cell on path
    Path homotopicPath;
    GetHomotopicPath(bdPath, homotopicPath);

    return _queenPaths.insert(std::make_pair(cellId, homotopicPath)).first;
}

template <typename Supplier>
void AKQHomotopicPaths<Supplier>::ReverseNegate(const Path& path, Path& outPath)
{
    outPath.clear();
    typename Path::const_reverse_iterator it = path.rbegin();
    typename Path::const_reverse_iterator itEnd = path.rend();
    for ( ; it != itEnd; ++it)
    {
        outPath.push_back(std::make_pair(it->first, -it->second));
    }
}

template <typename Supplier>
void AKQHomotopicPaths<Supplier>::Negate(Path& path)
{
    typename Path::iterator it = path.begin();
    typename Path::iterator itEnd = path.end();
    for ( ; it != itEnd; ++it)
    {
        it->second = -it->second;
    }
}

#endif	/* AKQHOMOTOPICPATHS_HPP */
