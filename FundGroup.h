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

class IFundGroup
{
public:

    virtual ~IFundGroup() { }

    friend std::ostream& operator<<(std::ostream &str, IFundGroup& fg)
    {
        str<<fg.ToString();
        return str;
    }

    virtual void ExportHapProgram(const char* filename) const = 0;
    virtual std::string ToString() = 0;
};

template <typename ComplexSupplierType>
class FundGroup : public IFundGroup
{
public:
    typedef ComplexSupplierType                         ComplexSupplier;
    typedef boost::shared_ptr<ComplexSupplier>          ComplexSupplierPtr;

    FundGroup();
    FundGroup(const char *filename);
    FundGroup(ComplexSupplierPtr complexSupplier);
    FundGroup(DebugComplexType debugComplexType);

    void ExportHapProgram(const char* filename) const override;
    std::string HapFunctionBody() const;
    std::string HapExpression() const;
    std::vector<int> HapInterfaceVector() const;

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
    virtual std::string ToString() override;

    void PrintDebug();

};

#include "FundGroup.hpp"

#endif	/* FUNDGROUP_H */
