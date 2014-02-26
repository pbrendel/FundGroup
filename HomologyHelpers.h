/*
 * File:   HomologyHelpers.h
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_H
#define	HOMOLOGYHELPERS_H

#include <vector>

template <typename Traits>
class HomologyHelpers
{
public:

    template <typename SComplexType>
    static std::vector<int> GetHomologySignature(SComplexType *complex);

};

#include "HomologyHelpers.hpp"

#endif	/* HOMOLOGYHELPERS_H */

