/*
 * File:   CubesSupplier.hpp
 * Author: Piotr Brendel
 */

#ifndef CUBESSUPPLIER_HPP
#define	CUBESSUPPLIER_HPP

#define USE_MEMORY_STREAM

#include "CubesSupplier.h"

#include <fstream>
#include <limits>
#include <sstream>
#ifdef USE_MEMORY_STREAM
#include <cstdio>
#endif

#include "FGLogger.h"

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
#ifdef USE_MEMORY_STREAM
    FILE* fp = fopen(filename, "rb");
    if (fp == 0)
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }
    fseek(fp, 0, SEEK_END);
    long int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buffer = new char[size];
    fread(buffer, sizeof(unsigned char), size, fp);
    fclose(fp);
    std::stringbuf inputBuf(std::ios_base::in);
    inputBuf.pubsetbuf(buffer, static_cast<std::streamsize>(size));
    std::istream input(&inputBuf);
#else
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }
#endif

    cubes.clear();
    bounds.clear();
    bounds.resize(DIM);

    FGLogger logger;
    logger.Begin(FGLogger::Details, "parsing data");

    FileType type = DetermineFileType(filename);
    switch (type)
    {
        case FT_HapBitmap:
            ParseHapBitmap(input, cubes, bounds);
            break;
        default:
            ParseFullCubes(input, cubes, bounds);
            break;
    }

#ifdef USE_MEMORY_STREAM
    delete [] buffer;
#else
    input.close();
#endif
    logger.End();
    logger.Log(FGLogger::Details)<<"parsed "<<cubes.size()<<" cubes"<<std::endl;
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
void CubesSupplier<T, DIM>::ParseFullCubes(std::istream& str, Cubes& cubes, Bounds& bounds)
{
    std::string line;
    while (getline(str, line))
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
            //std::cout<<cubes.size()<<std::endl;
            //assert(std::find(cubes.begin(), cubes.end(), cube) == cubes.end());
            cubes.push_back(cube);
        }
    }
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::ParseHapBitmap(istream& str, Cubes& cubes, Bounds& bounds)
{
    int currentDim = -1;
    Cube currentCube;
    int eof = std::char_traits<char>::eof();
    int c = 0;

    while ((c = str.get()) != eof)
    {
        if (c == '[')
        {
            currentDim++;
            while (currentDim > static_cast<int>(currentCube.size()) - 1)
            {
                bounds[currentCube.size()].Update(0);
                currentCube.push_back(0);
            }
            currentCube[currentDim] = 0;
        }
        else if (c == ']')
        {
            currentDim--;
        }
        else if (c == ',')
        {
            currentCube[currentDim]++;
        }
        else if (c == '1')
        {
            for (int i = 0; i < currentCube.size(); i++)
            {
                bounds[i].Update(currentCube[i]);
            }
            cubes.push_back(currentCube);
        }
        else
        {
            continue;
        }
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
            for (Coord k = 1; k < 2; k++)
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
    Cubes cubesTmp;
    Bounds boundsTmp;
    FillSphere2(cubesTmp, boundsTmp);
    CreateComplement(cubesTmp, boundsTmp, cubes, bounds);
}

template <typename T, int DIM>
void CubesSupplier<T, DIM>::CreateComplement(Cubes& cubesIn, Bounds& boundsIn,
                                             Cubes& cubesOut, Bounds& boundsOut)
{
    assert(boundsIn.size() == DIM);
    std::vector<T> coords(boundsIn.size());
    boundsOut.resize(boundsIn.size());
    size_t totalCount = 1;
    for (size_t i = 0; i < boundsIn.size(); i++)
    {
        boundsOut[i] = Bound(boundsIn[i]._min - 1, boundsIn[i]._max + 1);
        totalCount *= boundsOut[i].Size();
        coords[i] = boundsOut[i]._min;
    }
    cubesOut.clear();
    for (size_t i = 0; i < totalCount; i++)
    {
        // if cube is not found in the original set -> add it to its complement
        if (std::find(cubesIn.begin(), cubesIn.end(), coords) == cubesIn.end())
        {
            cubesOut.push_back(coords);
        }
        // increment coord
        size_t dim = 0;
        bool ok = false;
        while (!ok && dim < boundsOut.size())
        {
            coords[dim]++;
            if (coords[dim] <= boundsOut[dim]._max)
            {
                ok = true;
            }
            else
            {
                coords[dim] = boundsOut[dim]._min;
                dim++;
            }
        }
    }
}

template <typename T, int DIM>
typename CubesSupplier<T, DIM>::FileType
CubesSupplier<T, DIM>::DetermineFileType(const char* filename)
{
    // simple but error-prone determining file type by its extension

    size_t len = strlen(filename);

    if (len < 5)
    {
        // cannot determine by its extension
        // assuming default - list of full cubes
        return FT_FullCubes;
    }

    if (   (tolower(filename[len - 3]) == 'h')
        && (tolower(filename[len - 2]) == 'a')
        && (tolower(filename[len - 1]) == 'p') )
    {
        return FT_HapBitmap;
    }

    return FT_FullCubes;
}

#endif	/* CUBESSUPPLIER_HPP */

