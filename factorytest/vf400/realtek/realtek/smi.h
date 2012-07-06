
#ifndef __SMI_H__
#define __SMI_H__

#include <stdint.h>

int32_t smi_read(uint32_t mAddrs, uint32_t *rData);
int32_t smi_write(uint32_t mAddrs, uint32_t rData);

#endif /* __SMI_H__ */


