LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cmopengl
LOCAL_SRC_FILES := ./opengles/cmopengl_jniwrap.c \
					./opengles/cmshader.c \
					./opengles/matrix.c \
					./opengles/esUtil.c
LOCAL_LDLIBS += -lGLESv2 \
				-llog


#use GL ext model
LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
include $(BUILD_SHARED_LIBRARY)