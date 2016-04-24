/*
 * File:   Tests.cpp
 * Author: Piotr Brendel
 */

#include "Tests.h"

#define FGLOGGER_LEVEL Details

#include "AKQReducedSComplexSupplier.h"
#include "NotReducedSComplexSupplier.h"
#include "CollapsedAKQReducedCubSComplexSupplier.h"
#include "FundGroup.h"
#include "HomologyTraits.h"

#include "FGLogger.h"

////////////////////////////////////////////////////////////////////////////////

ComplexType Tests::complexType = CT_SComplex;
ReductionType Tests::reductionType = RT_Coreductions;
std::string Tests::inputFilename = "tests.txt";

////////////////////////////////////////////////////////////////////////////////

void Tests::PrintHelp()
{
    std::cout<<std::endl;
    std::cout<<"usage: FundGroup input options"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"input:"<<std::endl;
    std::cout<<"  --i filename - use filename as input ["<<inputFilename<<"]"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"options:"<<std::endl;
    std::cout<<"  --ct       - use complex of type ["<<complexType<<"]"<<std::endl;
    std::cout<<"               - 0 - general SComplex (for all types of input)"<<std::endl;
    std::cout<<"               - 1 - simplicial complex (for list of maximal simplices)"<<std::endl;
    std::cout<<"               - 2 - cubical 2-complex (for list of maximal cubes or hap-exported bitmap)"<<std::endl;
    std::cout<<"               - 3 - cubical 3-complex (for list of maximal cubes or hap-exported bitmap)"<<std::endl;
    std::cout<<"  --rt       - use reductions of type ["<<reductionType<<"]"<<std::endl;
    std::cout<<"               - 0 - no reductions"<<std::endl;
    std::cout<<"               - 1 - shaving + coreductions"<<std::endl;
    std::cout<<"               - 2 - shaving + coreductions + collapsible subcomplex (only for cubical complexes)"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"possible input formats:"<<std::endl;
    std::cout<<"*.sim - list of maximal simplices"<<std::endl;
    std::cout<<"*.kap - kappa map"<<std::endl;
    std::cout<<"*.hap - hap-exported bitmap of cubes"<<std::endl;
    std::cout<<"other - list of maximal cubes"<<std::endl;
    std::cout<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void Tests::ProcessArgument(std::vector<std::string> &args)
{
#define CC(a, n) if (args.size() != n + 1) { std::cout<<"Error: "<<a<<" expects "<<n<<" params"<<std::endl; return; }

    std::string arg = args[0];
    if (arg == "help")
    {
        CC("help", 0)
        PrintHelp();
    }
    else if (arg == "i")
    {
        CC("i", 1)
        inputFilename = args[1];
    }
    else if (arg == "ct")
    {
        CC("ct", 1)
        complexType = static_cast<ComplexType>(atoi(args[1].c_str()));
    }
    else if (arg == "rt")
    {
        CC("rt", 1)
        reductionType = static_cast<ReductionType>(atoi(args[1].c_str()));
    }
    else
    {
        std::cout<<"Unknown argument: "<<arg<<std::endl;
    }

#undef CC
}

void Tests::ProcessArguments(int argc, char **argv)
{
    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
    {
        std::string s = std::string(argv[i]);
        if (s.size() > 2 && s[0] == '-' && s[1] == '-')
        {
            if (args.size() > 0)
            {
                ProcessArgument(args);
            }
            args.clear();
            args.push_back(s.substr(2));
        }
        else
        {
            args.push_back(s);
        }
    }
    if (args.size() > 0)
    {
        ProcessArgument(args);
    }
}

////////////////////////////////////////////////////////////////////////////////

void Tests::TestFromCommandLine(int argc, char **argv)
{
    FGLogger logger;
    logger.Begin(FGLogger::Output, "Use FundGroup --help for more info");

    ProcessArguments(argc, argv);
    logger.Log(FGLogger::Output)<<"complex type: "<<complexType<<std::endl;
    logger.Log(FGLogger::Output)<<"reduction type: "<<reductionType<<std::endl;
    logger.Log(FGLogger::Output)<<"input: "<<inputFilename<<std::endl;

    IFundGroup* fg = CreateFundGroupAlgorithm();
    logger.Log(FGLogger::Output)<<*fg<<std::endl;
    delete fg;
    logger.End();
}

IFundGroup* Tests::CreateFundGroupAlgorithm()
{
    if (complexType == CT_SComplex)
    {
        if (reductionType == RT_None)
        {
            return new FundGroup<NotReducedSComplexSupplier<SComplexHomology> >(inputFilename.c_str());
        }
        else
        {
            return new FundGroup<AKQReducedSComplexSupplier<SComplexHomology> >(inputFilename.c_str());
        }
    }
    else if (complexType == CT_Simplicial)
    {
        if (reductionType == RT_None)
        {
            return new FundGroup<NotReducedSComplexSupplier<SimplicialHomology> >(inputFilename.c_str());
        }
        else
        {
            return new FundGroup<AKQReducedSComplexSupplier<SimplicialHomology> >(inputFilename.c_str());
        }
    }
    else if (complexType == CT_Cubical_2)
    {
        if (reductionType == RT_None)
        {
            return new FundGroup<NotReducedSComplexSupplier<CubicalHomology<2> > >(inputFilename.c_str());
        }
        else if (reductionType == RT_Coreductions)
        {
            return new FundGroup<AKQReducedSComplexSupplier<CubicalHomology<2> > >(inputFilename.c_str());
        }
        else
        {
            return new FundGroup<CollapsedAKQReducedCubSComplexSupplier<CubicalHomology<2> > >(inputFilename.c_str());
        }
    }
    else
    {
        if (reductionType == RT_None)
        {
            return new FundGroup<NotReducedSComplexSupplier<CubicalHomology<3> > >(inputFilename.c_str());
        }
        else if (reductionType == RT_Coreductions)
        {
            return new FundGroup<AKQReducedSComplexSupplier<CubicalHomology<3> > >(inputFilename.c_str());
        }
        else
        {
            return new FundGroup<CollapsedAKQReducedCubSComplexSupplier<CubicalHomology<3> > >(inputFilename.c_str());
        }
    }
    return nullptr;
}
