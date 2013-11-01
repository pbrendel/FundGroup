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

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType type);

private:

    typedef typename CubSComplex<DIM>::BCubCellSet  CubCellSet;
    typedef CRef<CubCellSet>                        CubCellSetPtr;
    typedef typename CubCellSet::EuclSet            EuclSet;
    typedef CubSetT<EuclSet>                        CubSet;
    typedef CRef<CubSet>                            CubSetPtr;

    static SComplexPtr Create(CubCellSetPtr);
    static SComplexPtr Create(CubSetPtr);
};

template <>
class SComplexFactory<SimplexSComplex>
{
public:

    typedef SimplexSComplex                 SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char* filename);
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

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    typedef typename SComplexType::Id       Id;
    typedef typename SComplexType::Dims     Dims;
    typedef typename SComplexType::KappaMap KappaMap;

    enum FileType
    {
        FT_Unknown,
        FT_KappaMap,
        FT_Cubes,
        FT_Simplices,
    };

    static FileType DetermineFileType(const char* filename);

    static SComplexPtr LoadKappaMap(const char* filename);
    static SComplexPtr LoadCubes(const char* filename);
    static SComplexPtr LoadSimplices(const char* filename);

    static void FillSphere2(Dims& dims, KappaMap& kappaMap);
    static void FillSphere3(Dims& dims, KappaMap& kappaMap);
    static void FillTorus(Dims& dims, KappaMap& kappaMap);
    static void FillSkeleton(Dims& dims, KappaMap& kappaMap);
    static void FillCustom0(Dims& dims, KappaMap& kappaMap);
};

#include "SComplexFactory.hpp"

#endif	/* SCOMPLEXFACTORY_H */
