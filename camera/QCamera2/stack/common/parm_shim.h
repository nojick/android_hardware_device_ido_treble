#ifndef PARM_SHIM_H
#define PARM_SHIM_H

#include "cam_intf.h"
#include "meta_remap.h"   /* SHIM_SETBUF_SIZE, g_remap/g_remap_rev, accessors */

/* HAL(new ABI) -> daemon(old ABI). is_get=1: entries with size 0 (get request) */
int32_t shim_parms_hal_to_daemon(const metadata_buffer_t *in, void *out,
                                 size_t out_size, uint8_t is_get);
/* daemon(old ABI) -> HAL(new ABI) */
int32_t shim_parms_daemon_to_hal(const void *in, metadata_buffer_t *out);
/* пока заглушка kill-test: обнуляет буфер capability */
int32_t shim_capability_daemon_to_hal(void *in, size_t size);

int32_t shim_ensure_mapped(void *my_obj);

/* при закрытии камеры: освободить слот шима */
void shim_release(void *my_obj);
void *shim_parmbuf(void *my_obj);

#endif /* PARM_SHIM_H */

