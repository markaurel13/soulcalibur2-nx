cmake_minimum_required(VERSION 3.20)
project(soulcalibur2_nx C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

if(NOT DEFINED ENV{DEVKITPRO})
    set(ENV{DEVKITPRO} "/opt/devkitpro")
endif()

set(DEVKITPRO $ENV{DEVKITPRO})
include(${DEVKITPRO}/cmake/Switch.cmake)

# Compiler optimization flags tailored for Cortex-A57 (Stock clocks 1020 MHz)
add_compile_options(
    -O3
    -ffast-math
    -ftree-vectorize
    -fomit-frame-pointer
    -Wall
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/../recomp-common
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/doa2u-re/recomp-runtime
    ${DEVKITPRO}/libnx/include
    ${DEVKITPRO}/portlibs/switch/include
)

set(SWITCH_SOURCES
    src/main_switch.cpp
    src/switch_input.cpp
    src/switch_fs.cpp
    src/d3d_presenter_deko3d.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/doa2u-re/recomp-runtime/fiber_switch_arm64.S
)

add_executable(soulcalibur2 ${SWITCH_SOURCES})

target_link_libraries(soulcalibur2
    PRIVATE
    nx
    deko3d
    m
)

nx_create_nro(soulcalibur2
    NAME "Soulcalibur II"
    AUTHOR "tmcleroy / Switch Port"
    VERSION "0.4.0"
    ICON ${CMAKE_CURRENT_SOURCE_DIR}/../docs/screenshots/select-link-heihachi.jpg
)
