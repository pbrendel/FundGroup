/*
 * File:   main.cpp
 * Author: Piotr Brendel
 */

#include <iostream>

#include "AKQReducedSComplexSupplier.hpp"
#include "CoreducedSComplexSupplier.hpp"
#include "DebugComplexType.h"
#include "FundGroup.hpp"
#include "HomologyTraits.hpp"
#include "SComplexFactory.hpp"

int main(int argc, char** argv)
{
//    FundGroup<CoreducedSComplexSupplier<SimplicialHomology> > G1(DCT_Sphere2);
//    std::cout<<G1<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<SimplicialHomology> > G2(DCT_Sphere3);
//    std::cout<<G2<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<SimplicialHomology> > G3(DCT_Skeleton);
//    std::cout<<G3<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<CubicalHomology<3> > > G4(DCT_Sphere2);
//    std::cout<<G4<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<CubicalHomology<3> > > G5(DCT_Sphere3);
//    std::cout<<G5<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<CubicalHomology<3> > > G6(DCT_Skeleton);
//    std::cout<<G6<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<SComplexHomology> > G7(DCT_Sphere2);
//    std::cout<<G7<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<SComplexHomology> > G8(DCT_Sphere3);
//    std::cout<<G8<<std::endl;
//    FundGroup<CoreducedSComplexSupplier<SComplexHomology> > G9(DCT_Skeleton);
//    std::cout<<G9<<std::endl;
//    FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G10(DCT_Sphere2);
//    std::cout<<G10<<std::endl;
//    FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G11(DCT_Sphere3);
//    std::cout<<G11<<std::endl;
//    FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G12(DCT_Skeleton);
//    std::cout<<G12<<std::endl;

    FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G13("kappa.txt");
    std::cout<<G13<<std::endl;

//    if (argc > 1)
//    {
//        std::cout<<argv[1]<<std::endl;
//        FundGroup<CoreducedSComplexSupplier<SimplicialHomology> > G1(argv[1]);
//        std::cout<<G1<<std::endl;
//        FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G2(DCT_Sphere2);
//        std::cout<<G2<<std::endl;
//    }
//    else
//    {
//        FundGroup<CoreducedSComplexSupplier<CubicalHomology<3> > > G1(DCT_Sphere3);
//        std::cout<<G1<<std::endl;
//        FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G2(argv[1]);
//        std::cout<<G2<<std::endl;
//    }
    return 0;
}

