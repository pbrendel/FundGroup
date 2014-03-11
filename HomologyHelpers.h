/*
 * File:   HomologyHelpers.h
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_H
#define	HOMOLOGYHELPERS_H

#include <list>
#include <vector>

#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/scomplex/SComplex.hpp>

template <typename Traits>
class HomologyHelpers
{
public:

    template <typename SComplexType>
    static std::vector<int> GetHomologySignature(SComplexType *complex);

    template <typename SComplexType>
    static void Reorder2Boundary(SComplexType*,
                    std::list<std::pair<typename SComplexType::Id, int> > &boundary);

    template <typename ComplexTraits>
    static void Reorder2Boundary(SComplex<ComplexTraits>*,
                    std::list<std::pair<typename SComplex<ComplexTraits>::Id, int> > &boundary);

    template <int DIM>
    static void Reorder2Boundary(CubSComplex<DIM>*,
                    std::list<std::pair<typename CubSComplex<DIM>::Id, int> > &boundary);

};

#include "HomologyHelpers.hpp"

#endif	/* HOMOLOGYHELPERS_H */

