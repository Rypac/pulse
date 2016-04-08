LOCAL_PATH := $(call my-dir)
ROOT_PATH := $(LOCAL_PATH)/../../..
SRC_PATH := ../../../src/pulse

include $(CLEAR_VARS)

$(call import-add-path,$(ROOT_PATH)/cocos2d)
$(call import-add-path,$(ROOT_PATH)/cocos2d/external)
$(call import-add-path,$(ROOT_PATH)/cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   $(SRC_PATH)/AppDelegate.cpp \
                   $(SRC_PATH)/scenes/DeveloperMenuScene.cpp \
                   $(SRC_PATH)/scenes/GameScene.cpp \
                   $(SRC_PATH)/scenes/PulseGameScene.cpp \
                   $(SRC_PATH)/scenes/SplashScene.cpp \
                   $(SRC_PATH)/sprites/Column.cpp \
                   $(SRC_PATH)/sprites/Pulse.cpp \
                   $(SRC_PATH)/sprites/Obstacle.cpp \
                   $(SRC_PATH)/sprites/SpritePhysicsBody.cpp \
                   $(SRC_PATH)/sprites/ObstaclePhysicsBody.cpp \
                   $(SRC_PATH)/ui/MenuSlider.cpp \
                   $(SRC_PATH)/generators/ObstacleGenerator.cpp

LOCAL_C_INCLUDES := $(ROOT_PATH)/src $(ROOT_PATH)/include

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END

LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
