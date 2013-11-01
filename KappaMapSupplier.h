/*
 * File:   KappaMapSupplier.h
 * Author: Piotr Brendel
 */

#ifndef KAPPAMAPSUPPLIER_H
#define	KAPPAMAPSUPPLIER_H

#include <vector>
#include <boost/tuple/tuple.hpp>

#include "DebugComplexType.h"

template <typename IdT = int, typename IndexT = int, typename DimT = int>
class KappaMapSupplier
{
public:

    typedef IdT                         Id;
    typedef IndexT                      Index;
    typedef boost::tuple<Id, Id, Index> KappaMapEntry;
    typedef std::vector<KappaMapEntry>  KappaMap;
    typedef DimT                        Dim;
    typedef std::vector<Dim>            Dims;

    static void Load(const char* filename, Dims& dims, KappaMap& kappaMap);
    static void Create(DebugComplexType type, Dims& dims, KappaMap& kappaMap);

private:

    static void FillSphere2(Dims& dims, KappaMap& kappaMap);
    static void FillSphere3(Dims& dims, KappaMap& kappaMap);
    static void FillTorus(Dims& dims, KappaMap& kappaMap);
    static void FillSkeleton(Dims& dims, KappaMap& kappaMap);
    static void FillCustom0(Dims& dims, KappaMap& kappaMap);
};

#include "KappaMapSupplier.hpp"

#endif	/* KAPPAMAPSUPPLIER_H */