#######################
# This cmake file was Generated from:  
# cmake-converter -p .\Hoffman_GGP\Hoffman_GGP\Hoffman_GGP.vcxproj -i
#######################

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

################### Variables. ####################
# Change if you want modify path or other values. #
###################################################

set(PROJECT_NAME DX11Starter)
# Output Variables
set(OUTPUT_DEBUG $(ProjectDir)_Output/)
set(OUTPUT_REL $(ProjectDir)_Output/)
# Folders files
set(CPP_DIR_1 ../additional/imgui/imgui)
set(CPP_DIR_2 Core)
set(CPP_DIR_3 ECS)
set(CPP_DIR_4 Entity)
set(CPP_DIR_5 Lighting)
set(CPP_DIR_6 Physics)
set(CPP_DIR_7 Resources/Materials)
set(CPP_DIR_8 Resources)
set(CPP_DIR_9 Scripting)
set(CPP_DIR_10 ./)
set(HEADER_DIR_1 ../additional/imgui/imgui)
set(HEADER_DIR_2 Core)
set(HEADER_DIR_3 ECS)
set(HEADER_DIR_4 Entity)
set(HEADER_DIR_5 Lighting)
set(HEADER_DIR_6 Physics)
set(HEADER_DIR_7 Resources)
set(HEADER_DIR_8 Resources/Materials)
set(HEADER_DIR_9 Scripting)
set(HEADER_DIR_10 )

############## CMake Project ################
#        The main options of project        #
#############################################

project(${PROJECT_NAME} CXX)

# Define Release by default.
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
  message(STATUS "Build type not specified: Use Release by default.")
endif(NOT CMAKE_BUILD_TYPE)

# Definition of Macros
add_definitions(
   -DSHADER_SRC
)

############## Artefacts Output #################
# Defines outputs , depending Debug or Release. #
#################################################

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}")
else()
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
  set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUT_REL}")
endif()

# Include directories
include_directories($ENV{SolutionDir}additional/Autodesk_2019_0/include)
include_directories($ENV{SolutionDir}additional/imgui)
include_directories($ENV{SolutionDir}additional/spdlog/include)
include_directories($ENV{SolutionDir}additional/json)
include_directories($ENV{SolutionDir}additional/LuaBridge/Source)
include_directories($ENV{SolutionDir}additional/lua/include)
include_directories($ENV{SolutionDir}additional/sol2)

################# Flags ################
# Defines Flags for Windows and Linux. #
########################################

if(MSVC)
   set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W3 /MD /Od /EHsc")
   set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /GL /Od /Oi /Gy /EHsc")
endif(MSVC)
if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
   endif()
endif(NOT MSVC)

################ Files ################
#   --   Add files to project.   --   #
#######################################

file(GLOB SRC_FILES
    ${CPP_DIR_1}/*.cpp
    ${CPP_DIR_2}/*.cpp
    ${CPP_DIR_3}/*.cpp
    ${CPP_DIR_4}/*.cpp
    ${CPP_DIR_5}/*.cpp
    ${CPP_DIR_6}/*.cpp
    ${CPP_DIR_7}/*.cpp
    ${CPP_DIR_8}/*.cpp
    ${CPP_DIR_9}/*.cpp
    ${CPP_DIR_10}/*.cpp
    ${HEADER_DIR_1}/*.h
    ${HEADER_DIR_2}/*.h
    ${HEADER_DIR_3}/*.h
    ${HEADER_DIR_4}/*.h
    ${HEADER_DIR_5}/*.h
    ${HEADER_DIR_6}/*.h
    ${HEADER_DIR_7}/*.h
    ${HEADER_DIR_8}/*.h
    ${HEADER_DIR_9}/*.h
    ${HEADER_DIR_10}/*.h
)

# Add executable to build.
add_executable(${PROJECT_NAME}
   ${SRC_FILES}
)

if(MSVC)
   target_link_libraries(${PROJECT_NAME} lua52.lib )
endif(MSVC)
