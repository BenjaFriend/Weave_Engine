cmake_minimum_required( VERSION 3.0 )

############### Project Settings ###############
project( Test_Boost_Cmake )

# Set Build Dir
set( CMAKE_BINARY_DIR    "/build" )

set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#Sets the output directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

################# Add Boost #####################
## TODO: Make this need to be entered from command line instead of in this
## cmake file as a static variable

set( BOOST_ROOT         "C:/Program Files/boost/boost_1_67_0/" )
set( BOOST_LIBRARYDIR   "C:/Program Files/boost/boost_1_67_0/stage/lib/" )
set( Boost_USE_STATIC_LIBS  ON )

message( STATUS "Boost Inc  at: ${Boost_INCLUDE_DIR}" )
message( STATUS "Boost Libs at: ${BOOST_LIBRARYDIR}" )

FIND_PACKAGE( Boost )

if( Boost_FOUND )
    message( STATUS      "Success, Boost found!" )
else()
    message( STATUS      "Cannot find boost!" )
    return()
endif()

############### Include Files  #################
# Glob include folder
file( GLOB SRC_FILES "src/*.cpp" "src/*.hpp" )

include_directories(
    include
    src
    ${Boost_INCLUDE_DIR}
    )

message( STATUS  "Inc files: ${INC_FILES}" )
message( STATUS  "SRC files: ${SRC_FILES}" )

############### Link Libraries ################
set( LINK_LIBS "${BOOST_LIBRARYDIR}" )

link_directories( ${LINK_LIBS} )

################# Complier Options ############
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall" )
endif()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMPILER_FLAGS}" )

################# Add Exe ######################
message ( STATUS "\tMY  Flags:" ${MY_COMPILER_FLAGS} )
message ( STATUS "\tcxx Flags:" ${CMAKE_CXX_FLAGS} )

add_executable( ${PROJECT_NAME} ${SRC_FILES} )
