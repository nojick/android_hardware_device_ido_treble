
#ifndef CAM_CAPABILITY_LEGACY_H
#define CAM_CAPABILITY_LEGACY_H

/* Раскладка, по которой демона mm-qcamera-daemon заполняет capability-буфер.
 * Константы подтверждены CAPDUMP/CAPSCAN, смещения сверены. */
#define LEGACY_MAX_ZOOMS_CNT            79
#define LEGACY_MAX_SIZES_CNT            30
#define LEGACY_CAM_ISO_MODE_MAX         8
#define LEGACY_CAM_FLASH_MODE_MAX       4
#define LEGACY_CAM_EFFECT_MODE_MAX      12
#define LEGACY_CAM_SCENE_MODE_MAX       22
#define LEGACY_CAM_AEC_MODE_MAX         7
#define LEGACY_CAM_ANTIBANDING_MODE_MAX 6
#define LEGACY_CAM_WB_MODE_MAX          11
#define LEGACY_CAM_FOCUS_MODE_MAX       8
#define LEGACY_CAM_MANUAL_FOCUS_MODE_MAX 4
#define LEGACY_CAM_FORMAT_MAX           36
#define LEGACY_CAM_HFR_MODE_MAX         5
#define LEGACY_CAM_FOCUS_ALGO_MAX       4

typedef struct { uint32_t width; uint32_t height; } legacy_dim_t;
typedef struct { uint32_t mode; legacy_dim_t dim; uint8_t frame_skip;
                 uint8_t ls_cnt; uint16_t pad;
                 legacy_dim_t livesnapshot_sizes_tbl[LEGACY_MAX_SIZES_CNT]; } legacy_hfr_info_t;

typedef struct {
    uint32_t version;                       /* w0   */
    int32_t  position;                      /* w1   */
    uint8_t  auto_hdr_supported;            /* w2   */

    uint32_t supported_iso_modes_cnt;       /* w3   */
    int32_t  supported_iso_modes[LEGACY_CAM_ISO_MODE_MAX];      /* w4-11  */
    int32_t  min_iso, max_iso;              /* w12-13 */
    uint64_t min_exposure_time;             /* w14-15 */
    uint64_t max_exposure_time;             /* w16-17 */
    int32_t  near_end_distance;             /* w18   */

    uint32_t supported_flash_modes_cnt;     /* w19  ✓ */
    int32_t  supported_flash_modes[LEGACY_CAM_FLASH_MODE_MAX];  /* w20-23 */

    uint32_t zoom_ratio_tbl_cnt;            /* w24  ✓ =79 */
    uint32_t zoom_ratio_tbl[LEGACY_MAX_ZOOMS_CNT];              /* w25-103 */

    uint32_t supported_effects_cnt;         /* w104 ✓ =12 */
    int32_t  supported_effects[LEGACY_CAM_EFFECT_MODE_MAX];     /* w105-116 */

    uint32_t supported_scene_modes_cnt;     /* w117 ✓ =20 */
    int32_t  supported_scene_modes[LEGACY_CAM_SCENE_MODE_MAX];  /* w118-139 */

    uint32_t supported_aec_modes_cnt;       /* w140 */
    int32_t  supported_aec_modes[LEGACY_CAM_AEC_MODE_MAX];      /* w141-147 */

    uint32_t fps_ranges_tbl_cnt;            /* w148 */
    float    fps_ranges_tbl[LEGACY_MAX_SIZES_CNT][4];           /* w149-268: min,max,vmin,vmax */

    uint32_t supported_antibandings_cnt;    /* w269 */
    int32_t  supported_antibandings[LEGACY_CAM_ANTIBANDING_MODE_MAX]; /* w270-275 */

    uint32_t supported_white_balances_cnt;  /* w276 */
    int32_t  supported_white_balances[LEGACY_CAM_WB_MODE_MAX];  /* w277-287 */
    int32_t  min_wb_cct, max_wb_cct;        /* w288-289 */
    float    min_wb_gain, max_wb_gain;      /* w290-291 */

    uint32_t supported_focus_modes_cnt;     /* w292 */
    int32_t  supported_focus_modes[LEGACY_CAM_FOCUS_MODE_MAX];  /* w293-300 */
    float    min_focus_pos[LEGACY_CAM_MANUAL_FOCUS_MODE_MAX];   /* w301-304 */
    float    max_focus_pos[LEGACY_CAM_MANUAL_FOCUS_MODE_MAX];   /* w305-308 */

    int32_t  exposure_compensation_min;     /* w309 */
    int32_t  exposure_compensation_max;     /* w310 */
    int32_t  exposure_compensation_default; /* w311 */
    float    exposure_compensation_step;    /* w312 */
    int32_t  exp_comp_step_num, exp_comp_step_den; /* w313-314 */
    uint8_t  video_stablization_supported;  /* w315 */

    uint32_t picture_sizes_tbl_cnt;         /* w316 ✓ */
    legacy_dim_t picture_sizes_tbl[LEGACY_MAX_SIZES_CNT];       /* w317-376 */

    int32_t  modes_supported;               /* w377 */
    uint32_t sensor_mount_angle;            /* w378 */
    float    focal_length;                  /* w379 */
    float    hor_view_angle;                /* w380 */
    float    ver_view_angle;                /* w381 */

    uint32_t preview_sizes_tbl_cnt;         /* w382 ✓ */
    legacy_dim_t preview_sizes_tbl[LEGACY_MAX_SIZES_CNT];       /* w383-442 */

    uint32_t video_sizes_tbl_cnt;           /* w443 ✓ */
    legacy_dim_t video_sizes_tbl[LEGACY_MAX_SIZES_CNT];         /* w444-503 */

    uint32_t livesnapshot_sizes_tbl_cnt;    /* w504 ✓ */
    legacy_dim_t livesnapshot_sizes_tbl[LEGACY_MAX_SIZES_CNT];  /* w505-564 */

    uint32_t vhdr_livesnapshot_sizes_tbl_cnt;                    /* w565 */
    legacy_dim_t vhdr_livesnapshot_sizes_tbl[LEGACY_MAX_SIZES_CNT]; /* w566-625 */

    uint32_t hfr_tbl_cnt;                   /* w626 */
    legacy_hfr_info_t hfr_tbl[LEGACY_CAM_HFR_MODE_MAX];         /* w627-796 */

    uint32_t supported_preview_fmt_cnt;     /* w797 */
    int32_t  supported_preview_fmts[LEGACY_CAM_FORMAT_MAX];     /* w798-833 */
    uint32_t supported_picture_fmt_cnt;     /* w834 */
    int32_t  supported_picture_fmts[LEGACY_CAM_FORMAT_MAX];     /* w835-870 */

    legacy_dim_t raw_dim;                   /* w871-872 */
    uint32_t supported_raw_fmt_cnt;         /* w873 */
    int32_t  supported_raw_fmts[LEGACY_CAM_FORMAT_MAX];         /* w874-909 */

    uint32_t supported_focus_algos_cnt;     /* w910 */
    int32_t  supported_focus_algos[LEGACY_CAM_FOCUS_ALGO_MAX];  /* w911-914 */
} cam_capability_legacy_t;

#endif
