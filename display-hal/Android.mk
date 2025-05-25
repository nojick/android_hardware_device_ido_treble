display-hals := libcopybit libmemtrack libqservice libqdutils
ifneq ($(TARGET_USES_GRALLOC1), true)
    display-hals += libgralloc
else
    display-hals += libgralloc1
endif

display-hals += hdmi_cec
sdm-libs := sdm/libs
display-hals += $(sdm-libs)/utils $(sdm-libs)/core $(sdm-libs)/hwc $(sdm-libs)/hwc2

ifneq (,$(call is-vendor-board-qcom))
    include $(call all-named-subdir-makefiles,$(display-hals))
else
ifneq ($(filter msm% apq%,$(TARGET_BOARD_PLATFORM)),)
    include $(call all-named-subdir-makefiles,$(display-hals))
endif
endif

include $(CLEAR_VARS)
LOCAL_MODULE := display_headers
LOCAL_EXPORT_C_INCLUDE_DIRS := \
    $(display_top)/libcopybit \
    $(display_top)/libgralloc \
    $(display_top)/libqdutils \
    $(display_top)/libqservice \
    $(display_top)/sdm/include \

include $(BUILD_HEADER_LIBRARY)
