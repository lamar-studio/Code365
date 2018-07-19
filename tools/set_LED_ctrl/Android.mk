LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= set_LED_ctrl.c
#LOCAL_SRC_FILES:= get_LED_ctrl.c

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \

LOCAL_MODULE:= set_LED_ctrl
#LOCAL_MODULE:= get_LED_ctrl

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
