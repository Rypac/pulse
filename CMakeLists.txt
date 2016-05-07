cmake_policy(SET CMP0017 NEW)

cmake_minimum_required(VERSION 2.8)

set(APP_NAME Pulse)
project(${APP_NAME})

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/libs/cocos2d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
include(CocosBuildHelpers)

# libcocos2d
set(BUILD_CPP_TESTS OFF CACHE BOOL "turn off build cpp-tests")
set(BUILD_LUA_LIBS OFF CACHE BOOL "turn off build lua related targets")
set(BUILD_JS_LIBS OFF CACHE BOOL "turn off build js related targets")
add_subdirectory(${COCOS2D_ROOT})

# Some macro definitions
ADD_DEFINITIONS(-DCC_TARGET_OS_MAC)
ADD_DEFINITIONS(-DUSE_FILE32API)
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

set(LIB_DIR ${CMAKE_SOURCE_DIR}/libs)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(PULSE_DIR ${SRC_DIR}/pulse)

include_directories(
    /usr/local/include/GLFW
    /usr/include/GLFW
    ${COCOS2D_ROOT}/cocos
    ${COCOS2D_ROOT}/cocos/platform
    ${COCOS2D_ROOT}/cocos/audio/include
    ${SRC_DIR}
    ${LIB_DIR}/cpputils/include
    ${LIB_DIR}/GSL/include
    ${LIB_DIR}/range-v3/include
)

set(GAME_SRC
    ${PULSE_DIR}/AppDelegate.cpp
    ${PULSE_DIR}/generators/ObstacleGenerator.cpp
    ${PULSE_DIR}/models/Resolution.cpp
    ${PULSE_DIR}/scenes/DeveloperMenuScene.cpp
    ${PULSE_DIR}/scenes/GameScene.cpp
    ${PULSE_DIR}/scenes/PulseGameScene.cpp
    ${PULSE_DIR}/scenes/SplashScene.cpp
    ${PULSE_DIR}/scenes/TitleScene.cpp
    ${PULSE_DIR}/sprites/Column.cpp
    ${PULSE_DIR}/sprites/Obstacle.cpp
    ${PULSE_DIR}/sprites/ObstaclePhysicsBody.cpp
    ${PULSE_DIR}/sprites/SpritePhysicsBody.cpp
    ${PULSE_DIR}/sprites/WrappedSprite.cpp
    ${PULSE_DIR}/ui/MenuSlider.cpp
    ${PLATFORM_SPECIFIC_SRC}
)

set(GAME_HEADERS
    ${PULSE_DIR}/AppDelegate.hpp
    ${PULSE_DIR}/generators/Generator.hpp
    ${PULSE_DIR}/generators/ObstacleGenerator.hpp
    ${PULSE_DIR}/models/Accelerometer.hpp
    ${PULSE_DIR}/models/GameOptions.hpp
    ${PULSE_DIR}/models/GameState.hpp
    ${PULSE_DIR}/models/Resolution.hpp
    ${PULSE_DIR}/models/TimeScale.hpp
    ${PULSE_DIR}/scenes/DeveloperMenuScene.hpp
    ${PULSE_DIR}/scenes/GameScene.hpp
    ${PULSE_DIR}/scenes/PulseGameScene.hpp
    ${PULSE_DIR}/scenes/SplashScene.hpp
    ${PULSE_DIR}/scenes/TitleScene.hpp
    ${PULSE_DIR}/sprites/Column.hpp
    ${PULSE_DIR}/sprites/Obstacle.hpp
    ${PULSE_DIR}/sprites/ObstaclePhysicsBody.hpp
    ${PULSE_DIR}/sprites/SpritePhysicsBody.hpp
    ${PULSE_DIR}/sprites/WrappedSprite.hpp
    ${PULSE_DIR}/ui/Font.hpp
    ${PULSE_DIR}/ui/MenuSlider.hpp
    ${PULSE_DIR}/utilities/Acceleration.hpp
    ${PULSE_DIR}/utilities/Direction.hpp
    ${PULSE_DIR}/utilities/Geometry.hpp
    ${PULSE_DIR}/utilities/Logger.hpp
    ${PULSE_DIR}/utilities/Position.hpp
    ${PULSE_DIR}/utilities/Random.hpp
    ${PULSE_DIR}/utilities/Rect.hpp
    ${PULSE_DIR}/utilities/Rotation.hpp
    ${PLATFORM_SPECIFIC_HEADERS}
)

add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})

target_link_libraries(${APP_NAME} cocos2d)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")

pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${APP_BIN_DIR}/resources
)
