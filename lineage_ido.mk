#
# Copyright (C) 2016 The CyanogenMod Project
#               2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

$(call inherit-product, device/xiaomi/ido/full_ido.mk)

# Inherit some common MK stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)
# Inherit some common Lineage stuff
$(call inherit-product, vendor/lineage/config/common.mk)

PRODUCT_BRAND := Xiaomi
PRODUCT_DEVICE := ido
PRODUCT_MANUFACTURER := Xiaomi
PRODUCT_NAME := lineage_ido
BOARD_VENDOR := Xiaomi

TARGET_VENDOR_PRODUCT_NAME := ido


PRODUCT_GMS_CLIENTID_BASE := android-xiaomi
BUILD_FINGERPRINT := "Xiaomi/ido/ido:5.1.1/LMY47V/V8.1.3.0.LAIMIDI:user/release-keys"
# Build fingerprint
PRODUCT_BUILD_PROP_OVERRIDES += \
    PRIVATE_BUILD_DESC="ido-user 5.1.1 LMY47V V8.1.3.0.LAIMIDI release-keys"

PRODUCT_SYSTEM_PROPERTY_BLACKLIST += \
    ro.product.model

