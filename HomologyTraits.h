/*
 * File:   HomologyTraits.hpp
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYTRAITS_HPP
#define	HOMOLOGYTRAITS_HPP

#include <capd/homAlgebra/ChainContainer.h>
#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/simplicial/SimplexSComplex.hpp>

class DefaultHomologyTraits
{
public:

    typedef double                              ScalarType;
    typedef int                                 IntType;
    typedef SComplex<SComplexDefaultTraits>     GeneralSComplexType;
};

template <int D>
class CubicalHomology : public DefaultHomologyTraits
{
public:

    typedef CubSComplex<D>                                  SComplexType;
    typedef typename SComplexType::BCubSet                  CubSetType;
    typedef CRef<CubSetType>                                CubSetPtrType;
    typedef typename SComplexType::BCubCellSet              CubCellSetType;
    typedef CRef<CubCellSetType>                            CubCellSetPtrType;
    typedef size_t                                          CubCoordType;
    enum
    {
        DIM = D,
    };
};

class SimplicialHomology : public DefaultHomologyTraits
{
public:

    typedef SimplexSComplex                                 SComplexType;
};

class SComplexHomology : public DefaultHomologyTraits
{
public:

    typedef SComplex<SComplexDefaultTraits>                 SComplexType;
};

#endif	/* HOMOLOGYTRAITS_HPP */

