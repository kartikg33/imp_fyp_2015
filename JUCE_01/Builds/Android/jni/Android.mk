# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI), armeabi-v7a)
    LOCAL_ARM_MODE := arm
endif

LOCAL_MODULE := juce_jni
LOCAL_SRC_FILES := \
  ../../../Source/AddBoardButton.cpp\
  ../../../Source/PAWSBoard_UI.cpp\
  ../../../Source/MainComponent.cpp\
  ../../../Source/Main.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_audio_basics/juce_audio_basics.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_audio_devices/juce_audio_devices.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_audio_formats/juce_audio_formats.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_audio_processors/juce_audio_processors.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_audio_utils/juce_audio_utils.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_core/juce_core.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_cryptography/juce_cryptography.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_data_structures/juce_data_structures.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_events/juce_events.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_graphics/juce_graphics.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_gui_basics/juce_gui_basics.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_gui_extra/juce_gui_extra.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_opengl/juce_opengl.cpp\
  ../../../../../../../../../../Volumes/Data/JUCE-OSX/modules/juce_video/juce_video.cpp\

ifeq ($(NDK_DEBUG),1)
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../../../../../../../Volumes/Data/JUCE-OSX/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=8" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_PAWS_audio_01" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/PAWS_audio_01\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -g -I "../../JuceLibraryCode" -I "../../../../../../../../../Volumes/Data/JUCE-OSX/modules" -O0 -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=8" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_PAWS_audio_01" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/PAWS_audio_01\" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
else
  LOCAL_CPPFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../../../../../../../Volumes/Data/JUCE-OSX/modules" -Os -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=8" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_PAWS_audio_01" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/PAWS_audio_01\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
  LOCAL_CFLAGS += -fsigned-char -fexceptions -frtti -I "../../JuceLibraryCode" -I "../../../../../../../../../Volumes/Data/JUCE-OSX/modules" -Os -std=c++11 -std=gnu++11 -D "JUCE_ANDROID=1" -D "JUCE_ANDROID_API_VERSION=8" -D "JUCE_ANDROID_ACTIVITY_CLASSNAME=com_yourcompany_PAWS_audio_01" -D JUCE_ANDROID_ACTIVITY_CLASSPATH=\"com/yourcompany/PAWS_audio_01\" -D "NDEBUG=1" -D "JUCER_ANDROID_7F0E4A25=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000"
  LOCAL_LDLIBS := -llog -lGLESv2
endif

include $(BUILD_SHARED_LIBRARY)
