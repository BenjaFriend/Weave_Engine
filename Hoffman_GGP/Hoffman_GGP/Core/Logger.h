#pragma once




class Logger
{



public:
    Logger();
    ~Logger();



};

#ifdef _DEBUG

#define LOG(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)


#else

#define LOG(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)

#endif