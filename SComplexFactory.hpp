/*
 * File:   SComplexFactory.hpp
 * Author: Piotr Brendel
 */

#ifndef SCOMPLEXFACTORY_HPP
#define	SCOMPLEXFACTORY_HPP

#include "SComplexFactory.h"

#include <exception>
#include <fstream>

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
SComplexFactory<SComplexType>::Load(const char *filename)
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
SComplexFactory<CubSComplex<DIM> >::Load(const char *filename)
{
    throw std::logic_error("loading CubSComplex not implemented yet");
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillCubSet(CubCellSetPtr cubSet)
{
    // filling with "edges of cube" in R^3
    int point[3] = { 0, 0, 0 };
    for (int i = 0; i < 3; i++)
    {
        point[0] = i;
        for (int j = 0; j < 3; j++)
        {
            point[1] = j;
            for (int k = 0; k < 3; k++)
            {
                // leaving center empty
                if ((i == 1 && j == 1) || (i == 1 && k == 1) || (j == 1) && (k == 1))
                {
                    continue;
                }
                point[2] = k;
                cubSet().insert(point);
            }
        }
    }
}

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Create()
{
    SComplexPtr complex = SComplexPtr(new SComplex());
    // filling with "edges of tetrahedron" in R^3
    int edges[] = { 1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4 };
    int count = sizeof(edges) / (sizeof(int) * 2);
    for (int i = 0; i < count; i++)
    {
        std::set<int> simplex(&edges[i * 2], &edges[(i + 1) * 2]);
        complex->addSimplex(simplex);
    }
    return complex;
}

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Load(const char *filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    SComplexPtr complex = SComplexPtr(new SComplex());
    std::string line;
    while (getline(input, line))
    {
        if (line.find_first_of("#", 0) != std::string::npos)
        {
            continue;
        }
        std::set<int> simplex;
        std::istringstream tokens(line);
        int token;
        while(tokens >> token)
        {
            simplex.insert(token);
        }
        complex->addSimplex(simplex);
    }
    input.close();
    return complex;
}

#endif	/* SCOMPLEXFACTORY_HPP */

