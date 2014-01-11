/*
 * File:   AKQHomotopicPaths.hpp
 * Author: Piotr Brendel
 */

#ifndef AKQHOMOTOPICPATHS_HPP
#define	AKQHOMOTOPICPATHS_HPP

#include "AKQHomotopicPaths.h"

#include "Logger.h"

template <typename Traits>
AKQHomotopicPaths<Traits>::AKQHomotopicPaths(Strategy* strategy)
    : _strategy(strategy)
{
    assert(_strategy != 0);
    _originalComplex = &_strategy->getComplex();
    _outputComplex = _strategy->getOutputComplex();
    ComputeAcesMap();
}

template <typename Traits>
void AKQHomotopicPaths<Traits>::ComputeAcesMap()
{
    OutputCellId id = OutputCellId(0);
    BOOST_FOREACH(InputCell ace, _strategy->aces)
    {
        _acesMap.insert(typename AcesMap::value_type(ace.getId(), id++));
    }
}

#define LOG Logger::Log(Logger::Debug)

template <typename Traits>
typename AKQHomotopicPaths<Traits>::OutputChain
AKQHomotopicPaths<Traits>::GetHomotopicBoundary(const OutputCell& cell)
{
    // cell needs to be an ace
    assert(_acesMap.right.find(cell.getId()) != _acesMap.right.end());
    // we get an original cell
    InputCell originalCell = (*_originalComplex)[_acesMap.right.at(cell.getId())];
    assert(_strategy->akq[originalCell.getId()] == Strategy::ACE);

    //LOG<<"cell id: "<<cell.getId()<<" original id: "<<originalCell.getId()<<std::endl;

    // we take its boundary cells
    BdCells bdCells = _originalComplex->iterators().bdCells(originalCell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    Path bdPath;
    //LOG<<"its boundary:"<<std::endl;
    for ( ; it != itEnd; ++it)
    {
        //LOG<<it->getId()<<std::endl;
        int ci = _originalComplex->coincidenceIndex(originalCell, *it);
        assert(ci != 0);
        bdPath.push_back(PathCell(it->getId(), ci));
    }

    Path homotopicPath;
    GetHomotopicPath(bdPath, homotopicPath);

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
        boundary.push_back(std::pair<OutputCell, int>(ace, ci));
        //LOG<<"old: "<<jt->second<<" new: "<<ci<<std::endl;
    }
    return boundary;
}

template <typename Traits>
void AKQHomotopicPaths<Traits>::GetHomotopicPath(const Path& path, Path& outPath)
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

template <typename Traits>
void AKQHomotopicPaths<Traits>::GetHomotopicPath(const PathCell& cell, Path& outPath)
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
            outPath = it->second;
        }
        else
        {
            ReverseNegate(it->second, outPath);
        }
    }
//        LOG<<"path for cell "<<cell.first<<" ("<<type<<") is:"<<std::endl;
//        for ( typename TBdList::iterator pIt = outPath.begin(); pIt != outPath.end(); pIt++)
//        {
//            LOG<<pIt->first<<std::endl;
//        }
}

template <typename Traits>
typename AKQHomotopicPaths<Traits>::PathsMapIterator
AKQHomotopicPaths<Traits>::GetQueenHomotopicPath(InputCellId cellId)
{
    // we can only compute homotopic path for QUEEN cell
    assert(_strategy->akq[cellId] == Strategy::QUEEN);

    // if the path has been already computed, we return it
    PathsMapIterator hpIt = _queenPaths.find(cellId);
    if (hpIt != _queenPaths.end())
    {
        return hpIt;
    }

    // first we need to take KING cell
    InputCell* kingCell = _strategy->kerKing[cellId];

    Path prevCells;
    Path nextCells;
    Path* currentCells = &prevCells;
    int orientation = 0;

    // then, we take a boundary of KING cell and compute
    // alternative path for traversing given QUEEN cell
    BdCells bdCells = _originalComplex->iterators().bdCells(*kingCell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();

    //LOG<<"computing alternative path for queen "<<cellId<<" and king "<<kingCell->getId()<<std::endl;
    //LOG<<"king boundary size is not null "<<(it != itEnd)<<std::endl;

    for ( ; it != itEnd; ++it)
    {
        int ci = _originalComplex->coincidenceIndex(*kingCell, *it);
        assert(ci != 0);
        if (it->getId() == cellId)
        {
            orientation = ci;
            currentCells = &nextCells;
        }
        else
        {
            //LOG<<"adding to current cells: "<<it->getId()<<std::endl;
            currentCells->push_back(std::make_pair(it->getId(), ci));
            //LOG<<"current cells size: "<<currentCells->size();
            //LOG<<"prev cells size: "<<prevCells.size()<<std::endl;
            //LOG<<"next cells size: "<<nextCells.size()<<std::endl;
        }
    }
    assert(orientation != 0);

    // based on orientation of QUEEN cell we join two paths ("from" it and "to")
    Path bdPath;
    if (orientation > 0)
    {
        Negate(prevCells);
        Negate(nextCells);
        bdPath.insert(bdPath.end(), prevCells.begin(), prevCells.end());
        bdPath.insert(bdPath.end(), nextCells.begin(), nextCells.end());
    }
    else
    {
        bdPath.insert(bdPath.end(), nextCells.begin(), nextCells.end());
        bdPath.insert(bdPath.end(), prevCells.begin(), prevCells.end());
    }

//        LOG<<"alternative path is:"<<std::endl;
//        for (typename Path::iterator pIt = bdPath.begin(); pIt != bdPath.end(); ++pIt)
//        {
//            LOG<<pIt->first<<std::endl;
//        }

    // finally we need to compute homotopic paths for every cell on path
    Path homotopicPath;
    GetHomotopicPath(bdPath, homotopicPath);

    return _queenPaths.insert(std::make_pair(cellId, homotopicPath)).first;
}

template <typename Traits>
void AKQHomotopicPaths<Traits>::ReverseNegate(const Path& path, Path& outPath)
{
    outPath.clear();
    typename Path::const_reverse_iterator it = path.rbegin();
    typename Path::const_reverse_iterator itEnd = path.rend();
    for ( ; it != itEnd; ++it)
    {
        outPath.push_back(std::make_pair(it->first, -it->second));
    }
}

template <typename Traits>
void AKQHomotopicPaths<Traits>::Negate(Path& path)
{
    typename Path::iterator it = path.begin();
    typename Path::iterator itEnd = path.end();
    for ( ; it != itEnd; ++it)
    {
        it->second = -it->second;
    }
}

#undef LOG

#endif	/* AKQHOMOTOPICPATHS_HPP */

