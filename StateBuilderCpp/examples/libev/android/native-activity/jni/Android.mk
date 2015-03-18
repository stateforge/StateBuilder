# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#



TOP_LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(TOP_LOCAL_PATH)/libev/Android.mk

LOCAL_PATH := $(TOP_LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE    := native-activity
LOCAL_SRC_FILES := main.cpp Blink.cpp BlinkFsm.cpp SmAndroidObserver.cpp

#STATEBUILDERCPP_HOME := C:\Users\frederic\AppData\Local\StateForge\StateBuilderCpp
STATEBUILDERCPP_HOME := $(HOME)/StateBuilderCpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/. \
    $(STATEBUILDERCPP_HOME)/src/cpp/include \
	$(LOCAL_PATH)/libev/ 

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM
LOCAL_STATIC_LIBRARIES := android_native_app_glue libev

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)




