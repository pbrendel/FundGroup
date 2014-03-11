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
    //FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > G(argv[1]);
    FundGroup<NotReducedSComplexSupplier<CubicalHomology<3> > > G(argv[1]);
    //FundGroup<CollapsedAKQReducedCubSComplexSupplier<CubicalHomology<3> > > G(argv[1]);
    logger.Log(FGLogger::Output)<<G<<std::endl;
    G.ExportHapProgram("fundgroup.g");
    logger.End();

    //FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > g1(DCT_S1);
    //logger.Log(FGLogger::Output)<<g1<<std::endl;
    //FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > g2(DCT_S1);
    //logger.Log(FGLogger::Output)<<g2<<std::endl;
    //FundGroup<AKQReducedSComplexSupplier<SimplicialHomology> > g3(DCT_S1);
    //logger.Log(FGLogger::Output)<<g3<<std::endl;

    return 0;
}

