/*
 * File:   SComplexFactory.h
 * Author: Piotr Brendel
 */

#ifndef SCOMPLEXFACTORY_H
#define	SCOMPLEXFACTORY_H

#include <boost/shared_ptr.hpp>

#include "DebugComplexType.h"

#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/simplicial/SimplexSComplex.hpp>

template <typename SComplexType>
class SComplexFactory
{
};

template <int DIM>
class SComplexFactory<CubSComplex<DIM> >
{
public:

    typedef CubSComplex<DIM>                SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char *filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    typedef typename CubSComplex<DIM>::BCubCellSet  CubCellSet;
    typedef CRef<CubCellSet>                        CubCellSetPtr;

    static void FillSphere2(CubCellSetPtr cubSet);
    static void FillSphere3(CubCellSetPtr cubSet);
    static void FillTorus(CubCellSetPtr cubSet);
    static void FillSkeleton(CubCellSetPtr cubSet);
    static void FillCustom0(CubCellSetPtr cubSet);
};

template <>
class SComplexFactory<SimplexSComplex>
{
public:

    typedef SimplexSComplex                 SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char *filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    static void FillSphere2(SComplexPtr complex);
    static void FillSphere3(SComplexPtr complex);
    static void FillTorus(SComplexPtr complex);
    static void FillSkeleton(SComplexPtr complex);
    static void FillCustom0(SComplexPtr complex);
};

template <typename Traits>
class SComplexFactory<SComplex<Traits> >
{
public:

    typedef SComplex<Traits>                SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char *filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    typedef typename SComplexType::Id       Id;
    typedef typename SComplexType::Dims     Dims;
    typedef typename SComplexType::KappaMap KappaMap;

    static void FillSphere2(Dims& dims, KappaMap& kappaMap);
    static void FillSphere3(Dims& dims, KappaMap& kappaMap);
    static void FillTorus(Dims& dims, KappaMap& kappaMap);
    static void FillSkeleton(Dims& dims, KappaMap& kappaMap);
    static void FillCustom0(Dims& dims, KappaMap& kappaMap);
};

#endif	/* SCOMPLEXFACTORY_H */

