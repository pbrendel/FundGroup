/*
 * File:   FGLogger.h
 * Author: Piotr Brendel
 */

#ifndef FGLOGGER_H
#define	FGLOGGER_H

#include <cassert>
#include <ctime>
#include <iostream>
#include <ostream>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/stream.hpp>
#include <string>
#include <vector>

class FGLogger
{
public:

    enum Level
    {
        None,
        Output,
        Details,
        Debug,
        Assert,
    };

    FGLogger()
        : _nullOstream(boost::iostreams::null_sink())
    {
#ifdef FGLOGGER_LEVEL
        _logLevel = FGLOGGER_LEVEL;
#else
        _logLevel = None;
#endif
    }

    void Begin(Level level)
    {
        _timers.push_back(clock());
    }

    void Begin(Level level, const std::string& msg)
    {
        Log(level)<<msg<<std::endl;
        _levels.push_back(level);
        _timers.push_back(clock());
    }

    int End()
    {
        assert(_levels.size() > 0);
        assert(_levels.size() == _timers.size());
        Level level = _levels.back();
        int t = static_cast<int>(clock() - _timers.back());
        t = t * 1000 / CLOCKS_PER_SEC;
        _levels.pop_back();
        _timers.pop_back();
        Log(level)<<"finished in "<<t<<" ms"<<std::endl;
        return t;
    }

    int End(const std::string& msg)
    {
        assert(_levels.size() > 0);
        assert(_levels.size() == _timers.size());
        Level level = _levels.back();
        int t = static_cast<int>(clock() - _timers.back());
        t = t * 1000 / CLOCKS_PER_SEC;
        _levels.pop_back();
        _timers.pop_back();
        Log(level)<<msg<<" in "<<t<<" ms"<<std::endl;
        return t;
    }

    std::ostream& Log(Level level)
    {
        if (static_cast<int>(level) <= static_cast<int>(_logLevel))
        {
            return std::cout;
        }
        return _nullOstream;
    }

    bool PrintShavedCellsCount()
    {
        return _logLevel != None;
    }

    bool PrintCoreducedCellsCount()
    {
        return _logLevel != None;
    }

private:

    Level                _logLevel;
    std::vector<Level>   _levels;
    std::vector<clock_t> _timers;
    boost::iostreams::stream<boost::iostreams::null_sink> _nullOstream;

};

#endif	/* FGLOGGER_H */

