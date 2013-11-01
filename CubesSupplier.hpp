/*
 * File:   CubesSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef CUBESSUPPLIER_HPP
#define	CUBESSUPPLIER_HPP

#include "CubesSupplier.h"

#include <fstream>
#include <limits>
#include <sstream>

#include "Logger.h"

////////////////////////////////////////////////////////////////////////////////

template <typename T, int DIM>
CubesSupplier<T, DIM>::Bound::Bound()
    : _min(std::numeric_limits<T>::max())
    , _max(std::numeric_limits<T>::min())
{}

template <typename T, int DIM>
CubesSupplier<T, DIM>::Bound::Bound(T size)
    : _min(0)
    , _max(size)
{}

template <typename T, int DIM>
CubesSupplier<T, DIM>::Bound::Bound(T min, T max)
    : _min(min)
    , _max(max)
{}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::Bound::Update(T coord)
{
    _min = std::min(coord, _min);
    _max = std::max(coord, _max);
}

template <typename T, int DIM>
size_t CubesSupplier<T, DIM>::Bound::Size()
{
    if (_max < _min)
    {
        return 0;
    }
    return static_cast<size_t>(_max - _min + 1);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T, int DIM>
void CubesSupplier<T, DIM>::Load(const char* filename, Cubes& cubes, Bounds& bounds)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    cubes.clear();
    bounds.clear();
    bounds.resize(DIM);
    Logger::Begin(Logger::Details, "parsing data");
    std::string line;
    while (getline(input, line))
    {
        if (line.find_first_of("#", 0) != std::string::npos)
        {
            continue;
        }
        std::istringstream tokens(line);
        Cube cube;
        Coord coord;
        while(tokens >> coord)
        {
            int index = std::min(static_cast<int>(cube.size()), DIM - 1);
            bounds[index].Update(coord);
            cube.push_back(coord);
        }
        if (cube.size() == DIM)
        {
            cubes.push_back(cube);
        }
    }
    input.close();
    Logger::End();
    Logger::Log(Logger::Details)<<"parsed "<<cubes.size()<<" cubes"<<std::endl;
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::Create(DebugComplexType type, Cubes& cubes, Bounds& bounds)
{
    cubes.clear();
    bounds.clear();
    switch (type)
    {
        case DCT_Sphere2:
            FillSphere2(cubes, bounds);
            break;
        case DCT_Sphere3:
            FillSphere3(cubes, bounds);
            break;
        case DCT_Torus:
            FillTorus(cubes, bounds);
            break;
        case DCT_Skeleton:
            FillSkeleton(cubes, bounds);
            break;
        case DCT_Custom0:
            FillCustom0(cubes, bounds);
            break;
        default:
            throw std::logic_error("not implemented");
    }
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::FillSphere2(Cubes& cubes, Bounds& bounds)
{
    assert(DIM == 3);
    bounds.push_back(Bound(3));
    bounds.push_back(Bound(3));
    bounds.push_back(Bound(3));
    // filling with 2-sphere (a cube with tunnel)
    Cube cube(3);
    for (Coord i = 0; i < 3; i++)
    {
        cube[0] = i;
        for (Coord j = 0; j < 3; j++)
        {
            // leaving empty tunnel
            if (i == 1 && j == 1)
            {
                continue;
            }
            cube[1] = j;
            for (Coord k = 0; k < 3; k++)
            {
                cube[2] = k;
                cubes.push_back(cube);
            }
        }
    }
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::FillSphere3(Cubes& cubes, Bounds& bounds)
{
    assert(DIM == 3);
    bounds.push_back(Bound(3));
    bounds.push_back(Bound(3));
    bounds.push_back(Bound(3));
    // filling with 3-sphere (an "empty" cube)
    Cube cube(DIM);
    for (Coord i = 0; i < 3; i++)
    {
        cube[0] = i;
        for (Coord j = 0; j < 3; j++)
        {
            cube[1] = j;
            for (Coord k = 0; k < 3; k++)
            {
                // leaving center empty
                if (i == 1 && j == 1 && k == 1)
                {
                    continue;
                }
                cube[2] = k;
                cubes.push_back(cube);
            }
        }
    }
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::FillTorus(Cubes& cubes, Bounds& bounds)
{
    throw std::logic_error("not implemented");
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::FillSkeleton(Cubes& cubes, Bounds& bounds)
{
    assert(DIM == 3);
    bounds.clear();
    bounds.push_back(Bound(3));
    bounds.push_back(Bound(3));
    bounds.push_back(Bound(3));
    // filling with "edges of cube" in R^3
    Cube cube(DIM);
    for (Coord i = 0; i < 3; i++)
    {
        cube[0] = i;
        for (Coord j = 0; j < 3; j++)
        {
            cube[1] = j;
            for (Coord k = 0; k < 3; k++)
            {
                // leaving tunnels in each dimension empty
                if ((i == 1 && j == 1) || (i == 1 && k == 1) || (j == 1 && k == 1))
                {
                    continue;
                }
                cube[2] = k;
                cubes.push_back(cube);
            }
        }
    }
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::FillCustom0(Cubes& cubes, Bounds& bounds)
{
    throw std::logic_error("not implemented");
}

#endif	/* CUBESSUPPLIER_HPP */

