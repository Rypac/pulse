LOCAL_PATH := $(call my-dir)
ROOT_PATH := $(LOCAL_PATH)/../../..
COCOS_PATH := $(ROOT_PATH)/libs/cocos2d
LIB_PATH := $(ROOT_PATH)/libs
INC_PATH := $(ROOT_PATH)/src
SRC_PATH := ../../../src
PULSE_PATH := $(SRC_PATH)/pulse

include $(CLEAR_VARS)

$(call import-add-path,$(COCOS_PATH))
$(call import-add-path,$(COCOS_PATH)/external)
$(call import-add-path,$(COCOS_PATH)/cocos)

LOCAL_MODULE := cocos2dcpp_shared
LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := \
    bootstrap/main.cpp \
    $(PULSE_PATH)/AppDelegate.cpp \
    $(PULSE_PATH)/actions/AnimatedBackground.cpp \
    $(PULSE_PATH)/actions/FollowedBy.cpp \
    $(PULSE_PATH)/actions/ObstacleSequence.cpp \
    $(PULSE_PATH)/actions/RemoveSelfWithCallback.cpp \
    $(PULSE_PATH)/generators/ObstacleGenerator.cpp \
    $(PULSE_PATH)/models/GameState.cpp \
    $(PULSE_PATH)/models/Resolution.cpp \
    $(PULSE_PATH)/movement/AccelerometerMovementSystem.cpp \
    $(PULSE_PATH)/scenes/DeveloperMenuScene.cpp \
    $(PULSE_PATH)/scenes/GameOverScene.cpp \
    $(PULSE_PATH)/scenes/BaseScene.cpp \
    $(PULSE_PATH)/scenes/PauseMenuScene.cpp \
    $(PULSE_PATH)/scenes/ModeSelectionScene.cpp \
    $(PULSE_PATH)/scenes/PhysicsScene.cpp \
    $(PULSE_PATH)/scenes/GameScene.cpp \
    $(PULSE_PATH)/scenes/SplashScene.cpp \
    $(PULSE_PATH)/scenes/TitleScene.cpp \
    $(PULSE_PATH)/sprites/Column.cpp \
    $(PULSE_PATH)/sprites/MirrorSprite.cpp \
    $(PULSE_PATH)/sprites/Obstacle.cpp \
    $(PULSE_PATH)/sprites/ObstacleIndicator.cpp \
    $(PULSE_PATH)/sprites/ObstaclePhysicsBody.cpp \
    $(PULSE_PATH)/sprites/Player.cpp \
    $(PULSE_PATH)/sprites/Score.cpp \
    $(PULSE_PATH)/sprites/SpritePhysicsBody.cpp \
    $(PULSE_PATH)/sprites/WrappedSprite.cpp \
    $(PULSE_PATH)/ui/Button.cpp \
    $(PULSE_PATH)/ui/MenuSlider.cpp

LOCAL_C_INCLUDES := \
    $(INC_PATH) \
    $(LIB_PATH)/cpputils/include \
    $(LIB_PATH)/GSL/include \
    $(LIB_PATH)/range-v3/include

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
