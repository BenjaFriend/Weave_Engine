cmake_minimum_required ( VERSION 3.13 )

############### Project Settings ###############
project( "Weave_Server" )

# Set Build Dir
set( CMAKE_BINARY_DIR    "/build" )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_Output/$<CONFIG> )

#Sets the output directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin" )

message( STATUS "Using compiler:\t " ${CMAKE_CXX_COMPILER} )

################# Add Boost #####################

if( NOT DEFINED ENV{BOOST_ROOT} )
    message( WARNING "BOOST_ROOT environment variable is not defined" )
endif()

if( NOT DEFINED ENV{BOOST_LIBRARYDIR} )
    message( WARNING "BOOST_LIBRARYDIR environment variable is not defined" )
endif()

# Set the boost variables to the local enviornment variables
set( BOOST_ROOT             $ENV{BOOST_ROOT} )
set( Boost_LIBRARY_DIR      $ENV{BOOST_LIBRARYDIR} )

message( STATUS "BOOST_ROOT         at: ${BOOST_ROOT}" )
message( STATUS "Boost_INCLUDE_DIR  at: ${Boost_INCLUDE_DIR}" )
message( STATUS "Boost_LIBRARY_DIR  at: ${Boost_LIBRARY_DIR}" )

#set( Boost_USE_STATIC_LIBS      OFF )
#set( Boost_USE_STATIC_RUNTIME 	OFF )
#set( Boost_USE_MULTITHREADED 	OFF )

FIND_PACKAGE( Boost 1.69.0 COMPONENTS system regex REQUIRED )

if( Boost_FOUND )
    message( STATUS      "Success, Boost found!" )
else()
    message( ERROR      "Cannot find boost! Exiting..." )
    return()
endif()

message( STATUS "-- Boost_INCLUDE_DIR  at: ${Boost_INCLUDE_DIR}" )
message( STATUS "-- Boost_LIBRARY_DIR  at: ${Boost_LIBRARY_DIR}" )

############### Include Files  #################
# Glob include folder
include_directories(
    include
    src
    ${Boost_INCLUDE_DIR}
)
############### Link Libraries ################
set( LINK_DIRS
    ${Boost_LIBRARY_DIR}
)

set ( LINK_LIBS
      boost_system boost_regex
      pthread
)

message ( STATUS "LINK_DIRS: " ${LINK_DIRS} )

################# Complier Options ############
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall -Wextra" )
endif()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMPILER_FLAGS}" )

### Setup visual studio source groups / filters ###
file( GLOB_RECURSE _source_list src/*.cpp* src/*.h* src/*.hpp*   include/*.cpp* include/*.h* include/*.hpp* )

if( MSVC )
    foreach( _source IN ITEMS ${_source_list} )
    	get_filename_component( _source_path "${_source}" PATH )
        string( REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}" )
        string( REPLACE "/" "\\" _group_path "${_group_path}" )
        source_group( "${_group_path}" FILES "${_source}" )
    endforeach()
endif()
################# Add Exe ######################
message ( STATUS "\tcxx Flags:" ${CMAKE_CXX_FLAGS} )

add_executable( ${PROJECT_NAME} ${_source_list} )

# C++ Standard
set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11 )

# Link the directories to target
link_directories( ${LINK_DIRS} )

# Link libraries to target
target_link_libraries( ${PROJECT_NAME} ${LINK_LIBS} )
