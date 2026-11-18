/* parm_shim.c - ABI converter: new expanded metadata_buffer_t (HAL)
 * <-> old transitional parm_buffer_new_t (msm8916 daemon blob).
 * Old-side layout is hard-coded (32-bit ARM).
 * Indices/sizes/accessors come from generated meta_remap.h. */

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <pthread.h>
#include <sys/mman.h>
#include <cutils/ashmem.h>

#include "cam_intf.h"
#include "meta_remap.h"

/* transport lives in mm_camera.c of the same library */
extern int32_t mm_camera_map_buf(void *my_obj, uint8_t buf_type,
                                 int fd, size_t size);

/* ---- old transitional layout, 32-bit ARM, hard-coded ---- */
typedef struct {
    uint32_t entry_type;     /* old parameter id */
    uint32_t size;
    uint32_t aligned_size;   /* FULL stride incl. header (GET_NEXT_PARAM) */
    uint8_t  data[1];
} old_entry_t;               /* data at offset 12 */

typedef struct {
    uint32_t num_entry;
    uint32_t tot_rem_size;
    uint32_t curr_size;
    uint8_t  entry[1];
} old_buf_t;                 /* first entry at offset 12 */

#define OLD_HDR     12u
#define OLD_ALIGN(x) (((x) + 3u) & ~3u)
#define OLD_IDX_HAL_VERSION 0u
#define SHIM_MIN(a,b) ((a) < (b) ? (a) : (b))

/* ============ HAL -> daemon ============ */

static int add_entry(old_buf_t *b, uint32_t old_id, const void *val, uint32_t sz)
{
    uint32_t stride = OLD_ALIGN(OLD_HDR + sz);
    if (b->curr_size + stride > b->tot_rem_size) return -1;
    old_entry_t *e = (old_entry_t *)(b->entry + b->curr_size);
    e->entry_type = old_id;
    e->size = sz;
    e->aligned_size = stride;
    if (sz && val) memcpy(e->data, val, sz);
    b->curr_size += stride;
    b->num_entry++;
    return 0;
}

/* CAM_INTF_PARM_HDR: new cam_exp_bracketing_t -> old cam_hdr_param_t */
static void conv_hdr_to_old(const void *src, void *dst)
{
    const cam_exp_bracketing_t *in = (const cam_exp_bracketing_t *)src;
    uint32_t *o = (uint32_t *)dst;      /* {hdr_enable, hdr_need_1x, hdr_mode} */
    o[0] = (in->mode != CAM_EXP_BRACKETING_OFF) ? 1u : 0u;
    o[1] = 1u;                          /* hdr_need_1x */
    o[2] = 1u;                          /* CAM_HDR_MODE_MULTIFRAME */
}

/* META_AEC_ROI / AF_ROI / AWB_REGIONS: cam_area_t[1] -> cam_area_t[5] */
static void conv_roi1_to_5(const void *src, void *dst, uint32_t old_sz)
{
    memset(dst, 0, old_sz);
    memcpy(dst, src, sizeof(cam_area_t));
}

int32_t shim_parms_hal_to_daemon(const metadata_buffer_t *in, void *out,
                                 size_t out_size, uint8_t is_get)
{
    old_buf_t *b = (old_buf_t *)out;
    if (!in || !out || out_size < OLD_HDR) return -1;

    memset(out, 0, out_size);
    b->num_entry = 0;
    b->tot_rem_size = (uint32_t)out_size - OLD_HDR;
    b->curr_size = 0;

    /* FIRST entry is always HAL_VERSION - daemon unpacks the rest by it */
    int32_t ver = CAM_HAL_V1;
    if (add_entry(b, OLD_IDX_HAL_VERSION, &ver, sizeof(ver)) < 0) return -1;

    for (uint32_t i = 0; i < CAM_INTF_PARM_MAX; i++) {
        const remap_t *r = &g_remap[i];
        if (r->old_idx == OLD_IDX_NONE || r->old_size == 0) continue;
        if (r->old_idx == OLD_IDX_HAL_VERSION) continue;  /* already sent */
        if (!in->is_valid[i]) continue;

        if (is_get) {
            /* get request: entry with empty payload */
            if (add_entry(b, r->old_idx, NULL, 0) < 0) return -1;
            continue;
        }


        const void *src = shim_get_src(in, i);
        if (!src) continue;
        uint32_t nsz = shim_get_nsz(in, i);
        if (!nsz) continue;

        switch (r->conv) {
        case CONV_SAME:
            add_entry(b, r->old_idx, src, SHIM_MIN(r->old_size, nsz));
            break;
            /* SAME means old_size == new field size by construction */
            add_entry(b, r->old_idx, src, r->old_size);
            break;
        case CONV_U8_TO_U32: {   /* old 1 byte <- new uint32 */
            uint8_t v = (uint8_t)(*(const uint32_t *)src);
            add_entry(b, r->old_idx, &v, 1);
            break; }
        case CONV_U32_TO_U8: {   /* old 4 bytes <- new uint8 */
            uint32_t v = (uint32_t)(*(const uint8_t *)src);
            add_entry(b, r->old_idx, &v, 4);
            break; }
        case CONV_MANUAL:
            if (i == CAM_INTF_PARM_HDR) {
                uint8_t tmp[16];
                conv_hdr_to_old(src, tmp);
                add_entry(b, r->old_idx, tmp, r->old_size);
            } else if (i == CAM_INTF_META_AEC_ROI || i == CAM_INTF_META_AF_ROI ||
                       i == CAM_INTF_META_AWB_REGIONS) {
                uint8_t tmp[128];
                conv_roi1_to_5(src, tmp, r->old_size);
                add_entry(b, r->old_idx, tmp, r->old_size);
            }
            /* прочие MANUAL демону неизвестны - пропускаем */
            break;
        }
    }
    return 0;
}

