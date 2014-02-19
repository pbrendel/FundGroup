/*
 * File:   HapCWComplexExporter.hpp
 * Author: Piotr Brendel
 */

#ifndef HAPCWCOMPLEXEXPORTER_HPP
#define	HAPCWCOMPLEXEXPORTER_HPP

#include "HapCWComplexExporter.h"

#include <fstream>

template <typename ComplexType>
void HapCWComplexExporter<ComplexType>::CollectComplexData(ComplexPtr complex)
{
    _maxDim = complex->getDim();

    _boundaries.resize(_maxDim + 1);
    _coboundaries.resize(_maxDim + 1);
    _orientation.resize(_maxDim + 1);
    _idIndexMapByDim.resize(_maxDim + 1);
    _vectorField.resize(_maxDim + 1);
    _inverseVectorField.resize(_maxDim + 1);

    std::vector<size_t> cellsCountByDim(_maxDim + 1);
    std::vector<size_t> cellsCountInLowerDims(_maxDim + 1);

    for (int i = 0; i <= _maxDim; i++)
    {
        DimCells dimCells = complex->iterators().dimCells(i);
        size_t cellsCount = 0;
        typename DimCells::iterator dimIt = dimCells.begin();
        typename DimCells::iterator dimItEnd = dimCells.end();
        for ( ; dimIt != dimItEnd; ++dimIt)
        {
            _idIndexMapByDim[i][dimIt->getId()] = cellsCount;
            cellsCount++;
        }
        cellsCountByDim[i] = cellsCount;
        if (i > 0)
        {
            cellsCountInLowerDims[i] = cellsCountInLowerDims[i - 1] + cellsCount;
        }
        _boundaries[i].resize(cellsCount);
        _coboundaries[i].resize(cellsCount);
        _orientation[i].resize(cellsCount);
        _vectorField[i].assign(cellsCount, -1);
        _inverseVectorField[i].assign(cellsCount, -1);
    }

    AllCells allCells = complex->iterators().allCells();
    typename AllCells::iterator it = allCells.begin();
    typename AllCells::iterator itEnd = allCells.end();
    for ( ; it != itEnd; ++it)
    {
        Dim dim = it->getDim();
        size_t myIndex = _idIndexMapByDim[dim][it->getId()];
        assert(dim >= 0 && dim <= _maxDim);
        if (dim > 0)
        {
            BdCells bdCells = complex->iterators().bdCells(*it);
            typename BdCells::iterator bdIt = bdCells.begin();
            typename BdCells::iterator bdItEnd = bdCells.end();
            for ( ; bdIt != bdItEnd; ++bdIt)
            {
                assert(bdIt->getDim() == dim - 1);
                size_t bdIndex = _idIndexMapByDim[dim - 1][bdIt->getId()];
                _boundaries[dim][myIndex].push_back(bdIndex + 1);
                _coboundaries[dim - 1][bdIndex].push_back(myIndex + 1);
                _orientation[dim][myIndex].push_back(complex->coincidenceIndex(*it, *bdIt));
            }
        }
    }
}

template <typename ComplexType>
void HapCWComplexExporter<ComplexType>::CollectVectorFieldData(ComplexPtr complex, StrategyPtr strategy)
{
    for (int i = 0; i < strategy->akq.size(); i++)
    {
        if (strategy->akq[i] == Strategy::QUEEN)
        {
            Cell queenCell = (*complex)[i];
            Cell* kingCell = strategy->kerKing[i];
            Dim dimQueen = queenCell.getDim();
            Dim dimKing = kingCell->getDim();
            size_t indexQueen = _idIndexMapByDim[dimQueen][i];
            size_t indexKing = _idIndexMapByDim[dimKing][kingCell->getId()];
            _vectorField[dimKing][indexKing] = indexQueen + 1;
            _inverseVectorField[dimQueen][indexQueen] = indexKing + 1;
        }
        else if (strategy->akq[i] == Strategy::ACE)
        {
            Cell ace = (*complex)[i];
            Dim dim = ace.getDim();
            size_t index = _idIndexMapByDim[dim][i];
            std::vector<int> criticalCell(2);
            criticalCell[0] = static_cast<int>(dim);
            criticalCell[1] = static_cast<int>(index + 1);
            _criticalCells.push_back(criticalCell);
        }
    }
}

