#ifndef _RW_BIOS_NV_USER_H
#define _RW_BIOS_NV_USER_H

#include "bios_nv.h"

/* 定义幻数 */
#define RW_BIOS_NV_IOC_MAGIC  'k'

/* 定义命令 */
#define RW_BIOS_NV_IOCPRINT      _IO(RW_BIOS_NV_IOC_MAGIC, 1)
#define RW_BIOS_NV_IOCGETDATA    _IOR(RW_BIOS_NV_IOC_MAGIC, 2, unsigned long)

#define VDI_RW_BIOS_NV_IOCGETBIOSNV  _IOW(RW_BIOS_NV_IOC_MAGIC, 3, GET_VAR_STRUCTURE)
#define VDI_RW_BIOS_NV_IOCSETBIOSNV  _IOW(RW_BIOS_NV_IOC_MAGIC, 4, SET_VAR_STRUCTURE)

#define IDV_RW_BIOS_NV_IOCGETBIOSNV  _IOW(RW_BIOS_NV_IOC_MAGIC, 3, BIOS_VAR_STRUCTURE)
#define IDV_RW_BIOS_NV_IOCSETBIOSNV  _IOW(RW_BIOS_NV_IOC_MAGIC, 4, BIOS_VAR_STRUCTURE)


#define RW_BIOS_NV_IOC_MAXNR 4

// vdi
void vdi_bios_nv_read_user(unsigned int nv_name, unsigned int *nv_value);
void vdi_bios_nv_write_user(unsigned int nv_name, unsigned int nv_value);

// idv
void idv_bios_nv_read_user(unsigned int nv_name, unsigned int *nv_value);
void idv_bios_nv_write_user(unsigned int nv_name, unsigned int nv_value);

#endif
