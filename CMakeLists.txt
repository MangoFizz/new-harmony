# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.16)

project(new-harmony)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(balltze-lib/balltze.cmake)

add_library(${PROJECT_NAME} MODULE
    src/lua/helpers.cpp
    src/lua/library.cpp
    src/lua/script.cpp
    src/lua/api/optic.cpp
    src/optic/animation.cpp
    src/optic/handler.cpp
    src/optic/render.cpp
    src/optic/sound.cpp
    src/optic/sprite.cpp
    src/optic/store.cpp
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} balltze ${CMAKE_SOURCE_DIR}/balltze-lib/lib/liblua53.a)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "nharmony")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-static -static-libgcc -static-libstdc++")

install(TARGETS new-harmony DESTINATION "${CMAKE_INSTALL_PREFIX}")
