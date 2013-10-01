LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_ARM_MODE := arm
LOCAL_CPPFLAGS := $(OPENVX_DEFS)
LOCAL_SRC_FILES := openvx_jni.cpp \
                   Context.cpp \
                   Scalar.cpp \
                   Buffer.cpp \
                   Image.cpp \
                   Kernel.cpp \
                   Graph.cpp \
                   Node.cpp \
                   Parameter.cpp \
                   Reference.cpp \
                   Target.cpp
LOCAL_C_INCLUDES += $(JNI_H_INCLUDE) $(OPENVX_INC)
LOCAL_SHARED_LIBRARIES += \
    libopenvx \
    libvxu \
    libcutils \
    libhardware \
    libhardware_legacy \
    libnativehelper \
    libsystem_server \
    libutils \
    libui \
    libdl \
    libbinder \
    libgui \
    libmedia

ifeq ($(TARGET_SIMULATOR),true)
ifeq ($(TARGET_OS),linux)
ifeq ($(TARGET_ARCH),x86)
LOCAL_LDLIBS += -lpthread -ldl -lrt
endif
endif
endif
ifeq ($(WITH_MALLOC_LEAK_CHECK),true)
    LOCAL_CFLAGS += -DMALLOC_LEAK_CHECK
endif
LOCAL_MODULE:= libopenvx_jni
include $(BUILD_SHARED_LIBRARY)
