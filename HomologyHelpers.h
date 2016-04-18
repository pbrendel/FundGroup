/*
 * File:   HomologyHelpers.h
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_H
#define	HOMOLOGYHELPERS_H

#include <list>
#include <vector>
#include <capd/complex/CubCellComplex.h>

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


  template <typename T>
  static void Reorder2Boundary(CubCellComplex<T>* complex,
                               std::list<std::pair<typename CubCellComplex<T>::Id, int> >& boundary);

};

#include "HomologyHelpers.hpp"


#endif	/* HOMOLOGYHELPERS_H */
