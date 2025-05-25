LOCAL_PATH := $(call my-dir)
include $(LOCAL_PATH)/../common.mk
include $(CLEAR_VARS)

LOCAL_MODULE                  := libqservice
LOCAL_VENDOR_MODULE      := true
LOCAL_MODULE_TAGS             := optional
LOCAL_HEADER_LIBRARIES        := display_headers generated_kernel_headers libcutils_headers libbinder_headers1
LOCAL_SHARED_LIBRARIES        := $(common_libs) libbinder_vendor
LOCAL_CFLAGS                  := $(common_flags) -DLOG_TAG=\"qdqservice\" -Wno-sign-conversion
LOCAL_CFLAGS                  += -Wno-error
LOCAL_CLANG                   := true
LOCAL_ADDITIONAL_DEPENDENCIES := $(common_deps)
LOCAL_SRC_FILES               := QService.cpp \
                                 IQService.cpp \
                                 IQClient.cpp \
                                 IQHDMIClient.cpp
LOCAL_EXPORT_C_INCLUDE_DIRS   := $(LOCAL_PATH)


include $(BUILD_SHARED_LIBRARY)
