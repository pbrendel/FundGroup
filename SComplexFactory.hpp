/*
 * File:   SComplexFactory.hpp
 * Author: Piotr Brendel
 */

#ifndef SCOMPLEXFACTORY_HPP
#define	SCOMPLEXFACTORY_HPP

#include "SComplexFactory.h"

#include <exception>

#include <capd/bitSet/BitmapT.hpp>
#include <capd/bitSet/EuclBitSetT.hpp>

template <typename SComplexType>
typename SComplexFactory<SComplexType>::SComplexPtr
SComplexFactory<SComplexType>::Create()
{
    throw std::logic_error("don't use it");
}

template <typename SComplexType>
typename SComplexFactory<SComplexType>::SComplexPtr
SComplexFactory<SComplexType>::Load(const char*)
{
    throw std::logic_error("don't use it");
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create()
{
    // 3^n cubical set
    CubCellSetPtr cubSet = CubCellSetPtr(new CubCellSet(3, true));
    FillCubSet(cubSet);
    SComplexPtr complex = SComplexPtr(new SComplex(cubSet));
    return complex;
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Load(const char*)
{
    throw std::logic_error("loading CubSComplex not implemented yet");
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillCubSet(CubCellSetPtr cubSet)
{
    // filling with circle made of full cubes in R^3
    // xxx
    // x x
    // xxx
    int point[3] = { 0, 0, 0 };
    for (int i = 0; i < 3; i++)
    {
        point[0] = i;
        for (int j = 0; j < 3; j++)
        {
            // leaving center empty
            if (i == 1 && j == 1)
            {
                continue;
            }
            point[1] = j;
            cubSet().insert(point);
        }
    }
    // second circle ortogonal to the first one
//    point[0] = 0;
//    for (int i = 0; i < 3; i++)
//    {
//        point[2] = i;
//        for (int j = 0; j < 3; j++)
//        {
//            // leaving center empty
//            if (i == 1 && j == 1)
//            {
//                continue;
//            }
//            point[1] = j;
//            cubSet().insert(point);
//        }
//    }
}

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Create()
{
    throw std::logic_error("creating SimplexSComplex not implemented yet");
}

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Load(const char*)
{
    throw std::logic_error("loading SimplexSComplex not implemented yet");
}

#endif	/* SCOMPLEXFACTORY_HPP */