/* ============ daemon -> HAL ============ */

int32_t shim_parms_daemon_to_hal(const void *in, metadata_buffer_t *out)
{
    if (!in || !out) return -1;
    const old_buf_t *b = (const old_buf_t *)in;
    uint32_t off = 0;

    for (uint32_t k = 0; k < b->num_entry && off + OLD_HDR <= b->curr_size; k++) {
        const old_entry_t *e = (const old_entry_t *)(b->entry + off);
        if (e->entry_type >= OLD_PARM_MAX) return -1;
        if (e->aligned_size < OLD_HDR) return -1;
        off += e->aligned_size;

        /* empty entry = get request, not a response */
        if (e->size == 0 || e->entry_type == OLD_IDX_HAL_VERSION) continue;

        const remap_rev_t *rr = &g_remap_rev[e->entry_type];
        if (rr->new_idx == REV_NEW_NONE) continue;

        uint32_t ni = rr->new_idx;
        const remap_t *r2 = &g_remap[ni];

        switch (r2->conv) {
        case CONV_SAME:
            shim_set_dst(out, ni, e->data, SHIM_MIN(e->size, r2->old_size));
            break;
        case CONV_U8_TO_U32: {   /* new uint32 <- old 1 byte, zero-padded */
            uint8_t z[4] = {0, 0, 0, 0};
            memcpy(z, e->data, SHIM_MIN(e->size, 4u));
            shim_set_dst(out, ni, z, 4u);
            break; }
        case CONV_U32_TO_U8:     /* new uint8 <- old 4 bytes, low byte (LE) */
            shim_set_dst(out, ni, e->data, 1);
            break;
        case CONV_MANUAL:
            if (ni == CAM_INTF_PARM_HDR) {
                const uint32_t *o = (const uint32_t *)e->data;
                cam_exp_bracketing_t v;
                memset(&v, 0, sizeof(v));
                v.mode = o[0] ? CAM_EXP_BRACKETING_ON : CAM_EXP_BRACKETING_OFF;
                shim_set_dst(out, ni, &v, sizeof(v));
            } else if (ni == CAM_INTF_META_AEC_ROI || ni == CAM_INTF_META_AF_ROI ||
                       ni == CAM_INTF_META_AWB_REGIONS) {
                shim_set_dst(out, ni, e->data, sizeof(cam_area_t)); /* 1-я область */
            } else if (ni == CAM_INTF_META_SENSOR_TIMESTAMP) {
                const uint32_t *o = (const uint32_t *)e->data; /* tv_sec, tv_usec */
                int64_t v = (int64_t)o[0] * 1000000000LL + (int64_t)o[1] * 1000LL;
                shim_set_dst(out, ni, &v, sizeof(v));
            }
            break;
        }
        out->is_valid[ni] = 1;
    }
    return 0;
}

/* ============ capability: заглушка kill-test ============ */
int32_t shim_capability_daemon_to_hal(void *in, size_t size)
{
    memset(in, 0, size);   /* HAL получит нули вместо старой структуры */
    return 0;
}

/* ============ mapping PARM_BUF (single shared buffer) ============ */

#define SHIM_MAX_CAMS 4
typedef struct {
    uint32_t cam;
    int      fd;
    uint8_t *addr;
    uint8_t  mapped;
} shim_cam_t;
static shim_cam_t g_shim[SHIM_MAX_CAMS];
static pthread_mutex_t g_shim_lock = PTHREAD_MUTEX_INITIALIZER;

static shim_cam_t *shim_state(uint32_t cam)
{
    for (int i = 0; i < SHIM_MAX_CAMS; i++)
        if (g_shim[i].cam == cam) return &g_shim[i];
    for (int i = 0; i < SHIM_MAX_CAMS; i++)
        if (!g_shim[i].cam) { g_shim[i].cam = cam; return &g_shim[i]; }
    return NULL;
}

int32_t shim_ensure_mapped(void *my_obj)
{
    int32_t rc = -1;
    pthread_mutex_lock(&g_shim_lock);
    shim_cam_t *s = shim_state((uint32_t)(uintptr_t)my_obj);
    if (!s) goto unlock;
    if (s->mapped) { rc = 0; goto unlock; }

    s->fd = ashmem_create_region("cam_shim_parm", SHIM_SETBUF_SIZE);
    if (s->fd < 0) goto unlock;
    s->addr = (uint8_t *)mmap(NULL, SHIM_SETBUF_SIZE,
        PROT_READ | PROT_WRITE, MAP_SHARED, s->fd, 0);
    if (s->addr == MAP_FAILED) { s->addr = NULL; goto unlock; }

    /* PARM_BUF == 1 в enum'ах обеих эр - демону достаточно номера */
    rc = mm_camera_map_buf(my_obj, CAM_MAPPING_BUF_TYPE_PARM_BUF,
                           s->fd, SHIM_SETBUF_SIZE);
    if (rc == 0) s->mapped = 1;

unlock:
    pthread_mutex_unlock(&g_shim_lock);
    return rc;
}

void *shim_parmbuf(void *my_obj)
{
    shim_cam_t *s = shim_state((uint32_t)(uintptr_t)my_obj);
    return (s && s->mapped) ? s->addr : NULL;
}

