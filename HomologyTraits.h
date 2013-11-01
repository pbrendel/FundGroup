/*
 * File:   HomologyTraits.hpp
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYTRAITS_HPP
#define	HOMOLOGYTRAITS_HPP

// SComplexFiltrT needs this
// remove after refactorization
#include <boost/foreach.hpp>

#include <capd/homAlgebra/ChainContainer.h>
#include <capd/RedHom/SComplexFiltrT.hpp>
#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/simplicial/SimplexSComplex.hpp>

template <int DIM>
class CubicalFilteredHomology
{
public:

    typedef CubSComplex<DIM>                                SComplexType;
    typedef typename SComplexType::Cell                     CellType;
    typedef double                                          ScalarType;
    typedef SComplexFiltrT<SComplexType, ScalarType>        FilterType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;
};

class SimplicialFilteredHomology
{
public:

    typedef SimplexSComplex                                 SComplexType;
    typedef typename SComplexType::Cell                     CellType;
    typedef double                                          ScalarType;
    typedef SComplexFiltrT<SComplexType, ScalarType>        FilterType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;
};

template <int D>
class CubicalHomology
{
public:

    typedef CubSComplex<D>                                  InputSComplexType;
    typedef SComplex<SComplexDefaultTraits>                 OutputSComplexType;
    typedef typename OutputSComplexType::Cell               CellType;
    typedef double                                          ScalarType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;
    typedef typename InputSComplexType::BCubSet             CubSetType;
    typedef CRef<CubSetType>                                CubSetPtrType;
    typedef typename InputSComplexType::BCubCellSet         CubCellSetType;
    typedef CRef<CubCellSetType>                            CubCellSetPtrType;
    typedef size_t                                          CoordType;
    enum
    {
        DIM = D,
    };
};

class SimplicialHomology
{
public:

    typedef SimplexSComplex                                 InputSComplexType;
    typedef SComplex<SComplexDefaultTraits>                 OutputSComplexType;
    typedef typename OutputSComplexType::Cell               CellType;
    typedef double                                          ScalarType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;
};

class SComplexHomology
{
public:

    typedef SComplex<SComplexDefaultTraits>                 InputSComplexType;
    typedef SComplex<SComplexDefaultTraits>                 OutputSComplexType;
    typedef typename OutputSComplexType::Cell               CellType;
    typedef double                                          ScalarType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;

};

#endif	/* HOMOLOGYTRAITS_HPP */

