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
#include <capd/repSet/ECellMDCodeT.h>

#include "Logger.h"

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Load(const char* filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    Logger::Begin(Logger::Details, "parsing data");
    Cubes cubes;
    BitmapSizes sizes(DIM);
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
            sizes[index].Update(coord);
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

    return Create(cubes, sizes);
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create(DebugComplexType debugComplexType)
{
    Cubes cubes;
    BitmapSizes sizes;
    switch (debugComplexType)
    {
        case DCT_Sphere2:
            FillSphere2(cubes, sizes);
            break;
        case DCT_Sphere3:
            FillSphere3(cubes, sizes);
            break;
        case DCT_Torus:
            FillTorus(cubes, sizes);
            break;
        case DCT_Skeleton:
            FillSkeleton(cubes, sizes);
            break;
        case DCT_Custom0:
            FillCustom0(cubes, sizes);
            break;
        default:
            throw std::logic_error("not implemented");
    }
    return Create(cubes, sizes);
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::Create(Cubes& cubes, BitmapSizes& sizes)
{
    //return CreateWithCubCellSet(cubes, sizes, true);
    return CreateWithCubSet(cubes, sizes, true);
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::CreateWithCubCellSet(Cubes& cubes, BitmapSizes& sizes, bool shave)
{
    // recalculating into RedHom CubCellSet internal format
    Logger::Begin(Logger::Details, "Creating CubCellSet");
    std::vector<int> cubCellSetSizes(DIM);
    for (int i = 0; i < DIM; i++)
    {
        Logger::Log(Logger::Debug)<<sizes[i].Size()<<std::endl;
        cubCellSetSizes[i] = 2 * sizes[i].Size() + 1;
    }
    CubCellSetPtr cubCellSet = CubCellSetPtr(new CubCellSet(&cubCellSetSizes[0], true));

    // renormalizing and adding cubes
    int cube[DIM];
    size_t count = cubes.size();
    for (size_t i = 0; i < count; i++)
    {
        Cube c = cubes[i];
        for (size_t j = 0; j < DIM; j++)
        {
            // recalculating into RedHom CubCellSet internal format
            cube[j] = 2 * c[j] - sizes[j]._min + 1;
        }
        cubCellSet().insert(&cube[0]);
    }
    Logger::End();

    Logger::Begin(Logger::Details, "creating lower dimensional cubes");
    cubCellSet().fillWithSubEmbDimCells();
    Logger::End();

    if (shave)
    {
        Logger::Begin(Logger::Details, "shaving");
        count = static_cast<size_t>(cubCellSet().shave());
        if (Logger::PrintShavedCellsCount())
        {
            Logger::Log(Logger::Details)<<"shaved "<<count<<" cubes"<<std::endl;
        }
        Logger::End();
    }

    Logger::Begin(Logger::Details, "creating SComplex");
    SComplexPtr complex = SComplexPtr(new SComplexType(cubCellSet));
    Logger::End();

    return complex;
}

template <int DIM>
typename SComplexFactory<CubSComplex<DIM> >::SComplexPtr
SComplexFactory<CubSComplex<DIM> >::CreateWithCubSet(Cubes& cubes, BitmapSizes& sizes, bool shave)
{
   // recalculating into RedHom CubSet internal format
    Logger::Begin(Logger::Details, "Creating CubSet");
    std::vector<int> cubSetSizes(DIM);
    for (int i = 0; i < DIM; i++)
    {
        Logger::Log(Logger::Debug)<<sizes[i].Size()<<std::endl;
        cubSetSizes[i] = sizes[i].Size();
    }

    CubSetPtr cubSet = CubSetPtr(new CubSet(&cubSetSizes[0]));

    // renormalizing and adding cubes
    int cube[DIM];
    size_t count = cubes.size();
    for (size_t i = 0; i < count; i++)
    {
        Cube c = cubes[i];
        for (size_t j = 0; j < DIM; j++)
        {
            // recalculating into RedHom CubSet internal format
            cube[j] = c[j] - sizes[j]._min;
        }
        cubSet().insert(&cube[0]);
    }
    cubSet().addEmptyCollar();
    Logger::End();

    if (shave)
    {
        Logger::Begin(Logger::Details, "shaving");
    //    cubSet().shaveBI();
        if (Logger::PrintShavedCellsCount())
        {
            count = count - static_cast<size_t>(cubSet().cardinality());
            Logger::Log(Logger::Details)<<"shaved "<<count<<" cubes"<<std::endl;
        }
        Logger::End();
    }

    Logger::Begin(Logger::Details, "creating CubCellSet");
    CubCellSetPtr cubCellSet = CubCellSetPtr(new CubCellSet(cubSet()));
    Logger::End();

    Logger::Begin(Logger::Details, "creating SComplex");
    SComplexPtr complex = SComplexPtr(new SComplexType(cubCellSet));
    Logger::End();

    return complex;


}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillSphere2(Cubes& cubes, BitmapSizes& sizes)
{
    assert(DIM == 3);
    sizes.push_back(BitmapSize(3));
    sizes.push_back(BitmapSize(3));
    sizes.push_back(BitmapSize(3));
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

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillSphere3(Cubes& cubes, BitmapSizes& sizes)
{
    assert(DIM == 3);
    sizes.push_back(BitmapSize(3));
    sizes.push_back(BitmapSize(3));
    sizes.push_back(BitmapSize(3));
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

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillTorus(Cubes& cubes, BitmapSizes& sizes)
{
    throw std::logic_error("not implemented");
}

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillSkeleton(Cubes& cubes, BitmapSizes& sizes)
{
    assert(DIM == 3);
    sizes.push_back(BitmapSize(3));
    sizes.push_back(BitmapSize(3));
    sizes.push_back(BitmapSize(3));
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

template <int DIM>
void SComplexFactory<CubSComplex<DIM> >::FillCustom0(Cubes& cubes, BitmapSizes& sizes)
{
    throw std::logic_error("not implemented");
}

typename SComplexFactory<SimplexSComplex>::SComplexPtr
SComplexFactory<SimplexSComplex>::Load(const char* filename)
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
SComplexFactory<SComplex<Traits> >::Load(const char* filename)
{
    FileType fileType = DetermineFileType(filename);
    switch (fileType)
    {
        case FT_Cubes:
            return LoadCubes(filename);
        case FT_Simplices:
            return LoadSimplices(filename);
        case FT_KappaMap:
        default:
            return LoadKappaMap(filename);
    }
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::FileType
SComplexFactory<SComplex<Traits> >::DetermineFileType(const char* filename)
{
    // simple but error-prone determining file type by its extension

    size_t len = strlen(filename);

    if (len < 5)
    {
        // cannot determine by its extension
        // assuming default - Kappa map format
        return FT_KappaMap;
    }

    if (   (tolower(filename[len - 3]) == 'c')
        && (tolower(filename[len - 3]) == 'u')
        && (tolower(filename[len - 1]) == 'b') )
    {
        return FT_Cubes;
    }

    if (   (tolower(filename[len - 3]) == 's')
        && (tolower(filename[len - 2]) == 'i')
        && (tolower(filename[len - 1]) == 'm') )
    {
        return FT_Simplices;
    }

    return FT_KappaMap;
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::LoadKappaMap(const char* filename)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        throw std::runtime_error(std::string("cannot open file ") + filename);
    }

    Dims dims;
    KappaMap kappaMap;
    size_t topDim = 0;
    size_t totalCellsCount = 0;
    size_t kappaMapSize = 0;

    Logger::Begin(Logger::Details, "reading SComplex as kappa map");
    input>>topDim;
    Logger::Log(Logger::Details)<<"top dim: "<<topDim<<std::endl;
    for (int dim = 0; dim <= topDim; dim++)
    {
        int cellsCount = 0;
        input>>cellsCount;
        Logger::Log(Logger::Details)<<cellsCount<<" cells in dim "<<dim<<std::endl;
        totalCellsCount += cellsCount;
        for (int i = 0; i < cellsCount; i++)
        {
            dims.push_back(dim);
        }
        // for every cell there is 2 boundary cells in each dimension
        kappaMapSize += dim * cellsCount * 2;
    }
    Logger::Log(Logger::Details)<<"total cells count = "<<totalCellsCount<<std::endl;

    for (int i = 0; i < kappaMapSize; i++)
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
    Logger::End("data read successfully");
    return SComplexPtr(new SComplexType(3, dims, kappaMap, 1));
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::LoadCubes(const char* filename)
{
    throw std::logic_error("not implemented");
}

template <typename Traits>
typename SComplexFactory<SComplex<Traits> >::SComplexPtr
SComplexFactory<SComplex<Traits> >::LoadSimplices(const char* filename)
{
    throw std::logic_error("not implemented");
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

