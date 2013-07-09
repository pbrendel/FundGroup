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
    FundGroup<HomologyTraits<3> > f;
    std::cout<<f<<std::endl;
    return 0;
}

