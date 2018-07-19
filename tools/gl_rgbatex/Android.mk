LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	gl_rgbatex.cpp

LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libEGL \
    libGLESv1_CM \
    libutils \
    libgui \
    libui \
    libandroid_runtime\
    libskia \
    libEGL \
    libGLESv2 \
    libandroid

LOCAL_C_INCLUDES += frameworks/base/core/jni/android/graphics \
					hardware/rk29/libgralloc_ump

LOCAL_MODULE:= libglrgbatexure

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS := -DGL_GLEXT_PROTOTYPES -DEGL_EGLEXT_PROTOTYPES

#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
