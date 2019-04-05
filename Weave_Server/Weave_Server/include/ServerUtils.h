//#pragma once

#ifndef __SERVER_UTILS_H
#define __SERVER_UTILS_H

#include "Config.h"
#include "stdafx.h"

/// <summary>
/// An initialization struct for easy config file setup of the server. 
/// </summary>
struct SERVER_INIT_DESC
{
    /** The default port to listen to data from the clients on  */
    unsigned short ListenPort = DEF_LISTEN_PORT;

    /** The default port to send data back to the clients on  */
    unsigned short ResponsePort = DEF_RESPONSE_PORT;

    /** The max number of rooms that are allowed on this server */
    unsigned short MaxRooms = 4;

    /** The time between client state updates on the server */
    float StateUpdateTickRate = 0.5f;

    friend std::ostream& operator<<( std::ostream& os, const SERVER_INIT_DESC& data )
    {
        os << "\nServer Config";
        os << "\n\tListen Port: \t" << data.ListenPort;
        os << "\n\tResponse Port: \t" << data.ResponsePort;
        os << "\n\tMax Rooms: \t" << data.MaxRooms;
        os << "\n\tTickRate: \t" << data.StateUpdateTickRate << "\n";
        return os;
    }
};

namespace ServerUtils
{
    static void PrintHelp() W_UNUSED;
    static void ParseConfigFile( SERVER_INIT_DESC & aOutDesc, const char* aFileName ) W_UNUSED;

    /// <summary>
    /// Print out the help options for this program.
    /// </summary>
    static void PrintHelp() 
    {
        std::cout << "Usage: ./Weave_Server [-h] [-c]" << std::endl;
        std::cout << "\t-h\tPrint out this useful help message" << std::endl;
        std::cout << "\t-c\tSpecify the server config file to use. If not specified the default values will be used." << std::endl;
    }

    /// <summary>
    /// Attempt to read in the server config file.
    /// </summary>
    /// <param name="aOutDesc">Server description to populate with config file data</param>
    /// <param name="aFileName">The config file name</param>
    static void ParseConfigFile( SERVER_INIT_DESC & aOutDesc, const char* aFileName )
    {
        assert( aFileName != nullptr );

        std::ifstream fileInput( aFileName );
        if ( !fileInput.is_open() )
        {
            std::cerr << aFileName << " Was not a valid file! Cannot load in config." << std::endl;
            return;
        }

        for ( std::string line; getline( fileInput, line ); )
        {
            std::string firstWord = line.substr( 0, line.find( " " ) );

            if ( firstWord == "ListenPort" )
            {
                sscanf( line.c_str(), "ListenPort : %hu", &aOutDesc.ListenPort );
            }
            else if ( firstWord == "ResponsePort" )
            {
                sscanf( line.c_str(), "ResponsePort : %hu", &aOutDesc.ResponsePort );
            }
            else if ( firstWord == "MaxRooms" )
            {
                sscanf( line.c_str(), "MaxRooms : %hu", &aOutDesc.MaxRooms );
            }
            else if ( firstWord == "TickRate" )
            {
                sscanf( line.c_str(), "TickRate : %f", &aOutDesc.StateUpdateTickRate );
            }
        }

        fileInput.close();
    }

};   // namespace ServerUtils

#endif  // __SERVER_UTILS_H