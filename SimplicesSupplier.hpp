/*
 * File:   SimplicesSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef SIMPLICESSUPPLIER_HPP
#define	SIMPLICESSUPPLIER_HPP

#include "SimplicesSupplier.h"

#include <fstream>

template <typename T>
void SimplicesSupplier<T>::Load(const char* filename, Simplices& simplices)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    simplices.clear();
    std::string line;
    while (getline(input, line))
    {
        if (line.find_first_of("#", 0) != std::string::npos)
        {
            continue;
        }
        Simplex simplex;
        std::istringstream tokens(line);
        int token;
        while(tokens >> token)
        {
            simplex.insert(static_cast<Id>(token));
        }
        simplices.push_back(simplex);
    }
}

template <typename T>
void SimplicesSupplier<T>::Create(DebugComplexType type, Simplices& simplices)
{
    simplices.clear();
    switch (type)
    {
        case DCT_Sphere2:
            FillSphere2(simplices);
            break;
        case DCT_Sphere3:
            FillSphere3(simplices);
            break;
        case DCT_Torus:
            FillTorus(simplices);
            break;
        case DCT_Skeleton:
            FillSkeleton(simplices);
            break;
        case DCT_Custom0:
            FillCustom0(simplices);
            break;
        default:
            throw std::logic_error("not implemented");
    }
}

template <typename T>
void SimplicesSupplier<T>::FillSphere2(Simplices& simplices)
{
    // filling with 2-sphere (edges of a triangle)
    Id edges[] = { 1, 2, 1, 3, 2, 3 };
    int count = sizeof(edges) / (sizeof(int) * 2);
    for (int i = 0; i < count; i++)
    {
        Simplex simplex(&edges[i * 2], &edges[(i + 1) * 2]);
        simplices.push_back(simplex);
    }
}

template <typename T>
void SimplicesSupplier<T>::FillSphere3(Simplices& simplices)
{
    // filling with 3-sphere (an empty tetrahedron)
    Id edges[] = { 1, 2, 3, 1, 2, 4, 1, 3, 4, 2, 3, 4 };
    int count = sizeof(edges) / (sizeof(int) * 3);
    for (int i = 0; i < count; i++)
    {
        Simplex simplex(&edges[i * 3], &edges[(i + 1) * 3]);
        simplices.push_back(simplex);
    }
}

template <typename T>
void SimplicesSupplier<T>::FillTorus(Simplices& simplices)
{
    throw std::logic_error("not implemented");
}

template <typename T>
void SimplicesSupplier<T>::FillSkeleton(Simplices& simplices)
{
    // filling with "edges of tetrahedron" in R^3
    Id edges[] = { 1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4 };
    int count = sizeof(edges) / (sizeof(int) * 2);
    for (int i = 0; i < count; i++)
    {
        Simplex simplex(&edges[i * 2], &edges[(i + 1) * 2]);
        simplices.push_back(simplex);
    }
}

template <typename T>
void SimplicesSupplier<T>::FillCustom0(Simplices& simplices)
{
    throw std::logic_error("not implemented");
}

#endif	/* SIMPLICESSUPPLIER_HPP */

