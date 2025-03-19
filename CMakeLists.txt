cmake_minimum_required(VERSION 3.14)
project(demoLvgl)

set(CMAKE_C_STANDARD 99) # LVGL officially supports C99 and above
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LV_CONF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/lv_conf.h)
set(LV_CONF_BUILD_DISABLE_EXAMPLES ON)
set(LV_CONF_BUILD_DISABLE_DEMOS OFF)

find_package(PkgConfig REQUIRED)

add_subdirectory(lvgl)
###############
### Wayland ###
###############
message("Including Wayland support")
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WAYLAND_CURSOR REQUIRED wayland-cursor)
pkg_check_modules(XKBCOMMON REQUIRED xkbcommon)
# Wayland protocols
pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols>=1.25)
pkg_get_variable(WAYLAND_PROTOCOLS_BASE wayland-protocols pkgdatadir)

message("Generate wayland protocols")
execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gen_wl_protocols.sh" ${PROJECT_SOURCE_DIR} OUTPUT_VARIABLE WAYLAND_PROTOCOLS_SRC)
file(GLOB_RECURSE WAYLAND_PROTOCOLS_SRC "${PROJECT_SOURCE_DIR}/wl_protocols/*.c")
message("Wayland protocols: ${WAYLAND_PROTOCOLS_SRC}")

target_include_directories(lvgl PRIVATE ${PROJECT_SOURCE_DIR}/wl_protocols)
target_sources(lvgl PRIVATE ${WAYLAND_PROTOCOLS_SRC})
###############
### Wayland: end ###
###############
add_executable(demoLvgl src/main.c src/examples.c)

target_include_directories(demoLvgl PRIVATE ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})

target_link_libraries(demoLvgl PRIVATE lvgl::lvgl lvgl_demos)
target_link_libraries(demoLvgl PRIVATE ${WAYLAND_CLIENT_LIBRARIES} ${WAYLAND_CURSOR_LIBRARIES} ${XKBCOMMON_LIBRARIES})

# Additionnal cmd for Wayland
# sudo apt install pkg-config wayland-protocols libwayland-dev libwayland-bin libxkbcommon-dev