template <typename ComplexType>
void HapCWComplexExporter<ComplexType>::ExportData(const char* filename)
{
    std::ofstream output(filename);

    output<<"A:=[ [[1,1,1],[1,0,1],[1,1,1]] , [[1,1,1],[0,0,0],[0,0,0]] , [[1,1,1],[1,0,1],[1,1,1]] ];;"<<std::endl;
    output<<"B:=PureCubicalComplex(A);"<<std::endl;
    output<<"Y:=CubicalComplexToRegularCWComplex(B);"<<std::endl;

    output<<"Y!.boundaries:=[ ";
    for (int i = 0; i <= _maxDim; i++)
    {
        output<<"[";
        for (int j = 0; j < _boundaries[i].size(); j++)
        {
            if (i == 0)
            {
                output<<"[1, 0]";
            }
            else
            {
                int size = _boundaries[i][j].size();
                output<<"["<<size<<", ";
                for (int k = 0; k < size; k++)
                {
                    output<<_boundaries[i][j][k];
                    if (k < size - 1)
                    {
                        output<<",";
                    }
                    output<<" ";
                }
                output<<"]";
            }
            if (j < _boundaries[i].size() - 1)
            {
                output<<",";
            }
            output<<" ";
        }
        output<<"]";
        if (i < _maxDim)
        {
            output<<",";
        }
        output<<" ";
    }
    output<<"];"<<std::endl;

    output<<"Y!.coboundaries:=[ ";
    for (int i = 0; i <= _maxDim; i++)
    {
        output<<"[";
        for (int j = 0; j < _coboundaries[i].size(); j++)
        {
            if (i == _maxDim)
            {
                output<<"[0]";
            }
            else
            {
                int size = _coboundaries[i][j].size();
                output<<"["<<size<<", ";
                for (int k = 0; k < size; k++)
                {
                    output<<_coboundaries[i][j][k];
                    if (k < size - 1)
                    {
                        output<<",";
                    }
                    output<<" ";
                }
                output<<"]";
            }
            if (j < _coboundaries[i].size() - 1)
            {
                output<<",";
            }
            output<<" ";
        }
        output<<"]";
        if (i < _maxDim)
        {
            output<<",";
        }
        output<<" ";
    }
    output<<"];"<<std::endl;

    output<<"Y!.orientation:=[ ";
    for (int i = 0; i <= _maxDim; i++)
    {
        output<<"[";
        for (int j = 0; j < _orientation[i].size(); j++)
        {
            if (i == 0)
            {
                output<<"[1]";
            }
            else
            {
                int size = _orientation[i][j].size();
                output<<"[";
                for (int k = 0; k < size; k++)
                {
                    output<<_orientation[i][j][k];
                    if (k < size - 1)
                    {
                        output<<",";
                    }
                    output<<" ";
                }
                output<<"]";
            }
            if (j < _orientation[i].size() - 1)
            {
                output<<",";
            }
            output<<" ";
        }
        output<<"]";
        if (i < _maxDim)
        {
            output<<",";
        }
        output<<" ";
    }
    output<<"];"<<std::endl;

    output<<"Y!.vectorField:=[ ";
    for (int i = 1; i <= _maxDim; i++)
    {
        output<<"[";
        for (int j = 0; j < _vectorField[i].size(); j++)
        {
            if (_vectorField[i][j] != -1)
            {
                output<<_vectorField[i][j];
            }
            if (j < _vectorField[i].size() - 1)
            {
                output<<",";
            }
            output<<" ";
        }
        output<<"]";
        if (i < _maxDim)
        {
            output<<",";
        }
        output<<" ";
    }
    output<<"];"<<std::endl;

    output<<"Y!.inverseVectorField:=[ ";
    for (int i = 0; i < _maxDim; i++)
    {
        output<<"[";
        for (int j = 0; j < _inverseVectorField[i].size(); j++)
        {
            if (_inverseVectorField[i][j] != -1)
            {
                output<<_inverseVectorField[i][j];
            }
            if (j < _inverseVectorField[i].size() - 1)
            {
                output<<",";
            }
            output<<" ";
        }
        output<<"]";
        if (i < _maxDim)
        {
            output<<",";
        }
        output<<" ";
    }
    output<<"];"<<std::endl;

    output<<"Y!.criticalCells:=[ ";
    for (int i = 0; i < _criticalCells.size(); i++)
    {
        assert(_criticalCells[i].size() == 2);
        output<<"["<<_criticalCells[i][0]<<", "<<_criticalCells[i][1]<<" ]";
        if (i < _criticalCells.size() - 1)
        {
            output<<",";
        }
        output<<" ";
    }
    output<<"];"<<std::endl;

    output<<"Y!.nrCells:=function(n);"<<std::endl;
    output<<"if n>"<<_maxDim<<" then return 0; fi;"<<std::endl;
    output<<"return Length(Filtered(Y!.boundaries[n+1],x->not x[1]=0));"<<std::endl;
    output<<"end;"<<std::endl;

    output.close();
}

template <typename ComplexType>
void HapCWComplexExporter<ComplexType>::PrintIdIndexMap()
{
    std::ofstream output("indices.txt");
    for (int i = 0; i <= _maxDim; i++)
    {
        output<<"dim: "<<i<<std::endl;
        for (typename std::map<CellId, size_t>::iterator it = _idIndexMapByDim[i].begin();
                                                         it != _idIndexMapByDim[i].end();
                                                         it++)
        {
            output<<it->first<<" -> "<<it->second<<std::endl;
        }
    }
    output.close();
}

#endif	/* HAPCWCOMPLEXEXPORTER_HPP */

