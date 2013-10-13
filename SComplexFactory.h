/*
 * File:   SComplexFactory.h
 * Author: Piotr Brendel
 */

#ifndef SCOMPLEXFACTORY_H
#define	SCOMPLEXFACTORY_H

#include <limits>
#include <boost/shared_ptr.hpp>

#include "DebugComplexType.h"

#include <redHom/complex/cubical/CubSComplex.hpp>
#include <redHom/complex/simplicial/SimplexSComplex.hpp>

template <typename SComplexType>
class SComplexFactory
{
};

template <int DIM>
class SComplexFactory<CubSComplex<DIM> >
{
public:

    typedef CubSComplex<DIM>                SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    template<typename C>
    struct BitmapSizeT
    {
        C _min;
        C _max;

        BitmapSizeT()
            : _min(std::numeric_limits<C>::max())
            , _max(std::numeric_limits<C>::min())
        {}

        BitmapSizeT(C size)
            : _min(0)
            , _max(size)
        {}

        BitmapSizeT(C min, C max)
            : _min(min)
            , _max(max)
        {}

        void Update(C coord)
        {
            _min = std::min(coord, _min);
            _max = std::max(coord, _max);
        }

        size_t Size()
        {
            if (_max < _min)
            {
                return 0;
            }
            return static_cast<size_t>(_max - _min + 1);
        }
    };

    typedef size_t                                  Coord;
    typedef std::vector<Coord>                      Cube;
    typedef std::vector<Cube>                       Cubes;
    typedef BitmapSizeT<Coord>                      BitmapSize;
    typedef std::vector<BitmapSize>                 BitmapSizes;
    typedef typename CubSComplex<DIM>::BCubCellSet  CubCellSet;
    typedef CRef<CubCellSet>                        CubCellSetPtr;
    typedef typename CubCellSet::EuclSet            EuclSet;
    typedef CubSetT<EuclSet>                        CubSet;
    typedef CRef<CubSet>                            CubSetPtr;

    static SComplexPtr Create(Cubes& cubes, BitmapSizes& sizes);
    static SComplexPtr CreateWithCubCellSet(Cubes& cubes, BitmapSizes& sizes, bool shave);
    static SComplexPtr CreateWithCubSet(Cubes& cubes, BitmapSizes& sizes, bool shave);

    static void FillSphere2(Cubes& cubes, BitmapSizes& sizes);
    static void FillSphere3(Cubes& cubes, BitmapSizes& sizes);
    static void FillTorus(Cubes& cubes, BitmapSizes& sizes);
    static void FillSkeleton(Cubes& cubes, BitmapSizes& sizes);
    static void FillCustom0(Cubes& cubes, BitmapSizes& sizes);
};

template <>
class SComplexFactory<SimplexSComplex>
{
public:

    typedef SimplexSComplex                 SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    static void FillSphere2(SComplexPtr complex);
    static void FillSphere3(SComplexPtr complex);
    static void FillTorus(SComplexPtr complex);
    static void FillSkeleton(SComplexPtr complex);
    static void FillCustom0(SComplexPtr complex);
};

template <typename Traits>
class SComplexFactory<SComplex<Traits> >
{
public:

    typedef SComplex<Traits>                SComplexType;
    typedef boost::shared_ptr<SComplexType> SComplexPtr;

    static SComplexPtr Load(const char* filename);
    static SComplexPtr Create(DebugComplexType debugComplexType);

private:

    typedef typename SComplexType::Id       Id;
    typedef typename SComplexType::Dims     Dims;
    typedef typename SComplexType::KappaMap KappaMap;

    enum FileType
    {
        FT_Unknown,
        FT_KappaMap,
        FT_Cubes,
        FT_Simplices,
    };

    static FileType DetermineFileType(const char* filename);

    static SComplexPtr LoadKappaMap(const char* filename);
    static SComplexPtr LoadCubes(const char* filename);
    static SComplexPtr LoadSimplices(const char* filename);

    static void FillSphere2(Dims& dims, KappaMap& kappaMap);
    static void FillSphere3(Dims& dims, KappaMap& kappaMap);
    static void FillTorus(Dims& dims, KappaMap& kappaMap);
    static void FillSkeleton(Dims& dims, KappaMap& kappaMap);
    static void FillCustom0(Dims& dims, KappaMap& kappaMap);
};

#endif	/* SCOMPLEXFACTORY_H */
