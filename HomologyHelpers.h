/*
 * File:   HomologyHelpers.h
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_H
#define	HOMOLOGYHELPERS_H

#include <boost/shared_ptr.hpp>
#include <vector>

class HomologyHelpers
{
public:

    template <typename SComplexPtr>
    static std::vector<int> GetBettiNumbers(SComplexPtr complex);
};

#include "HomologyHelpers.hpp"

#endif	/* HOMOLOGYHELPERS_H */

