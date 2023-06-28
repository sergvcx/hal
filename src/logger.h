#ifndef __LOGGER_H_INCLUDED
#define __LOGGER_H_INCLUDED
#include <ostream>
#include <sstream>

enum TLogLevel {
    LOG_ERROR,
    LOG_WARNING,
    LOG_INFO,
    LOG_DEBUG,
    LOG_DEBUG1,
    LOG_DEBUG2,
    LOG_DEBUG3
};

class Log{
public:
    std::ostringstream os;
    TLogLevel messageLevel;

    Log(TLogLevel level = TLogLevel::LOG_INFO);

    virtual ~Log();

    std::ostringstream &get();

};





#endif //__LOGGER_H_INCLUDED