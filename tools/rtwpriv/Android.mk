ifeq ($(TARGET_ARCH),arm)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := rtwpriv.c

LOCAL_CFLAGS := -DANDROID_CHANGES

LOCAL_MODULE := rtwpriv

LOCAL_MODULE_TAGS := debug

LOCAL_CERTIFICATE := platform

include $(BUILD_EXECUTABLE)

endif
