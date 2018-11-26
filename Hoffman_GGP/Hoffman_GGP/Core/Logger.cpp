#include "Logger.h"

Logger::Logger()
{
    auto console = spdlog::stdout_color_mt( "console" );
    console->info( "Welcome to spdlog!" );
    console->error( "Some error message with arg: {}", 1 );

    SPDLOG_TRACE( console, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1, 3.23 );
    SPDLOG_DEBUG( console, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1, 3.23 );
}


Logger::~Logger()
{
}
