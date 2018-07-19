LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SHARED_LIBRARIES := \
        libutils libcutils liblog
LOCAL_SRC_FILES := base.c net_test.c
LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/interface_include
LOCAL_CFLAGS := -fpic
LOCAL_MODULE := networktest
include $(BUILD_EXECUTABLE)
