#pragma once

#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#include "spdlog/spdlog.h"

class Logger
{

public:

    Logger();

    ~Logger();

};

#if defined( DEBUG ) || defined ( _DEBUG )

#define  LOG_TRACE( ... )       printf( __VA_ARGS__ )
#define  LOG_WARN( ... )        printf( __VA_ARGS__ )
#define  LOG_ERROR( ... )       printf( __VA_ARGS__ )

#else

#define  LOG_TRACE( ... ) 
#define  LOG_WARN( ... )  
#define  LOG_ERROR( ... ) 

#endif