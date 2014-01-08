/*
 * File:   AKQHomotopicPaths.h
 * Author: Piotr Brendel
 */

#ifndef AKQHOMOTOPICPATHS_H
#define	AKQHOMOTOPICPATHS_H

#include <list>
#include <map>
#include <vector>
#include <boost/bimap.hpp>
#include <redHom/algorithm/AKQStrategy.hpp>

template <typename Traits>
class AKQHomotopicPaths
{
    typedef typename Traits::OutputSComplexType InputSComplex;
    typedef typename Traits::OutputSComplexType OutputSComplex;

public:

    typedef AKQReduceStrategy<InputSComplex>    Strategy;
    typedef typename OutputSComplex::Cell       Cell;
    typedef std::vector<std::pair<Cell, int> >  Chain;

    AKQHomotopicPaths(Strategy *strategy);

    Chain GetHomotopicBoundary(const Cell& cell);

private:

    typedef typename Strategy::AKQType                  AKQType;
    typedef size_t                                      CellId;
    typedef std::pair<CellId, int>                      PathCell;
    typedef std::list<PathCell>                         Path;
    typedef std::map<CellId, Path>                      PathsMap;
    typedef typename PathsMap::iterator                 PathsMapIterator;
    typedef boost::bimap<CellId, CellId>                AcesMap;
    typedef typename InputSComplex::Iterators::BdCells  BdCells;

    void ComputeAcesMap();
    void GetHomotopicPath(const Path& path, Path& outPath);
    void GetHomotopicPath(const PathCell& cell, Path& outPath);
    PathsMapIterator GetQueenHomotopicPath(CellId cellId);
    void ReverseNegate(const Path& path, Path& outPath);
    void Negate(Path& path);

    Strategy*       _strategy;
    InputSComplex*  _originalComplex;
    OutputSComplex* _outputComplex;
    PathsMap        _queenPaths;
    AcesMap         _acesMap;
};

#include "AKQHomotopicPaths.hpp"

#endif	/* AKQHOMOTOPICPATHS_H */

