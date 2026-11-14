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

set(RUNTIME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../build/src/recomp-runtime)
set(GENERATED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../private/program/generated)
set(PROGRAM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../private/program/runtime-sources)

file(MAKE_DIRECTORY ${PROGRAM_DIR})

option(SC2_FUNC_PROFILE "Muestreo de funciones guest (telemetria)" OFF)
if(SC2_FUNC_PROFILE)
    add_compile_definitions(SC2_FUNC_PROFILE=1)
endif()

option(SC2_ENABLE_LTO "Enable Link-Time Optimization (disable for fast incremental dev builds)" OFF)
option(SC2_GUEST_PROF "Enable per-function --wrap guest profiling" OFF)

set(_CHUNK_NAMES
    recomp_0000.c
    recomp_0001.c
    recomp_0002.c
    recomp_0003.c
    recomp_0004.c
    recomp_0005.c
    recomp_0006.c
    recomp_0007.c
    recomp_dispatch.c
    recomp_funcs.h
)

foreach(_name IN LISTS _CHUNK_NAMES)
    if(EXISTS ${GENERATED_DIR}/${_name})
        if(_name MATCHES "^recomp_000[0-7]\\.c$")
            file(READ ${GENERATED_DIR}/${_name} _CONTENT)
            if(_name STREQUAL "recomp_0000.c")
                string(REPLACE "void sub_00023C30(void)\n{" "void __real_sub_00023C30(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00024720(void)\n{" "void __real_sub_00024720(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_000239D0(void)\n{" "void __real_sub_000239D0(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00023A60(void)\n{" "void __real_sub_00023A60(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00023AF0(void)\n{" "void __real_sub_00023AF0(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00023B90(void)\n{" "void __real_sub_00023B90(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00011000(void)\n{" "void __real_sub_00011000(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00011250(void)\n{" "void __real_sub_00011250(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00011390(void)\n{" "void __real_sub_00011390(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_000123C0(void)\n{" "void __real_sub_000123C0(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012570(void)\n{" "void __real_sub_00012570(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012860(void)\n{" "void __real_sub_00012860(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012600(void)\n{" "void __real_sub_00012600(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012640(void)\n{" "void __real_sub_00012640(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_000126A0(void)\n{" "void __real_sub_000126A0(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012720(void)\n{" "void __real_sub_00012720(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012550(void)\n{" "void __real_sub_00012550(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012790(void)\n{" "void __real_sub_00012790(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00011710(void)\n{" "void __real_sub_00011710(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00012950(void)\n{" "void __real_sub_00012950(void)\n{" _CONTENT "${_CONTENT}")
            elseif(_name STREQUAL "recomp_0002.c")
                string(REPLACE "void sub_00079AE0(void)\n{" "void __real_sub_00079AE0(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00079A30(void)\n{" "void __real_sub_00079A30(void)\n{" _CONTENT "${_CONTENT}")
                string(REPLACE "void sub_00079CD0(void)\n{" "void __real_sub_00079CD0(void)\n{" _CONTENT "${_CONTENT}")
            elseif(_name STREQUAL "recomp_0003.c")
                string(REPLACE "void sub_000EB2A0(void)\n{" "void __real_sub_000EB2A0(void)\n{" _CONTENT "${_CONTENT}")
            elseif(_name STREQUAL "recomp_0006.c")
                string(REPLACE "void sub_00145A64(void)\n{" "void __real_sub_00145A64(void)\n{" _CONTENT "${_CONTENT}")
            endif()
            if(SC2_FUNC_PROFILE)
                string(REGEX REPLACE "void (sub_([0-9A-F]+))\\(void\\)\n\\{\n" "void \\1(void)\n{\n    SC2_PROF_SCOPE(0x\\2u);\n" _CONTENT "${_CONTENT}")
                set(_CONTENT "#include \"sc2_prof.h\"\n${_CONTENT}")
            endif()
            string(REPLACE "#include \"recomp_funcs.h\"" "#include \"recomp_funcs.h\"\n#include \"sc2_fast_mem.h\"" _CONTENT "${_CONTENT}")
            set(_OLD "")
            if(EXISTS ${PROGRAM_DIR}/${_name})
                file(READ ${PROGRAM_DIR}/${_name} _OLD)
            endif()
            if(_OLD STREQUAL "")
                file(WRITE ${PROGRAM_DIR}/${_name} "${_CONTENT}")
                execute_process(
                    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../tools/opt_eflags.py ${PROGRAM_DIR}/${_name}
                )
                execute_process(
                    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../tools/opt_direct_calls.py ${PROGRAM_DIR}/${_name}
                )
            endif()
        else()
            configure_file(${GENERATED_DIR}/${_name} ${PROGRAM_DIR}/${_name} COPYONLY)
        endif()
    endif()
