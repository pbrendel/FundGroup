/*
 * File:   Logger.cpp
 * Author: Piotr Brendel
 */

#include "Logger.h"

#include <cassert>
#include <iostream>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/stream.hpp>

////////////////////////////////////////////////////////////////////////////////

namespace
{
    boost::iostreams::stream<boost::iostreams::null_sink> nullOstream((boost::iostreams::null_sink()));
}

////////////////////////////////////////////////////////////////////////////////

Logger::Level               Logger::_logLevel = Logger::Assert;
std::vector<Logger::Level>  Logger::_levels;
std::vector<time_t>         Logger::_timers;

////////////////////////////////////////////////////////////////////////////////

void Logger::Begin(Logger::Level level)
{
    _timers.push_back(time(0));
}

void Logger::Begin(Logger::Level level, const std::string& msg)
{
    Log(level)<<msg<<std::endl;
    _levels.push_back(level);
    _timers.push_back(time(0));
}

int Logger::End()
{
    assert(_levels.size() > 0);
    assert(_levels.size() == _timers.size());
    Level level = _levels.back();
    int t = static_cast<int>(time(0) - _timers.back());
    _levels.pop_back();
    _timers.pop_back();
    Log(level)<<"finished in "<<t<<" seconds"<<std::endl;
    return t;
}

int Logger::End(const std::string& msg)
{
    assert(_levels.size() > 0);
    assert(_levels.size() == _timers.size());
    Level level = _levels.back();
    int t = static_cast<int>(time(0) - _timers.back());
    _levels.pop_back();
    _timers.pop_back();
    Log(level)<<msg<<" in "<<t<<" seconds"<<std::endl;
    return t;
}

std::ostream& Logger::Log(Logger::Level level)
{
    if (static_cast<int>(level) <= static_cast<int>(_logLevel))
    {
        return std::cout;
    }
    return nullOstream;
}

bool Logger::PrintShavedCellsCount()
{
    return true;
}

bool Logger::PrintCoreducedCellsCount()
{
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// eof
