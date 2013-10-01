/*
 * File:   main.cpp
 * Author: Piotr Brendel
 */

#include <ctime>
#include <iostream>

#include "AKQReducedSComplexSupplier.hpp"
#include "FundGroup.hpp"
#include "HomologyTraits.hpp"

int main(int argc, char** argv)
{
    std::cout<<"computing for cubes"<<std::endl;
    time_t now = time(0);
    FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > > G("cubes_m.txt");
    std::cout<<G<<std::endl;
    std::cout<<"finished in "<<(time(0) - now)<<" seconds"<<std::endl;

    std::cout<<"computing for general kappa map"<<std::endl;
    now = time(0);
    FundGroup<AKQReducedSComplexSupplier<SComplexHomology> > H("kappa.txt");
    std::cout<<H<<std::endl;
    std::cout<<"finished in "<<(time(0) - now)<<" seconds"<<std::endl;

    return 0;
}

