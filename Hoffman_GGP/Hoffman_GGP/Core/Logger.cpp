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
    console = spdlog::stdout_color_mt( "LOG" );
    spdlog::set_pattern( "[%H:%M:%S] [%^%L%$] [thread %t] %v" );
}

Logger::~Logger()
{
    // Delete the SPD log console

}
