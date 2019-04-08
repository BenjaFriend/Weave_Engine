cmake_minimum_required ( VERSION 3.8 )

project( WEAVE_TESTS LANGUAGES CXX VERSION 0.0.1 )

# Set Build Dir
set( CMAKE_BINARY_DIR	"/build" )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_Output/$<CONFIG> )


# Sets the output directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin" )


################# Complier Options ############
if( MSVC )
    set ( MY_COMPILER_FLAGS "/W3" )
else()
    set ( MY_COMPILER_FLAGS "-Wall -Wno-reorder -Wno-unknown-pragmas -Wno-multichar" )
endif()

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_COMPILER_FLAGS}" )

################# Includes #####################
include_directories( 
	../Weave_Common 
	../Weave_Server/Weave_Server/include 
    ../Weave_Engine/additional/spdlog/include
    ../Weave_Engine/additional/glm
    ../Weave_Engine/additional/json
)

### Setup visual studio source groups / filters ###
file( GLOB_RECURSE _source_list
    ./*.cpp* ./*.h* ./*.hpp*
    ../Weave_Common/*.cpp* 
	../Weave_Common/*.h* 
	../Weave_Common/*.hpp*
)

if( MSVC )
    foreach( _source IN ITEMS ${_source_list} )
    	get_filename_component( _source_path "${_source}" PATH )
        string( REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}" )
        string( REPLACE "/" "\\" _group_path "${_group_path}" )
        source_group( "${_group_path}" FILES "${_source}" )
    endforeach()
endif()


# Prepare "Catch" library for other executables
set( CATCH_INCLUDE_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/Catch2/single_include )
add_library ( Catch INTERFACE )
target_include_directories( Catch INTERFACE ${CATCH_INCLUDE_DIR} )

# Make test executable
set( TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Tests.cpp )
add_executable( ${PROJECT_NAME} ${TEST_SOURCES} )
target_link_libraries( ${PROJECT_NAME} Catch )
