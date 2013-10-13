/*
 * File:   Logger.h
 * Author: Piotr Brendel
 */

#ifndef LOGGER_H
#define	LOGGER_H

#include <ctime>
#include <ostream>
#include <string>
#include <vector>

class Logger
{
public:

    enum Level
    {
        Output,
        Details,
        Debug,
        Assert,
    };

    static void Begin(Level level);
    static void Begin(Level level, const std::string& msg);
    static int End();
    static int End(const std::string& msg);
    static std::ostream& Log(Level level);

    static bool PrintShavedCellsCount();
    static bool PrintCoreducedCellsCount();

private:

    static Level                _logLevel;
    static std::vector<Level>   _levels;
    static std::vector<time_t>  _timers;

};

#endif	/* LOGGER_H */

