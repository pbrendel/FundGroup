/*
 * File:   HomologyHelpers.hpp
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_HPP
#define	HOMOLOGYHELPERS_HPP

#include "HomologyHelpers.h"

#include <capd/complex/BettiNumbers.hpp>

template <typename Traits>
template <typename SComplexType>
std::vector<int> HomologyHelpers<Traits>::GetHomologySignature(SComplexType* complex)
{
    using namespace capd::complex;

    typedef typename Traits::ScalarType         Scalar;
    typedef typename Traits::IntType            Int;

    typedef BettiNumbers<SComplexType, Scalar, Int> BettiNumbersAlg;
    BettiNumbersAlg alg(*complex);
    std::vector<Int> betti = alg();

    std::vector<int> res;
    for (typename std::vector<Int>::iterator it = betti.begin(); it != betti.end(); ++it)
    {
        res.push_back(static_cast<int>(*it));
    }
    return res;
}

template <typename Traits>
template <typename SComplexType>
void HomologyHelpers<Traits>::Reorder2Boundary(SComplexType*,
         std::list<std::pair<typename SComplexType::Id, int> >& boundary)
{
    // in general case does nothing
}

namespace
{

template <typename ComplexType>
void Get1Boundary(ComplexType* complex, typename ComplexType::Id cellId, typename ComplexType::Id& v0, typename ComplexType::Id& v1)
{
    typedef typename ComplexType::Id Id;
    typedef typename ComplexType::Cell Cell;
    typedef typename ComplexType::Iterators::BdCells BdCells;
    Cell cell = (*complex)[cellId];
    std::list<std::pair<Id, int> > boundary;
    BdCells bdCells = complex->iterators().bdCells(cell);
    typename BdCells::iterator it = bdCells.begin();
    typename BdCells::iterator itEnd = bdCells.end();
    std::vector<Id> verts;
    for( ; it != itEnd; ++it)
    {
        verts.push_back(it->getId());
    }

    if (verts.size() == 2)
    {
        v0 = verts[0];
        v1 = verts[1];
    }
    else if (verts.size() == 1)
    {
        v0 = v1 = verts[0];
    }
}

template <typename ComplexType>
void Print2Boundary(ComplexType* complex, std::list<std::pair<typename ComplexType::Id, int> >& boundary)
{
    typedef typename ComplexType::Id Id;
    typename std::list<std::pair<Id, int> >::iterator it = boundary.begin();
    typename std::list<std::pair<Id, int> >::iterator itEnd = boundary.end();
    for( ; it != itEnd; ++it)
    {
        Id v0, v1;
        Get1Boundary(complex, it->first, v0, v1);
        std::cout<<v0<<" "<<v1<<" "<<it->second<<std::endl;
    }
}

}

template <typename Traits>
template <typename ComplexTraits>
void HomologyHelpers<Traits>::Reorder2Boundary(SComplex<ComplexTraits>*,
         std::list<std::pair<typename SComplex<ComplexTraits>::Id, int> >& boundary)
{
    typedef typename SComplex<ComplexTraits>::Id Id;
    typedef std::pair<Id, int> Cell;
    typedef std::list<Cell> CellsList;

    // reordering (1, 2, 4, 3) -> (1, 2, 3, 4)
    assert(boundary.size() == 4);
    typename CellsList::iterator it = boundary.begin();
    // skip first two
    it++;
    it++;
    // remember and remove 3rd element
    Cell cell = *it;
    it = boundary.erase(it);
    // insert remembered cell at the end
    it++;
    boundary.insert(it, cell);
}

template <typename Traits>
template <int DIM>
void HomologyHelpers<Traits>::Reorder2Boundary(CubSComplex<DIM>* complex,
        std::list<std::pair<typename CubSComplex<DIM>::Id, int> >& boundary)
{
    typedef typename CubSComplex<DIM>::Id Id;
    typedef std::pair<Id, int> Cell;
    typedef std::list<Cell> CellsList;

    // reordering (1, 3, 4, 2) -> (1, 2, 3, 4)
    assert(boundary.size() == 4);
    // remember last cell and remove ir
    typename CellsList::iterator it = boundary.end();
    it--;
    Cell cell = *it;
    boundary.erase(it);
    // skip first cell and insert remembered cell at second position
    it = boundary.begin();
    it++;
    boundary.insert(it, cell);
}

template <typename Traits>
template <typename T>
void HomologyHelpers<Traits>::Reorder2Boundary(CubCellComplex<T>* complex,
        std::list<std::pair<typename CubCellComplex<T>::Id, int> >& boundary)
{
    typedef typename CubCellComplex<T>::Id Id;
    typedef std::pair<Id, int> Cell;
    typedef std::list<Cell> CellsList;

    // reordering (1, 3, 4, 2) -> (1, 2, 3, 4)
    assert(boundary.size() == 4);
    // remember last cell and remove ir
    typename CellsList::iterator it = boundary.end();
    it--;
    Cell cell = *it;
    boundary.erase(it);
    // skip first cell and insert remembered cell at second position
    it = boundary.begin();
    it++;
    boundary.insert(it, cell);
}

#endif	/* HOMOLOGYHELPERS_HPP */
