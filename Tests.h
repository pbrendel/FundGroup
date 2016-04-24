/*
 * File:   Tests.h
 * Author: Piotr Brendel
 */

#ifndef TESTS_H
#define	TESTS_H

#include <vector>
#include <string>

enum ComplexType
{
    CT_SComplex,
    CT_Simplicial,
    CT_Cubical_2,
    CT_Cubical_3,
};

enum ReductionType
{
    RT_None,
    RT_Coreductions,
    RT_CoreductionsCollapsible,
};

class Tests
{
public:

    static void TestFromCommandLine(int, char **);

private:

    static ComplexType      complexType;
    static ReductionType    reductionType;
    static std::string      inputFilename;

    static void PrintHelp();
    static void ProcessArgument(std::vector<std::string> &args);
    static void ProcessArguments(int, char **);

    static void Test();
    static class IFundGroup* CreateFundGroupAlgorithm();
};

#endif	/* TESTS_H */

