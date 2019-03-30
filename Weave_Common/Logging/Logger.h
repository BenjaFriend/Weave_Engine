#pragma once

#define SPDLOG_TRACE_ON
#define SPDLOG_DEBUG_ON

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"

/// <summary>
/// Base logger class for handling the logging to the console 
/// and to any files
/// </summary>
/// <author>Ben Hoffman</author>
class Logger
{

public:

    /// <summary>
    /// Get an instance to the current logger
    /// </summary>
    /// <returns>Instance of the current logger</returns>
    static Logger * GetInstance();

    /// <summary>
    /// Release the current instance of the logger
    /// </summary>
    static void ReleaseInstance();

    /// <summary>
    /// Gets a reference to the current logging console
    /// </summary>
    /// <returns>Shader ptr to the current console</returns>
    static std::shared_ptr<spdlog::logger> GetCurrentConsole();

private:

    /// <summary>
    /// Private constructor of the logger
    /// </summary>
    Logger();

    /// <summary>
    /// Private destructor of the logger
    /// </summary>
    ~Logger();

    /** Static instance of the logger */
    static Logger* Instance;

    /** Pointer to the current console that is being used for logging */
    std::shared_ptr<spdlog::logger> console;
};

// Current print format: 
// LOG_TRACE ( "This is an example of {}", "this" );

#if defined( DEBUG ) || defined ( _DEBUG ) || defined ( W_ENABLE_LOGGING )

#define  LOG_TRACE( ... )    Logger::GetCurrentConsole()->info( __VA_ARGS__ )
#define  LOG_WARN( ... )     Logger::GetCurrentConsole()->warn( __VA_ARGS__ )
#define  LOG_ERROR( ... )    Logger::GetCurrentConsole()->error( __VA_ARGS__ )

#else

#define  LOG_TRACE( ... ) 
#define  LOG_WARN( ... )  
#define  LOG_ERROR( ... ) 

#endif