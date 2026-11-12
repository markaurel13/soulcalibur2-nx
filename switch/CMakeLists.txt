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
            endif()
            if(SC2_FUNC_PROFILE)
                string(REGEX REPLACE "void (sub_([0-9A-F]+))\\(void\\)\n\\{\n" "void \\1(void)\n{\n    SC2_PROF_SCOPE(0x\\2u);\n" _CONTENT "${_CONTENT}")
                set(_CONTENT "#include \"sc2_prof.h\"\n${_CONTENT}")
            endif()
            set(_OLD "")
            if(EXISTS ${PROGRAM_DIR}/${_name})
                file(READ ${PROGRAM_DIR}/${_name} _OLD)
            endif()
            if(_OLD STREQUAL "")
                file(WRITE ${PROGRAM_DIR}/${_name} "${_CONTENT}")
                execute_process(
                    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/../tools/opt_eflags.py ${PROGRAM_DIR}/${_name}
                )
            endif()
        else()
            configure_file(${GENERATED_DIR}/${_name} ${PROGRAM_DIR}/${_name} COPYONLY)
        endif()
    endif()
endforeach()

# Optimization flags for Cortex-A57 (Stock clocks 1020 MHz)
add_compile_options(
    -O2
    -mcpu=cortex-a57+crypto
    -mtune=cortex-a57
    -ffast-math
    -ftree-vectorize
    -fomit-frame-pointer
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
    src/switch_bone_neon.c
    src/switch_math_neon.c
)

set(PROGRAM_SOURCES
    ${PROGRAM_DIR}/recomp_0000.c
    ${PROGRAM_DIR}/recomp_0001.c
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

target_link_options(soulcalibur2
    PRIVATE
    -Wl,--wrap=sub_0002B670
    -Wl,--wrap=sub_000240C0
    -Wl,--wrap=recomp_d3d8_submit
    -Wl,--wrap=recomp_dsound_buffers_pump
    -Wl,--wrap=recomp_hrtf_render
    -Wl,--wrap=recomp_d3d8_draw_lookup_manual
    -Wl,--wrap=recomp_d3d_wait_vblank
    -Wl,--wrap=recomp_d3d_wait_present
    -Wl,--wrap=recomp_lookup
    -Wl,--wrap=recomp_lookup_manual
    -Wl,--wrap=recomp_dispatch_indirect_site
    -Wl,--wrap=sc2_x87_sin
    -Wl,--wrap=sc2_x87_cos
    -Wl,--wrap=sc2_x87_atan2
    -Wl,--wrap=recomp_memory
    -Wl,--wrap=recomp_memory_u32_checked
    -Wl,--wrap=recomp_guest_load
    -Wl,--wrap=recomp_guest_store
    -Wl,--wrap=recomp_guest_memcpy
    -Wl,--wrap=recomp_guest_memset
)



nx_create_nro(soulcalibur2
    NAME "Soulcalibur II"
    AUTHOR "tmcleroy / Switch Port"
    VERSION "0.4.0"
    ICON ${CMAKE_CURRENT_SOURCE_DIR}/../docs/screenshots/select-link-heihachi.jpg
)
