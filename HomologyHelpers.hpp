/*
 * File:   HomologyHelpers.hpp
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_HPP
#define	HOMOLOGYHELPERS_HPP

#include "HomologyHelpers.h"

#include <capd/complex/ConvertToRFC.h>

template <typename Traits>
template <typename SComplexType>
std::vector<int> HomologyHelpers<Traits>::GetHomologySignature(SComplexType* complex)
{
    using namespace capd::redHom;

    typedef typename Traits::ScalarType         Scalar;
    typedef typename Traits::IntType            Int;
    typedef FreeModule<typename SComplexType::Id, capd::vectalg::Matrix<Scalar, 0, 0>, Int> FreeModuleType;
    typedef ReducibleFreeChainComplex<FreeModuleType> ReducibleFreeChainComplexType;

    ConvertToRFC<SComplexType, Scalar> rfcBuilder(*complex);
    CRef<ReducibleFreeChainComplexType> RFCComplexCR = rfcBuilder.template build<ReducibleFreeChainComplexType>();
    CRef<HomologySignature<Scalar, Int> > homSignCR = HomAlgFunctors<FreeModuleType>::homSignViaAR_Random(RFCComplexCR);
    std::vector<Scalar> betti = homSignCR().ranks();

    std::vector<int> res;
    for (typename std::vector<Scalar>::iterator it = betti.begin(); it != betti.end(); ++it)
    {
        res.push_back(static_cast<int>(*it));
    }
    return res;
}

#endif	/* HOMOLOGYHELPERS_HPP */

