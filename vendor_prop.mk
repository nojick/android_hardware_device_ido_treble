
# Art
PRODUCT_PROPERTY_OVERRIDES += \
dalvik.vm.dex2oat-flags=--no-watch-dog


# ACDB vendor
PRODUCT_PROPERTY_OVERRIDES += \
persist.audio.calfile0=/vendor/etc/acdbdata/QRD/QRD_Bluetooth_cal.acdb \
persist.audio.calfile1=/vendor/etc/acdbdata/QRD/QRD_General_cal.acdb \
persist.audio.calfile2=/vendor/etc/acdbdata/QRD/QRD_Global_cal.acdb \
persist.audio.calfile3=/vendor/etc/acdbdata/QRD/QRD_Handset_cal.acdb \
persist.audio.calfile4=/vendor/etc/acdbdata/QRD/QRD_Hdmi_cal.acdb \
persist.audio.calfile5=/vendor/etc/acdbdata/QRD/QRD_Headset_cal.acdb \
persist.audio.calfile6=/vendor/etc/acdbdata/QRD/QRD_Speaker_cal.acdb

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
af.fast_track_multiplier=1 \
vendor.audio_hal.period_size=192 \
audio.deep_buffer.media=true \
persist.vendor.audio.fluence.speaker=true \
persist.vendor.audio.fluence.voicecall=true \
persist.vendor.audio.fluence.voicerec=false \
ro.config.vc_call_vol_steps=6 \
ro.vendor.audio.sdk.fluencetype=fluence \
ro.vendor.audio.sdk.ssr=false \
vendor.tunnel.audio.encode=false \
vendor.voice.path.for.pcm.voip=true \
vidc.enc.narrow.searchrange=1 \
vendor.voice.playback.conc.disabled=true \
vendor.voice.record.conc.disabled=true \
vendor.voice.voip.conc.disabled=true

# Audio offload
PRODUCT_PROPERTY_OVERRIDES += \
vendor.audio.offload.buffer.size.kb=64 \
vendor.audio.offload.gapless.enabled=true \
audio.offload.min.duration.secs=30 \
audio.offload.pcm.16bit.enable=true \
audio.offload.pcm.24bit.enable=true \
audio.offload.video=true \
av.offload.enable=true \
av.streaming.offload.enable=false

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
bluetooth.hfp.client=1 \
qcom.bluetooth.soc=smd \
ro.bluetooth.dun=true \
ro.bluetooth.hfp.ver=1.7 \
ro.bluetooth.sap=true \
ro.qualcomm.bt.hci_transport=smd

# Camera
PRODUCT_PROPERTY_OVERRIDES += \
persist.camera.cpp.duplication=false

# CNE
PRODUCT_PROPERTY_OVERRIDES += \
persist.cne.feature=1 \

# Display
PRODUCT_PROPERTY_OVERRIDES += \
debug.sf.hw=0 \
debug.egl.hw=0 \
persist.hwc.mdpcomp.enable=true \
debug.mdpcomp.logs=0 \
debug.composition.type=c2d \
dev.pm.dyn_samplingrate=1 \
persist.demo.hdmirotationlock=false \
debug.mdpcomp.idletime=600 \
debug.enable.sglscale=1 \
debug.cpurend.vsync=false \
debug.sf.latch_unsignaled=1 \
sys.hwc.gpu_perf_mode=1 \
debug.hwui.renderer=opengl \
ro.opengles.version=196610 \
ro.sf.lcd_density=320

# DRM
PRODUCT_PROPERTY_OVERRIDES += \
drm.service.enabled=true \
vidc.enc.narrow.searchrange=1

# Factory reset protection
PRODUCT_PROPERTY_OVERRIDES += \
ro.frp.pst=/dev/block/bootdevice/by-name/config

# Media
PRODUCT_PROPERTY_OVERRIDES += \
media.msm8939hw=1 \
mm.enable.smoothstreaming=true \
mmp.enable.3g2=true \
media.aac_51_output_enabled=true \
mm.enable.qcom_parser=3407871 \
vendor.mediacodec.binder.size=6

# Netmgrd
PRODUCT_PROPERTY_OVERRIDES += \
ro.use_data_netmgrd=true \
persist.data.netmgrd.qos.enable=true \
persist.data.mode=concurrent

# Perf
PRODUCT_PROPERTY_OVERRIDES += \
ro.vendor.extension_library=libqti-perfd-client.so

# QMI
PRODUCT_PROPERTY_OVERRIDES += \
persist.data.qmi.adb_logmask=0

# Radio
PRODUCT_PROPERTY_OVERRIDES += \
DEVICE_PROVISIONED=1 \
persist.rild.nitz_plmn="" \
persist.rild.nitz_long_ons_0="" \
persist.rild.nitz_long_ons_1="" \
persist.rild.nitz_long_ons_2="" \
persist.rild.nitz_long_ons_3="" \
persist.rild.nitz_short_ons_0="" \
persist.rild.nitz_short_ons_1="" \
persist.rild.nitz_short_ons_2="" \
persist.rild.nitz_short_ons_3="" \
rild.libpath=/vendor/lib64/libril-qc-qmi-1.so \
ro.telephony.call_ring.multiple=false \
ril.subscription.types=NV,RUIM \
persist.radio.apm_sim_not_pwdn=1 \
persist.radio.multisim.config=dsds \
ro.telephony.default_network=9,9 \
persist.radio.add_power_save=1

# SDcardFS
PRODUCT_PROPERTY_OVERRIDES += \
ro.sys.sdcardfs=true

# Wfd
PRODUCT_PROPERTY_OVERRIDES += \
persist.debug.wfd.enable=1

# Time
PRODUCT_PROPERTY_OVERRIDES += \
persist.timed.enable=true

# WIFI
PRODUCT_PROPERTY_OVERRIDES += \
wifi.interface=wlan0

#ro.product.first_api_level indicates the first api level that the device has been commercially launched on.
PRODUCT_PROPERTY_OVERRIDES += \
ro.product.first_api_level=22

# Disable Nav Bar By Default
PRODUCT_PROPERTY_OVERRIDES += \
qemu.hw.mainkeys=1

# Fuse disable
PRODUCT_PROPERTY_OVERRIDES += \
persist.sys.fflag.override.settings_fuse=false

# WFD
PRODUCT_PROPERTY_OVERRIDES += \
    persist.debug.wfd.enable=1 \
    persist.sys.wfd.virtual=0

# Dalvik
PRODUCT_PROPERTY_OVERRIDES += \
pm.dexopt.boot=verify \
pm.dexopt.first-boot=verify \
pm.dexopt.install=speed-profile \
dalvik.vm.image-dex2oat-filter=speed \
dalvik.vm.image-dex2oat-threads=8 \
dalvik.vm.dex2oat-filter=speed \
dalvik.vm.dex2oat-threads=8 \
dalvik.vm.dex2oat64.enabled=true \

# Dalvik
PRODUCT_PROPERTY_OVERRIDES += \
dalvik.vm.dex2oat64.enabled=true

# LMKD
PRODUCT_PRODUCT_PROPERTIES += \
ro.lmk.critical_upgrade=true \
ro.lmk.upgrade_pressure=40 \
ro.lmk.downgrade_pressure=60 \
ro.lmk.thrashing_limit=30 \
ro.lmk.thrashing_limit_decay=50

PRODUCT_PROPERTY_OVERRIDES += \
    ro.charger.enable_suspend = true \
    ro.charger.disable_init_blank=true

