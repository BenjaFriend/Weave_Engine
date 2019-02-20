cmake_minimum_required ( VERSION 3.8 )

############### Project Settings ###############
project( "Weave_Server" )

# Set Build Dir
set( CMAKE_BINARY_DIR    "/build" )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_Output/$<CONFIG> )

#Sets the output directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin" )

################# Add Boost #####################

if( NOT DEFINED ENV{BOOST_ROOT} )
    message( WARNING "BOOST_ROOT environment variable is not defined" )
endif()

if( NOT DEFINED ENV{BOOST_LIBRARYDIR} )
    message( WARNING "BOOST_LIBRARYDIR environment variable is not defined" )
endif()

set( BOOST_ROOT             $ENV{BOOST_ROOT} )
set( BOOST_LIBRARYDIR       $ENV{BOOST_LIBRARYDIR} )
set( Boost_USE_STATIC_LIBS  ON )

message( STATUS "Boost Inc  at: ${Boost_INCLUDE_DIR}" )
message( STATUS "Boost Libs at: ${BOOST_LIBRARYDIR}" )

FIND_PACKAGE( Boost )

if( Boost_FOUND )
    message( STATUS      "Success, Boost found!" )
else()
    message( ERROR      "Cannot find boost! Exiting..." )
    return()
endif()

############### Include Files  #################
# Glob include folder
include_directories(
    include
    src
    ${Boost_INCLUDE_DIR}
    )
############### Link Libraries ################
set( LINK_LIBS "${BOOST_LIBRARYDIR}" )

link_directories( ${LINK_LIBS} )

################# Complier Options ############
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall -Wextra" )
endif()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMPILER_FLAGS}" )

### Setup visual studio source groups / filters ###
file( GLOB_RECURSE _source_list *.cpp* *.h* *.hpp* )

foreach( _source IN ITEMS ${_source_list} )

	get_filename_component( _source_path "${_source}" PATH )
    string( REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}" )
    string( REPLACE "/" "\\" _group_path "${_group_path}" )
    source_group( "${_group_path}" FILES "${_source}" )

endforeach()

################# Add Exe ######################
message ( STATUS "\tMY  Flags:" ${MY_COMPILER_FLAGS} )
message ( STATUS "\tcxx Flags:" ${CMAKE_CXX_FLAGS} )

add_executable( ${PROJECT_NAME} ${_source_list} )
