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

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Load(const char *filename)
{
    throw std::logic_error("loading CubSComplex not implemented yet");
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create(DebugComplexType debugComplexType)
{
    CubCellSetPtr cubSet = CubCellSetPtr(new CubCellSet(3, true));
    switch (debugComplexType)
    {
        case DCT_Sphere2:
            FillSphere2(cubSet);
            break;
        case DCT_Sphere3:
            FillSphere3(cubSet);
            break;
        case DCT_Torus:
            FillTorus(cubSet);
            break;
        case DCT_Skeleton:
            FillSkeleton(cubSet);
            break;
        case DCT_Custom0:
            FillCustom0(cubSet);
            break;
        default:
            throw std::logic_error("not implemented");
    }
    SComplexPtr complex = SComplexPtr(new SComplexType(cubSet));
    return complex;
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillSphere2(CubCellSetPtr cubSet)
{
    // filling with 2-sphere (a cube with tunnel)
    int point[3] = { 0, 0, 0 };
    for (int i = 0; i < 3; i++)
    {
        point[0] = i;
        for (int j = 0; j < 3; j++)
        {
            // leaving empty tunnel
            if (i == 1 && j == 1)
            {
                continue;
            }
            point[1] = j;
            for (int k = 0; k < 3; k++)
            {
                point[2] = k;
                cubSet().insert(point);
            }
        }
    }
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillSphere3(CubCellSetPtr cubSet)
{
    // filling with 3-sphere (an "empty" cube)
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
                if (i == 1 && j == 1 && k == 1)
                {
                    continue;
                }
                point[2] = k;
                cubSet().insert(point);
            }
        }
    }
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillTorus(CubCellSetPtr cubSet)
{
    throw std::logic_error("not implemented");
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillSkeleton(CubCellSetPtr cubSet)
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
                // leaving tunnels in each dimension empty
                if ((i == 1 && j == 1) || (i == 1 && k == 1) || (j == 1 && k == 1))
                {
                    continue;
                }
                point[2] = k;
                cubSet().insert(point);
            }
        }
    }
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillCustom0(CubCellSetPtr cubSet)
{
    throw std::logic_error("not implemented");
}

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Load(const char *filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    SComplexPtr complex = SComplexPtr(new SComplexType());
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

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Create(DebugComplexType debugComplexType)
{
    SComplexPtr complex = SComplexPtr(new SComplexType());
    switch (debugComplexType)
    {
        case DCT_Sphere2:
            FillSphere2(complex);
            break;
        case DCT_Sphere3:
            FillSphere3(complex);
            break;
        case DCT_Torus:
            FillTorus(complex);
            break;
        case DCT_Skeleton:
            FillSkeleton(complex);
            break;
        case DCT_Custom0:
            FillCustom0(complex);
            break;
        default:
            throw std::logic_error("not implemented");
    }
    return complex;
}

void SComplexFactory<SimplexSComplex>::FillSphere2(SComplexPtr complex)
{
    // filling with 2-sphere (edges of a triangle)
    int edges[] = { 1, 2, 1, 3, 2, 3 };
    int count = sizeof(edges) / (sizeof(int) * 2);
    for (int i = 0; i < count; i++)
    {
        std::set<int> simplex(&edges[i * 2], &edges[(i + 1) * 2]);
        complex->addSimplex(simplex);
    }
}

void SComplexFactory<SimplexSComplex>::FillSphere3(SComplexPtr complex)
{
    // filling with 3-sphere (an empty tetrahedron)
    int edges[] = { 1, 2, 3, 1, 2, 4, 1, 3, 4, 2, 3, 4 };
    int count = sizeof(edges) / (sizeof(int) * 3);
    for (int i = 0; i < count; i++)
    {
        std::set<int> simplex(&edges[i * 3], &edges[(i + 1) * 3]);
        complex->addSimplex(simplex);
    }
}

void SComplexFactory<SimplexSComplex>::FillTorus(SComplexPtr complex)
{
    throw std::logic_error("not implemented");
}

void SComplexFactory<SimplexSComplex>::FillSkeleton(SComplexPtr complex)
{
    // filling with "edges of tetrahedron" in R^3
    int edges[] = { 1, 2, 1, 3, 1, 4, 2, 3, 2, 4, 3, 4 };
    int count = sizeof(edges) / (sizeof(int) * 2);
    for (int i = 0; i < count; i++)
    {
        std::set<int> simplex(&edges[i * 2], &edges[(i + 1) * 2]);
        complex->addSimplex(simplex);
    }
}

void SComplexFactory<SimplexSComplex>::FillCustom0(SComplexPtr complex)
{
    throw std::logic_error("not implemented");
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::Load(const char *filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    Dims dims;
    KappaMap kappaMap;
    int topDim = 0;
    int totalCellsCount = 0;
    int cellsCountDim0 = 0;

    std::cout<<"reading SComplex"<<std::endl;
    input>>topDim;
    std::cout<<"top dim = "<<topDim<<std::endl;
    for (int dim = 0; dim <= topDim; dim++)
    {
        int cellsCount = 0;
        input>>cellsCount;
        std::cout<<cellsCount<<" in dim "<<dim<<std::endl;
        if (dim == 0)
        {
            cellsCountDim0 = cellsCount;
        }
        totalCellsCount += cellsCount;
        for (int i = 0; i < cellsCount; i++)
        {
            dims.push_back(dim);
        }
    }
    std::cout<<"total cells count = "<<totalCellsCount<<std::endl;

    for (int i = 0; i < totalCellsCount - cellsCountDim0; i++)
    {
        int cell;
        int boundary;
        int index;
        input>>cell;
        input>>boundary;
        input>>index;
        kappaMap.push_back(boost::tuple<Id, Id, int>(static_cast<Id>(cell),
                                                     static_cast<Id>(boundary),
                                                     index));
    }
    input.close();
    std::cout<<"data read successfully"<<std::endl;
    return SComplexPtr(new SComplexType(3, dims, kappaMap, 1));
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::Create(DebugComplexType debugComplexType)
{
    Dims dims;
    KappaMap kappaMap;
    switch (debugComplexType)
    {
        case DCT_Sphere2:
            FillSphere2(dims, kappaMap);
            break;
        case DCT_Sphere3:
            FillSphere3(dims, kappaMap);
            break;
        case DCT_Torus:
            FillTorus(dims, kappaMap);
            break;
        case DCT_Skeleton:
            FillSkeleton(dims, kappaMap);
            break;
        case DCT_Custom0:
            FillCustom0(dims, kappaMap);
            break;
        default:
            throw std::logic_error("not implemented");
    }
    return SComplexPtr(new SComplexType(3, dims, kappaMap, 1));
}

template <typename Traits>
void SComplexFactory<SComplex<Traits> >::FillSphere2(Dims& dims, KappaMap& kappaMap)
{
    // filling with 2-sphere (edges of a triangle)
    //                index - label
    dims.push_back(0); // 0 - [0]
    dims.push_back(0); // 1 - [1]
    dims.push_back(0); // 2 - [2]
    dims.push_back(1); // 3 - [0, 1]
    dims.push_back(1); // 4 - [1, 2]
    dims.push_back(1); // 5 - [0, 2]
    kappaMap.push_back(boost::tuple<Id, Id, int>(3, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(3, 1, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(4, 1, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(4, 2, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(5, 2, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(5, 0, 1));
}

template <typename Traits>
void SComplexFactory<SComplex<Traits> >::FillSphere3(Dims& dims, KappaMap& kappaMap)
{
    // filling with 3-sphere (an empty tetrahedron)
    //                index - label
    dims.push_back(0); // 0 - [0]
    dims.push_back(0); // 1 - [1]
    dims.push_back(0); // 2 - [2]
    dims.push_back(0); // 3 - [3]
    dims.push_back(1); // 4 - [0, 1]
    dims.push_back(1); // 5 - [0, 2]
    dims.push_back(1); // 6 - [0, 3]
    dims.push_back(1); // 7 - [1, 2]
    dims.push_back(1); // 8 - [1, 3]
    dims.push_back(1); // 9 - [2, 3]
    dims.push_back(2); // 10 - [0, 1, 2]
    dims.push_back(2); // 11 - [0, 1, 3]
    dims.push_back(2); // 12 - [0, 2, 3]
    dims.push_back(2); // 13 - [1, 2, 3]
    kappaMap.push_back(boost::tuple<Id, Id, int>(4, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(4, 1, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(5, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(5, 2, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(6, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(6, 3, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(7, 1, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(7, 2, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(8, 1, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(8, 3, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(9, 2, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(9, 3, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(10, 4, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(10, 5, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(10, 7, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(11, 4, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(11, 6, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(11, 8, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(12, 5, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(12, 6, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(12, 9, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(13, 7, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(13, 8, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(13, 9, 1));
}

template <typename Traits>
void SComplexFactory<SComplex<Traits> >::FillTorus(Dims& dims, KappaMap& kappaMap)
{
    throw std::logic_error("not implemented");
}

template <typename Traits>
void SComplexFactory<SComplex<Traits> >::FillSkeleton(Dims& dims, KappaMap& kappaMap)
{
    // filling with "edges of tetrahedron" in R^3
    dims.push_back(0); // 0 - [0]
    dims.push_back(0); // 1 - [1]
    dims.push_back(0); // 2 - [2]
    dims.push_back(0); // 3 - [3]
    dims.push_back(1); // 4 - [0, 1]
    dims.push_back(1); // 5 - [0, 2]
    dims.push_back(1); // 6 - [0, 3]
    dims.push_back(1); // 7 - [1, 2]
    dims.push_back(1); // 8 - [1, 3]
    dims.push_back(1); // 9 - [2, 3]
    kappaMap.push_back(boost::tuple<Id, Id, int>(4, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(4, 1, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(5, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(5, 2, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(6, 0, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(6, 3, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(7, 1, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(7, 2, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(8, 1, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(8, 3, 1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(9, 2, -1));
    kappaMap.push_back(boost::tuple<Id, Id, int>(9, 3, 1));
}

template <typename Traits>
void SComplexFactory<SComplex<Traits> >::FillCustom0(Dims& dims, KappaMap& kappaMap)
{
    throw std::logic_error("not implemented");
}

#endif	/* SCOMPLEXFACTORY_HPP */

