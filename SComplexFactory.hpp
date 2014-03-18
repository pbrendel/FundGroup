/*
 * File:   SComplexFactory.hpp
 * Author: Piotr Brendel
 */

#ifndef SCOMPLEXFACTORY_HPP
#define	SCOMPLEXFACTORY_HPP

#include <redHom/complex/scomplex/SComplexReader.hpp>
#include "SComplexFactory.h"

#include "CubSetFactory.h"
#include "CubCellSetFactory.h"
#include "KappaMapSupplier.h"
#include "SimplicesSupplier.h"

#include "FGLogger.h"

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Load(const char* filename)
{
    //CubCellSetPtr cubCellSet = CubCellSetFactory<CubCellSet>::Load(filename, true);
    //return Create(cubCellSet);
    CubSetPtr cubSet = CubSetFactory<CubSet>::Load(filename, true);
    return Create(cubSet);
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create(DebugComplexType type)
{
    //CubCellSetPtr cubCellSet = CubCellSetFactory<CubCellSet>::Create(type, true);
    //return Create(cubCellSet);
    CubSetPtr cubSet = CubSetFactory<CubSet>::Create(type, true);
    return Create(cubSet);
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create(CubCellSetPtr cubCellSet)
{
    FGLogger logger;
    logger.Begin(FGLogger::Details, "creating SComplex");
    SComplexPtr complex = SComplexPtr(new SComplexType(cubCellSet, false));
    logger.End();

    return complex;
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create(CubSetPtr cubSet)
{
    FGLogger logger;
    logger.Begin(FGLogger::Details, "creating CubCellSet");
    CubCellSetPtr cubCellSet = CubCellSetPtr(new CubCellSet(cubSet()));
    logger.End();

    logger.Begin(FGLogger::Details, "creating SComplex");
    SComplexPtr complex = SComplexPtr(new SComplexType(cubCellSet, false));
    logger.End();

    return complex;
}

SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Load(const char* filename)
{
    Simplices simplices;
    SimplicesSupplier<Id>::Load(filename, simplices);
    return Create(simplices);
}

SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Create(DebugComplexType type)
{
    Simplices simplices;
    SimplicesSupplier<Id>::Create(type, simplices);
    return Create(simplices);
}

SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Create(Simplices& simplices)
{
    SComplexPtr complex = SComplexPtr(new SComplexType());
    Simplices::iterator it = simplices.begin();
    Simplices::iterator itEnd = simplices.end();
    for ( ; it != itEnd; ++it)
    {
        complex->addSimplex(*it);
    }
    return complex;
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::Load(const char* filename)
{
    Dims dims;
    KappaMap kappaMap;
    SComplexReader<Traits> reader;
    FileType fileType = DetermineFileType(filename);
    switch (fileType)
    {
        case FT_KappaMap:
            KappaMapSupplier<Id, int, Dim>::Load(filename, dims, kappaMap);
            break;
        case FT_Cubes:
            return reader(filename, 3, 1);
        case FT_Simplices:
        default:
            throw std::logic_error("not implemented");
    }
    return SComplexPtr(new SComplexType(3, dims, kappaMap, 1));
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::Create(DebugComplexType type)
{
    Dims dims;
    KappaMap kappaMap;
    KappaMapSupplier<Id, int, Dim>::Create(type, dims, kappaMap);
    return SComplexPtr(new SComplexType(3, dims, kappaMap, 1));
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::Create(Dims& dims, KappaMap& kappaMap)
{
    return SComplexPtr(new SComplexType(3, dims, kappaMap, 1));
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::FileType
SComplexFactory<SComplex<Traits> >::DetermineFileType(const char* filename)
{
    // simple but error-prone determining file type by its extension

    size_t len = strlen(filename);

    if (len < 5)
    {
        // cannot determine by its extension
        // assuming default - list of full cubes
        return FT_Cubes;
    }

    if (   (tolower(filename[len - 3]) == 'k')
        && (tolower(filename[len - 2]) == 'a')
        && (tolower(filename[len - 1]) == 'p') )
    {
        return FT_KappaMap;
    }

    if (   (tolower(filename[len - 3]) == 's')
        && (tolower(filename[len - 2]) == 'i')
        && (tolower(filename[len - 1]) == 'm') )
    {
        return FT_Simplices;
    }

    return FT_Cubes;
}

#endif	/* SCOMPLEXFACTORY_HPP */
