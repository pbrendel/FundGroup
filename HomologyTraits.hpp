/*
 * File:   HomologyTraits.hpp
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYTRAITS_HPP
#define	HOMOLOGYTRAITS_HPP

//#include <capd/vectalg/Matrix.hpp>
//#include <capd/vectalg/Vector.hpp>
//
//#include <capd/homAlgebra/FreeModule.h>
//#include <capd/homAlgebra/FreeChainComplex.h>
//#include <capd/homAlgebra/QuotientGradedModule.h>
//#include <capd/redAlg/ReducibleFreeChainComplex.h>
//
// #include <redHom/algorithm/Algorithms.hpp>
////#include <capd/matrixAlgorithms/intMatrixAlgorithms.hpp>
//
//#include <capd/homAlgebra/ChainT.h>

// SComplexFiltrT needs this
// remove after refactorization
#include <boost/foreach.hpp>

#include <capd/homAlgebra/ChainContainer.h>
#include <capd/RedHom/SComplexFiltrT.hpp>
#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/simplicial/SimplexSComplex.hpp>

template <int DIM>
class CubicalHomology
{
public:

    typedef CubSComplex<DIM>                                SComplexType;
    typedef typename SComplexType::Cell                     CellType;
    typedef double                                          ScalarType;
    typedef SComplexFiltrT<SComplexType, ScalarType>        FilterType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;
};

class SimplicialHomology
{
public:

    typedef SimplexSComplex                                 SComplexType;
    typedef typename SComplexType::Cell                     CellType;
    typedef double                                          ScalarType;
    typedef SComplexFiltrT<SComplexType, ScalarType>        FilterType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;
};

class SComplexHomology
{
public:

    typedef SComplex<SComplexDefaultTraits>                 SComplexType;
    typedef typename SComplexType::Cell                     CellType;
    typedef double                                          ScalarType;
    typedef SComplexFiltrT<SComplexType, ScalarType>        FilterType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;

};

#endif	/* HOMOLOGYTRAITS_HPP */

