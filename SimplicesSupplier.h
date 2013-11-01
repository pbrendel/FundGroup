/*
 * File:   SimplicesSupplier.h
 * Author: Piotr Brendel
 */

#ifndef SIMPLICESSUPPLIER_H
#define	SIMPLICESSUPPLIER_H

#include <set>
#include <vector>

#include "DebugComplexType.h"

template <typename T>
class SimplicesSupplier
{
public:

    typedef T                       Id;
    typedef std::set<Id>            Simplex;
    typedef std::vector<Simplex>    Simplices;

    static void Load(const char* filename, Simplices& simplices);
    static void Create(DebugComplexType type, Simplices& simplices);

private:

    static void FillSphere2(Simplices& simplices);
    static void FillSphere3(Simplices& simplices);
    static void FillTorus(Simplices& simplices);
    static void FillSkeleton(Simplices& simplices);
    static void FillCustom0(Simplices& simplices);
};

#include "SimplicesSupplier.hpp"

#endif	/* SIMPLICESSUPPLIER_H */

