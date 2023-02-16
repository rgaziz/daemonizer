project(Daemonizer)

set(target daemonizer)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/service.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/service.h
    ${CMAKE_CURRENT_SOURCE_DIR}/daemon.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/daemon.h)

add_library(${target} STATIC  ${sources})

target_include_directories(${target}
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties( ${target}
        PROPERTIES
            CXX_STANDARD 17)
