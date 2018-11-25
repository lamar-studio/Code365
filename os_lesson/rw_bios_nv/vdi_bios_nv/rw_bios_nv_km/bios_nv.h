#ifndef _bios_H
#define _bios_H
#pragma pack(1)
typedef signed char     INT8;
typedef unsigned char   UINT8;
typedef signed short    INT16;
typedef unsigned short  UINT16;
typedef signed int      INT32;
typedef unsigned int    UINT32;
typedef unsigned char   CHAR8;
typedef unsigned short  CHAR16;
#define VOID            void
#define IN
#define OUT
#define GET_BIOS_NV_CMD    0xF3
#define SET_BIOS_NV_CMD    0xF4

#define SW_SMI_FUNCTION_NOT_SUPPORTED 0x82
#define AC_POWER_VAR_NAME             0x00

typedef struct {
    UINT16		var_name;
    UINT16      var_value;
    UINT16		RetStatus;
} GET_VAR_STRUCTURE;


typedef struct {
    UINT16		var_name;
    UINT16      var_value;
    UINT16		RetStatus;
} SET_VAR_STRUCTURE;

#pragma pack()
#endif