endforeach()

# Optimization flags for Cortex-A57 (Stock clocks 1020 MHz)
add_compile_options(
    -O3
    -mcpu=cortex-a57+crypto
    -mtune=cortex-a57
    -fno-math-errno
    -fno-trapping-math
    -ffp-contract=off
    -ftree-vectorize
    -fomit-frame-pointer
    $<$<AND:$<BOOL:${SC2_ENABLE_LTO}>,$<OR:$<COMPILE_LANGUAGE:C>,$<COMPILE_LANGUAGE:CXX>>>:-flto=4>
    -Wall
    -Wno-unused-variable
    -Wno-unused-label
    -Wno-unused-function
    $<$<COMPILE_LANGUAGE:CXX>:-fno-char8_t>
)

add_compile_definitions(
    RECOMP_XBE_SHA256="7ce12851850ab46766f77c612b251afe7c8bd81f879e11b4836bdf766be542c6"
    __SWITCH__=1
    main=sc2_runner_main
    $<$<BOOL:${SC2_GUEST_PROF}>:SC2_GUEST_PROF=1>
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime
    ${CMAKE_CURRENT_SOURCE_DIR}/../recomp-common
    ${RUNTIME_DIR}
    ${PROGRAM_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../build/src/xbe
    ${DEVKITPRO}/libnx/include
    ${DEVKITPRO}/portlibs/switch/include
)

set(SWITCH_PLATFORM_SOURCES
    src/main_switch.cpp
    src/switch_input.cpp
    src/switch_fs.cpp
    src/d3d_presenter_deko3d.cpp
    src/switch_audio_audren.cpp
    src/switch_guest_state.cpp
    src/switch_profiler.cpp
    src/switch_sub_23c30.c
    src/switch_sub_24720.c
    src/switch_sub_2b670.c
    src/switch_sub_240c0.c
    src/switch_sub_4b630.c
    src/switch_sub_69370.c
    src/switch_sub_843d0.c
    src/switch_sub_2b670.c
    src/switch_bone_neon.c
    src/switch_math_neon.c
    src/switch_fast_mem_wrap.c
)

set(RECOMP_0001_SRC ${PROGRAM_DIR}/recomp_0001.c)
if(EXISTS ${CMAKE_BINARY_DIR}/recomp_0001_prof.c)
    set(RECOMP_0001_SRC ${CMAKE_BINARY_DIR}/recomp_0001_prof.c)
    set_source_files_properties(${RECOMP_0001_SRC} PROPERTIES COMPILE_DEFINITIONS "SC2_FUNC_PROFILE=1;RECOMP_GENERATED_CODE=1")
endif()

set(PROGRAM_SOURCES
    ${PROGRAM_DIR}/recomp_0000.c
    ${RECOMP_0001_SRC}
    ${PROGRAM_DIR}/recomp_0002.c
    ${PROGRAM_DIR}/recomp_0003.c
    ${PROGRAM_DIR}/recomp_0004.c
    ${PROGRAM_DIR}/recomp_0005.c
    ${PROGRAM_DIR}/recomp_0006.c
    ${PROGRAM_DIR}/recomp_0007.c
    ${PROGRAM_DIR}/recomp_dispatch.c
    ${PROGRAM_DIR}/recomp_program_missing.c
)

set_source_files_properties(${PROGRAM_SOURCES} PROPERTIES
    COMPILE_FLAGS "-O3 -include ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math.h -Wno-parentheses-equality"
)

set(RUNTIME_SOURCES
    ${RUNTIME_DIR}/runner.cpp
    ${RUNTIME_DIR}/program_adapters.c
    ${RUNTIME_DIR}/program_manual.c
    ${RUNTIME_DIR}/d3d8_adapter.c
    ${RUNTIME_DIR}/d3d8_state_adapter.c
    ${RUNTIME_DIR}/d3d8_resource_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_d3d8_draw_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/d3d_presenter_switch_thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_d3d_worker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/d3d_capture_packet.cpp
    ${RUNTIME_DIR}/d3d_vblank.cpp
    ${RUNTIME_DIR}/d3d_draw_model.c
    ${RUNTIME_DIR}/d3d_texture_model.c
    ${RUNTIME_DIR}/d3d_pixel_program.cpp
    ${RUNTIME_DIR}/d3d_device_model.c
    ${RUNTIME_DIR}/d3d_light_model.c
    ${RUNTIME_DIR}/d3d_render_state_model.c
    ${RUNTIME_DIR}/d3d_card_model.c
    ${RUNTIME_DIR}/d3d_frame_model.c
    ${RUNTIME_DIR}/d3d_tile_model.c
    ${RUNTIME_DIR}/d3d_vertex_shader_model.c
    ${RUNTIME_DIR}/dsound_api_adapter.c
    ${RUNTIME_DIR}/dsound_device_model.c
    ${RUNTIME_DIR}/dsound_buffer_adapter.c
    ${RUNTIME_DIR}/dsound_buffer_model.c
    ${RUNTIME_DIR}/dsound_effects_adapter.c
    ${RUNTIME_DIR}/dsound_effects_image.c
    ${RUNTIME_DIR}/dsound_mix_model.c
    ${RUNTIME_DIR}/dsound_hrtf.c
    ${RUNTIME_DIR}/xbox_adpcm.c
    ${RUNTIME_DIR}/input_model.c
    ${RUNTIME_DIR}/input_adapter.c
    ${RUNTIME_DIR}/input_pulse_source.c
    ${RUNTIME_DIR}/free_camera.c
    ${RUNTIME_DIR}/free_camera_adapter.c
    ${RUNTIME_DIR}/scene_freeze.c
    ${RUNTIME_DIR}/scene_freeze_adapter.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_host_diagnostics.cpp
    ${RUNTIME_DIR}/runtime.c
    ${RUNTIME_DIR}/stop_report.c
    ${RUNTIME_DIR}/guest_heap.c
    ${RUNTIME_DIR}/xapi_clock.cpp
    ${RUNTIME_DIR}/native_fiber.c
    ${RUNTIME_DIR}/fiber_model.c
    ${RUNTIME_DIR}/fiber_adapter.c
    ${RUNTIME_DIR}/kernel_config.c
    ${RUNTIME_DIR}/kernel_crypto.c
    ${RUNTIME_DIR}/kernel_debug.c
    ${RUNTIME_DIR}/kernel_device.c
    ${RUNTIME_DIR}/kernel_dispatch.c
    ${RUNTIME_DIR}/kernel_file.c
    ${RUNTIME_DIR}/disc_image.c
    ${RUNTIME_DIR}/kernel_interrupt.c
    ${RUNTIME_DIR}/kernel_memory.c
    ${RUNTIME_DIR}/kernel_ordinals.c
    ${RUNTIME_DIR}/kernel_rtl.c
    ${RUNTIME_DIR}/kernel_startup.c
    ${RUNTIME_DIR}/kernel_thread.c
    ${RUNTIME_DIR}/kernel_video.c
    ${RUNTIME_DIR}/device_model.c
    ${RUNTIME_DIR}/apu_model.c
    ${RUNTIME_DIR}/ohci_model.c
    ${RUNTIME_DIR}/directory_model.c
    ${RUNTIME_DIR}/symbolic_link_model.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/switch_save_transaction.cpp
    ${RUNTIME_DIR}/win32_compat.c
    ${RUNTIME_DIR}/fiber_switch_arm64.S
    ${RUNTIME_DIR}/runner_load.cpp
    ${RUNTIME_DIR}/../xbe/sha.cpp
    ${RUNTIME_DIR}/../xbe/xbe_parser.cpp
)

set(GAME_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_hooks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_settings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_training.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_guest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_netplay.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_state.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_frame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/sc2_wm_select.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/overlay_text.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/sin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/cos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/__sin.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/__cos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/__rem_pio2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/__rem_pio2_large.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/atan.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/atan2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/log2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/log2_data.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../runtime/x87_math/expm1.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../recomp-common/menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../recomp-common/settings.cpp
)

add_executable(soulcalibur2
    ${SWITCH_PLATFORM_SOURCES}
    ${PROGRAM_SOURCES}
    ${RUNTIME_SOURCES}
    ${GAME_SOURCES}
)

target_link_libraries(soulcalibur2
    PRIVATE
    nx
    deko3d
    SDL2
    m
)

set_source_files_properties(
    ${PROGRAM_SOURCES}
    src/switch_sub_2b670.c
    src/switch_profiler.cpp
    PROPERTIES COMPILE_OPTIONS "-fno-lto"
)

target_link_options(soulcalibur2
    PRIVATE
    -O3
    $<$<BOOL:${SC2_ENABLE_LTO}>:-flto=4>
    -ffp-contract=off
    -fno-math-errno
    -fno-trapping-math
    -Wl,--wrap=sub_0002B670
    -Wl,--wrap=sub_000240C0
    -Wl,--wrap=sub_00012860
    -Wl,--wrap=sub_00012570
    -Wl,--wrap=sub_00012640
    -Wl,--wrap=sub_000126A0
    -Wl,--wrap=sub_000125A0
    -Wl,--wrap=sub_00012B00
    -Wl,--wrap=sub_00012BC0
    -Wl,--wrap=sub_000129F0
    -Wl,--wrap=sub_00012790
    -Wl,--wrap=sub_00012950
    -Wl,--wrap=sub_000123C0
    -Wl,--wrap=sub_00011B60
    -Wl,--wrap=recomp_d3d_light_set
    -Wl,--wrap=recomp_d3d_set_simple_render_state
    -Wl,--wrap=recomp_d3d8_submit
    -Wl,--wrap=recomp_dsound_buffers_pump
    -Wl,--wrap=recomp_hrtf_render
    -Wl,--wrap=recomp_d3d8_draw_lookup_manual
    -Wl,--wrap=recomp_d3d_wait_vblank
    -Wl,--wrap=recomp_d3d_wait_present
    -Wl,--wrap=recomp_lookup
    -Wl,--wrap=recomp_lookup_manual
    -Wl,--wrap=recomp_dispatch_indirect_site
    -Wl,--wrap=nanosleep
    -Wl,--wrap=recomp_audio_output_submit
    -Wl,--wrap=recomp_audio_output_reset_voice
    -Wl,--wrap=sub_000843D0
    -Wl,--wrap=sub_0004B630
    -Wl,--wrap=sub_0004D090
    -Wl,--wrap=sub_00069370
    -Wl,--wrap=sub_00067D70
    -Wl,--wrap=sub_00069680
    -Wl,--wrap=sub_000688E0
    -Wl,--wrap=sub_00063100
    -Wl,--wrap=sub_0003EE30
    -Wl,--wrap=sub_000343A0
    -Wl,--wrap=sub_0003D9A0
    -Wl,--wrap=sub_0003D9F0
    -Wl,--wrap=sub_00046EA0
    -Wl,--wrap=sub_000423B0
    -Wl,--wrap=sub_0003FCF0
    -Wl,--wrap=sub_00042A80
    -Wl,--wrap=sub_00048780
    -Wl,--wrap=sub_00048C00
    -Wl,--wrap=sub_00041120
    -Wl,--wrap=sub_00041550
    -Wl,--wrap=sub_000496E0
    -Wl,--wrap=sub_00040670
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00012AB0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00014B90>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00024E10>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002AA30>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002ABC0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002AD90>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002AF00>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002B050>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002B2F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002B4F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0002EA90>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00060F70>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00061B10>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00076BD0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00076C20>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0008FFC0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00090110>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000DACA0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004B630>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004D090>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003EE30>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00040DD0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041FB0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00042090>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046EA0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046FF0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00053680>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00053700>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000A28A0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000A29E0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000A2A20>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000ADA70>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0001F600>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004D610>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041120>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000343A0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004C8C0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00029E50>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004A090>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00089BA0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003FCF0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00042A80>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000423B0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00048C00>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00048780>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00069370>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004B420>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00042160>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00048510>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000E9610>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0004B030>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000477A0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000444F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046280>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000ED5C0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003DAB0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00040670>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041330>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041AF0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000440E0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00044B00>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046A50>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00047140>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00047750>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00048A40>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000496E0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00089850>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000E6410>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000E94F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0007BD50>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000F62D0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000F67D0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000F6A10>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000F7170>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00011B60>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00012B40>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0001BF30>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00033E40>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00034E50>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000356D0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003D560>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003D5D0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003D6F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003D9A0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003D9F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_0003E7A0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000412F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041550>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000415F0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000419C0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041A00>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041CA0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00041FD0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00043690>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000441C0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000443E0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00044CC0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00045550>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00045BA0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046020>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046870>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046D30>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00046E20>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00048170>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000482D0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00048E60>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00050D20>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00053650>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00053740>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00053C00>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000EC580>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_001456E4>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000E9290>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00011C20>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00145BE2>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00082A10>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00082C50>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00067CE0>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_00018130>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=sub_000200B0>
    -Wl,--wrap=sc2_x87_sin
    -Wl,--wrap=sc2_x87_cos
    -Wl,--wrap=sc2_x87_atan2
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=recomp_memory>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=recomp_memory_u32_checked>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=recomp_guest_load>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=recomp_guest_store>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=recomp_guest_memcpy>
    $<$<BOOL:${SC2_GUEST_PROF}>:-Wl,--wrap=recomp_guest_memset>
)



nx_create_nro(soulcalibur2
    NAME "Soulcalibur II"
    AUTHOR "tmcleroy / Switch Port"
    VERSION "0.4.0"
    ICON ${CMAKE_CURRENT_SOURCE_DIR}/../docs/screenshots/select-link-heihachi.jpg
)
