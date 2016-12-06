cmake_minimum_required(VERSION 3.3)

project("freerdp-web")

#list of needed libraries
set(LIBS)

#set the version string
if(VERSION_STRING)
    add_definitions(-DVERSION_STRING="${VERSION_STRING}")
else()
    add_definitions(-DVERSION_STRING="DEFAULT")
endif()

set(FREERDP_WEB_SOURCES src/main.cpp
                        src/log.cpp
                        src/service.cpp
                        src/server.cpp
                        src/nt_service.cpp
                        src/daemon.cpp)

if(WIN32)
    set(FREERDP_WEB_SOURCES ${FREERDP_WEB_SOURCES} src/log.hpp
                                                   src/service.hpp
                                                   src/server.hpp
                                                   src/nt_service.hpp
                                                   src/daemon.hpp)
endif()

if(WIN32)
else()
    set(EXTRA_FLAGS "-std=gnu++14")
endif()

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS})

add_executable(freerdp-web ${FREERDP_WEB_SOURCES})

target_link_libraries(freerdp-web ${LIBS})
