cmake_minimum_required( VERSION 3.0 )

############### Project Settings ###############
project( Test_Boost_Cmake )

# Set Build Dir
set( CMAKE_BINARY_DIR    "/build" )

################################################

################# Add Boost #####################
## TODO: Make this need to be entered from command line instead of in this
## cmake file as a static variable

set( BOOST_ROOT         "C:/local/boost_1_69_0/" )
set( BOOST_LIBRARYDIR   "C:/local/boost_1_69_0/stage/lib" )
set( Boost_USE_STATIC_LIBS  ON )

message( STATUS "Boost Inc  at: ${Boost_INCLUDE_DIR}" )
message( STATUS "Boost Libs at: ${BOOST_LIBRARYDIR}" )

FIND_PACKAGE( Boost 1.69.0 )

if( Boost_FOUND )
    message( STATUS      "Success, Boost found!" )
endif()
################################################

############### Include Files  #################

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

################################################

############### Link Libraries ################
set(LINK_LIBS "${BOOST_LIBRARYDIR}")

link_directories( ${LINK_LIBS} )
################################################

################# Add Exe ######################
add_executable( ${PROJECT_NAME} main.cpp )

################################################
