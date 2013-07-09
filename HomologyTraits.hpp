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

//int Zp::p = 1009;
//int Zp::p = 2;

template <int DIM>
class HomologyTraits
{
public:

    typedef CubSComplex<DIM>                                SComplexType;
    // typedef SimplexSComplex                              SComplexType;
    typedef typename SComplexType::Cell                     CellType;
    typedef double                                          ScalarType;
    typedef SComplexFiltrT<SComplexType, ScalarType>        FilterType;
    typedef ChainT<ChainContainer<CellType, ScalarType> >   ChainType;

    //typedef capd::vectalg::Matrix<ScalarType, 0, 0> MatrixType;
    //typedef capd::vectalg::Vector<ScalarType, 0> VectorType;
    //
};

#endif	/* HOMOLOGYTRAITS_HPP */

