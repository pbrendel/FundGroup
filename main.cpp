/*
 * File:   main.cpp
 * Author: Piotr Brendel
 */

#include "AKQReducedSComplexSupplier.hpp"
#include "FundGroup.hpp"
#include "HomologyTraits.hpp"

#include "Logger.h"

int main(int argc, char** argv)
{
    Logger::Begin(Logger::Output, "computing for cubes");
    FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > G("cubes_c.txt");
    Logger::Log(Logger::Output)<<G<<std::endl;
    Logger::End();

//    Logger::Begin(Logger::Output, "computing for general kappa map");
//    FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > H("kappa.txt");
//    Logger::Log(Logger::Output)<<H<<std::endl;
//    Logger::End();

    return 0;
}

