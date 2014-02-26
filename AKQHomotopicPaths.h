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
    typedef typename Traits::InputSComplex      InputSComplex;
    typedef typename Traits::OutputSComplex     OutputSComplex;

public:
    typedef typename InputSComplex::Id          InputCellId;
    typedef typename OutputSComplex::Id         OutputCellId;
    typedef AKQReduceStrategy<InputSComplex>    Strategy;
    typedef typename OutputSComplex::Cell       OutputCell;
    typedef std::vector<std::pair<OutputCellId, int> > OutputChain;

    AKQHomotopicPaths(Strategy *strategy);

    OutputChain GetHomotopicBoundary(const OutputCellId& cell);

private:

    typedef typename Strategy::AKQType                  AKQType;
    typedef typename InputSComplex::Cell                InputCell;
    typedef std::vector<std::pair<InputCellId, int> >   InputChain;
    typedef std::pair<InputCellId, int>                 PathCell;
    typedef std::list<PathCell>                         Path;
    typedef std::map<InputCellId, Path>                 PathsMap;
    typedef typename PathsMap::iterator                 PathsMapIterator;
    typedef boost::bimap<InputCellId, OutputCellId>     AcesMap;
    typedef typename InputSComplex::Iterators::BdCells  BdCells;

    void ComputeAcesMap();
    void GetHomotopicPath(const Path& path, Path& outPath);
    void GetHomotopicPath(const PathCell& cell, Path& outPath);
    PathsMapIterator GetQueenHomotopicPath(InputCellId cellId);
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
