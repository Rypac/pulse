cmake_policy(SET CMP0017 NEW)

cmake_minimum_required(VERSION 2.8)

set(APP_NAME MyGame)
project (${APP_NAME})

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
include(CocosBuildHelpers)

# libcocos2d
set(BUILD_CPP_TESTS OFF CACHE BOOL "turn off build cpp-tests")
set(BUILD_LUA_LIBS OFF CACHE BOOL "turn off build lua related targets")
set(BUILD_JS_LIBS OFF CACHE BOOL "turn off build js related targets")
add_subdirectory(${COCOS2D_ROOT})

# Some macro definitions
ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
ADD_DEFINITIONS (-DUSE_FILE32API)
set(PLATFORM_FOLDER mac)

# Compiler options
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wno-deprecated-declarations -Wno-reorder")
if(CLANG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(PLATFORM_SPECIFIC_SRC
  proj.ios_mac/ios/main.m
  proj.ios_mac/ios/RootViewController.mm
  proj.ios_mac/ios/AppController.mm
)
set(PLATFORM_SPECIFIC_HEADERS
  proj.ios_mac/ios/RootViewController.h
  proj.ios_mac/ios/AppController.h
)

include_directories(
  /usr/local/include/GLFW
  /usr/include/GLFW
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/audio/include/
  Classes
)

set(GAME_SRC
  Classes/AppDelegate.cpp
  Classes/HelloWorldScene.cpp
  ${PLATFORM_SPECIFIC_SRC}
)

set(GAME_HEADERS
  Classes/AppDelegate.h
  Classes/HelloWorldScene.h
  ${PLATFORM_SPECIFIC_HEADERS}
)

add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})

target_link_libraries(${APP_NAME} cocos2d)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")

pre_build(${APP_NAME}
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
)
