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

    typedef CubSComplex<DIM>                        SComplexType;
    typedef boost::shared_ptr<SComplexType>         SComplexPtr;
    typedef typename CubSComplex<DIM>::BCubCellSet  CubCellSet;
    typedef CRef<CubCellSet>                        CubCellSetPtr;
    typedef typename CubSComplex<DIM>::BCubSet      CubSet;
    typedef CRef<CubSet>                            CubSetPtr;

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType type);
    static SComplexPtr Create(CubCellSetPtr cubCellSet);
    static SComplexPtr Create(CubSetPtr cubSet);
};

template <>
class SComplexFactory<SimplexSComplex>
{
public:

    typedef SimplexSComplex                 SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;
    typedef int                             Id;
    typedef std::set<Id>                    Simplex;
    typedef std::vector<Simplex>            Simplices;

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType type);
    static SComplexPtr Create(Simplices& simplices);
};

template <typename Traits>
class SComplexFactory<SComplex<Traits> >
{
public:

    typedef SComplex<Traits>                SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;
    typedef typename SComplexType::Id       Id;
    typedef typename SComplexType::Dim      Dim;
    typedef typename SComplexType::Dims     Dims;
    typedef typename SComplexType::KappaMap KappaMap;

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType type);
    static SComplexPtr Create(Dims& dims, KappaMap& kappaMap);

private:

    enum FileType
    {
        FT_Unknown,
        FT_KappaMap,
        FT_Cubes,
        FT_Simplices,
    };

    static FileType DetermineFileType(const char* filename);
};

#include "SComplexFactory.hpp"

#endif	/* SCOMPLEXFACTORY_H */
