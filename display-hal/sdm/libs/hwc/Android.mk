LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
include $(LOCAL_PATH)/../../../common.mk
ifeq ($(use_hwc2),false)

LOCAL_MODULE                  := hwcomposer.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_RELATIVE_PATH    := hw
LOCAL_VENDOR_MODULE      := true
LOCAL_MODULE_TAGS             := optional
LOCAL_HEADER_LIBRARIES        := display_headers

LOCAL_CFLAGS                  := -Wno-missing-field-initializers -Wno-unused-parameter \
                                 -std=c++11 -fcolor-diagnostics\
                                 -DLOG_TAG=\"SDM\" $(common_flags)
LOCAL_CLANG                   := true

LOCAL_SHARED_LIBRARIES        := libsdmcore libqservice libbinder libhardware \
                                 libutils liblog libcutils libsync libmemalloc libqdutils libdl \
                                 libsdmutils libc++

# Allow implicit fallthroughs in hwc_display.cpp until they are fixed.
LOCAL_CFLAGS                  += -Wno-implicit-fallthrough -Wno-sign-conversion -Wno-shorten-64-to-32

LOCAL_SRC_FILES               := hwc_session.cpp \
                                 hwc_display.cpp \
                                 hwc_display_null.cpp \
                                 hwc_display_primary.cpp \
                                 hwc_display_external.cpp \
                                 hwc_display_virtual.cpp \
                                 hwc_debugger.cpp \
                                 hwc_buffer_allocator.cpp \
                                 hwc_buffer_sync_handler.cpp \
                                 hwc_color_manager.cpp \
                                 blit_engine_c2d.cpp \
                                 cpuhint.cpp \
                                 android/uevent.cpp

include $(BUILD_SHARED_LIBRARY)
endif
