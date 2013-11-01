/*
 * File:   HomologyHelpers.h
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_H
#define	HOMOLOGYHELPERS_H

#include <boost/shared_ptr.hpp>
#include <vector>

template <typename Traits>
class HomologyHelpers
{
public:

    typedef typename Traits::SComplexType   SComplex;
    typedef boost::shared_ptr<SComplex>     SComplexPtr;
    typedef typename Traits::FilterType     Filter;
    typedef boost::shared_ptr<Filter>       FilterPtr;
    typedef typename Traits::ScalarType     Scalar;

    static FilterPtr CreateFilter(SComplexPtr complex,
                                  bool useShaving,
                                  bool useCoreductions,
                                  bool restore);
    static std::vector<int> GetBettiNumbers(FilterPtr filter);
    static std::vector<int> GetBettiNumbers(SComplexPtr complex);
};

#include "HomologyHelpers.hpp"

#endif	/* HOMOLOGYHELPERS_H */

