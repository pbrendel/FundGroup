/*
 * File:   main.cpp
 * Author: Piotr Brendel
 */

#include "AKQReducedSComplexSupplier.h"
#include "CollapsedAKQReducedCubSComplexSupplier.h"
#include "FundGroup.h"
#include "HomologyTraits.h"

#include "Logger.h"

int main(int argc, char** argv)
{
    Logger::Begin(Logger::Output, "computing for cubes");
    //FundGroup<CollapsedAKQReducedCubSComplexSupplier<CubicalHomology<3> > > G("cubes_c.txt");
    //FundGroup<CollapsedAKQReducedCubSComplexSupplier<CubicalHomology<3> > > G("bitmap_c_3_1.hap");
    FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > G("cubes_c.txt");
    Logger::Log(Logger::Output)<<G<<std::endl;
    Logger::End();

    //Logger::Begin(Logger::Output, "computing for cubes");
    //FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > G("cubes_c.txt");
    //Logger::Log(Logger::Output)<<G<<std::endl;
    //Logger::End();

    //Logger::Begin(Logger::Output, "computing for general kappa map");
    //FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > H("kappa.txt");
    //Logger::Log(Logger::Output)<<H<<std::endl;
    //Logger::End();

    //FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > g1(DCT_Sphere2);
    //Logger::Log(Logger::Output)<<g1<<std::endl;
    //FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > g2(DCT_Sphere2);
    //Logger::Log(Logger::Output)<<g2<<std::endl;
    //FundGroup<AKQReducedSComplexSupplier<SimplicialHomology> > g3(DCT_Sphere2);
    //Logger::Log(Logger::Output)<<g3<<std::endl;

    return 0;
}

