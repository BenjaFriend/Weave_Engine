#include "Logger.h"

Logger* Logger::Instance = nullptr;

Logger * Logger::GetInstance()
{
    if ( Instance == nullptr )
    {
        Instance = new Logger();
    }

    return Instance;
}

void Logger::ReleaseInstance()
{
    if ( Instance ) delete Instance;
}

std::shared_ptr<spdlog::logger> Logger::GetCurrentConsole()
{
    Instance->console->info( "GetConsole" );

    return Instance->console;
}

Logger::Logger()
{
    // Create the SPD log console 

    console = spdlog::stdout_color_mt( "console" );
    console->info( "Welcome to spdlog!" );
    console->error( "Some error message with arg: {}", 1 );

    SPDLOG_TRACE( console, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1, 3.23 );
    SPDLOG_DEBUG( console, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1, 3.23 );
}

Logger::~Logger()
{
    // Delete the SPD log console

}
