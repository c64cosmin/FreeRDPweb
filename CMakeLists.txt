cmake_minimum_required(VERSION 3.3)

project("freerdp-web")

#list of needed libraries
set(LIBS)

#search for Boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_ALL_DYN_LINK)
find_package(Boost 1.61 REQUIRED system program_options)

#set up directories
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#add libraries for linking
set(LIBS ${LIBS} ${Boost_LIBRARIES})

#set the version string
if(VERSION_STRING)
    add_definitions(-DVERSION_STRING="${VERSION_STRING}")
else()
    add_definitions(-DVERSION_STRING="DEFAULT")
endif()

set(FREERDP_WEB_SOURCES src/main.cpp
                        src/log.cpp)

if(WIN32)
    set(FREERDP_WEB_SOURCES ${FREERDP_WEB_SOURCES} src/log.hpp)
endif()

add_executable(freerdp-web ${FREERDP_WEB_SOURCES})

target_link_libraries(freerdp-web ${LIBS})
