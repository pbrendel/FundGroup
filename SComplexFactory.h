/*
 * File:   SComplexFactory.h
 * Author: Piotr Brendel
 */

#ifndef SCOMPLEXFACTORY_H
#define	SCOMPLEXFACTORY_H

#include <boost/shared_ptr.hpp>

#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/simplicial/SimplexSComplex.hpp>

template <typename SComplexType>
class SComplexFactory
{
public:

    typedef SComplexType                SComplex;
    typedef boost::shared_ptr<SComplex> SComplexPtr;

    static SComplexPtr Create();
    static SComplexPtr Load(const char *filename);
};

template <int DIM>
class SComplexFactory<CubSComplex<DIM> >
{
public:

    typedef CubSComplex<DIM>            SComplex;
    typedef boost::shared_ptr<SComplex> SComplexPtr;

    static SComplexPtr Create();
    static SComplexPtr Load(const char *filename);

private:

    typedef typename CubSComplex<DIM>::BCubCellSet  CubCellSet;
    typedef CRef<CubCellSet>                        CubCellSetPtr;

    static void FillCubSet(CubCellSetPtr cubSet);
};

template <>
class SComplexFactory<SimplexSComplex>
{
public:

    typedef SimplexSComplex             SComplex;
    typedef boost::shared_ptr<SComplex> SComplexPtr;

    static SComplexPtr Create();
    static SComplexPtr Load(const char *filename);
};

#endif	/* SCOMPLEXFACTORY_H */

