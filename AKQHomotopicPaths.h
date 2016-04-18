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
#include <capd/complex/AKQStrategy.hpp>

template <typename Supplier>
class AKQHomotopicPaths
{
    typedef typename Supplier::InputSComplex    InputSComplex;
    typedef typename Supplier::OutputSComplex   OutputSComplex;
    typedef typename Supplier::Scalar           Scalar;

public:
    typedef typename InputSComplex::Id          InputCellId;
    typedef typename OutputSComplex::Id         OutputCellId;
    typedef capd::complex::AKQReduceStrategy<InputSComplex, OutputSComplex, Scalar>    Strategy;
    typedef typename OutputSComplex::Cell       OutputCell;
    typedef std::vector<std::pair<OutputCellId, int> > OutputChain;

    AKQHomotopicPaths(Supplier *complexSupplier, Strategy *strategy);

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

    Supplier*       _complexSupplier;
    Strategy*       _strategy;
    InputSComplex*  _originalComplex;
    OutputSComplex* _outputComplex;
    PathsMap        _queenPaths;
    AcesMap         _acesMap;
};

#include "AKQHomotopicPaths.hpp"

#endif	/* AKQHOMOTOPICPATHS_H */
