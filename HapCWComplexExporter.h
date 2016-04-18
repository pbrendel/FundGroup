/*
 * File:   HapCWComplexExporter.h
 * Author: Piotr Brendel
 */

#ifndef HAPCWCOMPLEXEXPORTER_H
#define	HAPCWCOMPLEXEXPORTER_H

#include <vector>
#include <boost/shared_ptr.hpp>

template <typename ComplexType>
class HapCWComplexExporter
{
public:

    typedef boost::shared_ptr<ComplexType>  ComplexPtr;
    typedef AKQReduceStrategy<ComplexType>  Strategy;
    typedef Strategy*                       StrategyPtr;

    void CollectComplexData(ComplexPtr complex);
    void CollectVectorFieldData(ComplexPtr complex, StrategyPtr strategy);
    void GenerateTrivialVectorField(ComplexPtr complex);
    void ExportData(const char* filename);

    void PrintIdIndexMap();

private:

    typedef typename ComplexType::Cell      Cell;
    typedef typename ComplexType::Iterators Iterators;
    typedef Iterators::AllCells    AllCells;
    typedef Iterators::DimCells    DimCells;
    typedef Iterators::BdCells     BdCells;
    typedef Iterators::CbdCells    CbdCells;
    typedef typename ComplexType::Id        CellId;
    typedef typename ComplexType::Dim       Dim;

    Dim                                             _maxDim;
    std::vector<std::vector<std::vector<CellId> > > _boundaries;
    std::vector<std::vector<std::vector<CellId> > > _coboundaries;
    std::vector<std::vector<std::vector<int> > >    _orientation;
    std::vector<std::map<CellId, size_t> >          _idIndexMapByDim;
    std::vector<std::vector<int> >                  _vectorField;
    std::vector<std::vector<int> >                  _inverseVectorField;
    std::vector<std::vector<int> >                  _criticalCells;
};

#include "HapCWComplexExporter.hpp"

#endif	/* HAPCWCOMPLEXEXPORTER_H */

