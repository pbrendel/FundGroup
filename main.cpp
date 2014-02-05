/*
 * File:   main.cpp
 * Author: Piotr Brendel
 */

#define FGLOGGER_LEVEL Details

#include "AKQReducedSComplexSupplier.h"
#include "NotReducedSComplexSupplier.h"
#include "CollapsedAKQReducedCubSComplexSupplier.h"
#include "FundGroup.h"
#include "HomologyTraits.h"

#include "FGLogger.h"

int main(int argc, char** argv)
{
    FGLogger logger;
    logger.Begin(FGLogger::Output, "computing for cubes");
    FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > G("cubes_m.txt");
    logger.Log(FGLogger::Output)<<G<<std::endl;
    logger.End();

    //logger.Begin(FGLogger::Output, "computing for cubes");
    //FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > G("cubes_c.txt");
    //logger.Log(FGLogger::Output)<<G<<std::endl;
    //logger.End();

    //logger.Begin(FGLogger::Output, "computing for general kappa map");
    //FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > H("kappa.txt");
    //logger.Log(FGLogger::Output)<<H<<std::endl;
    //logger.End();

    //FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > g1(DCT_Sphere2);
    //logger.Log(FGLogger::Output)<<g1<<std::endl;
    //FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > g2(DCT_Sphere2);
    //logger.Log(FGLogger::Output)<<g2<<std::endl;
    //FundGroup<AKQReducedSComplexSupplier<SimplicialHomology> > g3(DCT_Sphere2);
    //logger.Log(FGLogger::Output)<<g3<<std::endl;

    return 0;
}

