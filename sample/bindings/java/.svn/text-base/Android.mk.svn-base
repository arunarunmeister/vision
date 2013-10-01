LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := $(call all-java-files-under, src)
LOCAL_JNI_SHARED_LIBRARIES := libopenvx_jni
LOCAL_MODULE := org.khronos.openvx
LOCAL_CERTIFICATE := platform
LOCAL_SDK_VERSION := current
LOCAL_PROGUARD_FLAG_FILES := procfg.txt
LOCAL_PROGUARD_ENABLED := disabled
include $(BUILD_STATIC_JAVA_LIBRARY)

include $(LOCAL_PATH)/jni/Android.mk
