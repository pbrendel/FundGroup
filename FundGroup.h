/*
 * File:   FundGroup.h
 * Author: Piotr Brendel
 */

#ifndef FUNDGROUP_H
#define	FUNDGROUP_H

#include <map>
#include <ostream>
#include <set>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "FGLogger.h"

template <typename ComplexSupplierType>
class FundGroup
{
public:
    typedef ComplexSupplierType                         ComplexSupplier;
    typedef boost::shared_ptr<ComplexSupplier>          ComplexSupplierPtr;

    FundGroup();
    FundGroup(const char *filename);
    FundGroup(ComplexSupplierPtr complexSupplier);
    FundGroup(DebugComplexType debugComplexType);

    friend std::ostream& operator<<(std::ostream &str, FundGroup& g)
    {
        str<<g.ToString();
        return str;
    }

    void ExportHapProgram(const char* filename) const;
    std::string HapFunctionBody() const;
    std::string HapExpression() const;

private:

  //    typedef typename ComplexSupplierType::Cell          Cell;
    typedef typename ComplexSupplierType::Id            Id;
    typedef typename ComplexSupplierType::Cells         Cells;
    typedef typename ComplexSupplierType::CellsByDim    CellsByDim;
    typedef typename ComplexSupplierType::Chain         Chain;
    typedef std::pair<Id, int>                          RelatorComponent;
    typedef std::vector<RelatorComponent>               Relator;
    typedef std::vector<Relator>                        Relators;

    ComplexSupplierPtr      _complexSupplier;
    CellsByDim              _cellsByDim;
    std::map<Id, Chain>     _2Boundaries;
    Cells                   _spanningTreeEdges;
    Relators                _relators;
    FGLogger                _logger;

    void Compute();
    void CreateSpanningTree();
    void ComputeRelators();
    void SimplifyRelators();
    std::string ToString();

    void PrintDebug();

};

#include "FundGroup.hpp"

#endif	/* FUNDGROUP_H */
