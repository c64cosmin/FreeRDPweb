cmake_minimum_required(VERSION 3.3)

project("freerdp-web")

#list of needed libraries
set(LIBS)

#search for Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(BOOST_ALL_DYN_LINK ON)
set(BOOST_ALL_NO_LIB ON)
find_package(Boost 1.61 REQUIRED system program_options)

#set up directories
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#add libraries for linking
set(LIBS ${LIBS} ${Boost_LIBRARIES})

set(FREERDP_WEB_SOURCES src/main.cpp)

add_executable(freerdp-web ${FREERDP_WEB_SOURCES})
