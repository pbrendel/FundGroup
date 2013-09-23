/*
 * File:   HomologyHelpers.hpp
 * Author: Piotr Brendel
 */

#ifndef HOMOLOGYHELPERS_HPP
#define	HOMOLOGYHELPERS_HPP

#include "HomologyHelpers.h"

#include <redHom/algorithm/Algorithms.hpp>

template <typename Traits>
typename HomologyHelpers<Traits>::FilterPtr
HomologyHelpers<Traits>::CreateFilter(typename HomologyHelpers<Traits>::SComplexPtr complex,
                                      bool useShaving,
                                      bool useCoreductions,
                                      bool restore)
{
    typedef typename ShaveAlgorithmFactory<SComplex>::DefaultAlgorithm ShaveAlgorithm;
    typedef typename CoreductionAlgorithmFactory<SComplex, Scalar>::DefaultAlgorithm CoreductionAlgorithm;
    typename Filter::ReducedCells reducedCellsS, reducedCellsC;
    typename Filter::ExtractedCells extractedCells;

    if (useShaving)
    {
        boost::shared_ptr<ShaveAlgorithm> salgorithm = ShaveAlgorithmFactory<SComplex>::createDefault(*complex);
        salgorithm->setStoreReducedCells(true);
        (*salgorithm)();
        reducedCellsS = salgorithm->getReducedCells();
    }

    if (useCoreductions)
    {
        boost::shared_ptr<CoreductionAlgorithm> algorithm = CoreductionAlgorithmFactory<SComplex, Scalar>::createDefault(*complex);
        algorithm->setStoreReducedCells(true);
        (*algorithm)();
        extractedCells = algorithm->getExtractedCells();
        reducedCellsC = algorithm->getReducedCells();
    }

    FilterPtr homFilter(new Filter(*complex, extractedCells, reducedCellsS, reducedCellsC));
    (*homFilter)(true, restore);
    return homFilter;
}

template <typename Traits>
std::vector<int>
HomologyHelpers<Traits>::GetBettiNumbers(typename HomologyHelpers<Traits>::FilterPtr filter)
{
    std::vector<int> bettiNumbers;
    for (int i = 0; i <= filter->topDim(); i++)
    {
        bettiNumbers.push_back(filter->baseHomologyChains()[i].size());
    }
    return bettiNumbers;
}

template <typename Traits>
std::vector<int>
HomologyHelpers<Traits>::GetBettiNumbers(typename HomologyHelpers<Traits>::SComplexPtr complex)
{
    return GetBettiNumbers(CreateFilter(complex, false, false, false));
}

#endif	/* HOMOLOGYHELPERS_HPP */

