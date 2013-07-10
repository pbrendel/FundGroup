/*
 * File:   main.cpp
 * Author: Piotr Brendel
 */

#include <iostream>

#include "FundGroup.hpp"
#include "HomologyTraits.hpp"
#include "SComplexFactory.hpp"

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        std::cout<<argv[1]<<std::endl;
        FundGroup<SimplicialHomology> G(argv[1]);
        std::cout<<G<<std::endl;
    }
    else
    {
        FundGroup<CubicalHomology<3> > G;
        std::cout<<G<<std::endl;
    }
    return 0;
}

