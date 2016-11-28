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
                        src/nt_service.cpp

if(WIN32)
    set(FREERDP_WEB_SOURCES ${FREERDP_WEB_SOURCES} src/log.hpp
                                                   src/service.hpp
                                                   src/nt_service.hpp)
endif()

add_executable(freerdp-web ${FREERDP_WEB_SOURCES})

target_link_libraries(freerdp-web ${LIBS})
