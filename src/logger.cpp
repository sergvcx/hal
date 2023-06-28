#include "logger.h"
#include <iostream>
#include <iomanip>

Log::Log(TLogLevel level) {
    messageLevel = level;
}

std::string toString(TLogLevel level){
    switch (level)
    {
    case LOG_ERROR:
        return "ERROR";
    case LOG_WARNING:
        return "WARNING";
    case LOG_INFO:
        return "INFO";
    case LOG_DEBUG:
        return "DEBUG";
    case LOG_DEBUG1:
        return "DEBUG1";
    case LOG_DEBUG2:
        return "DEBUG2";
    case LOG_DEBUG3:
        return "DEBUG3";
    default:
        return "";
    }
}

std::ostringstream &Log::get(){
    os << "[" << std::setw(8) << toString(messageLevel) << "]";
    return os;
}

Log::~Log()
{
    os << std::endl;
    fprintf(stderr, "%s", os.str().c_str());
    fflush(stderr);
}