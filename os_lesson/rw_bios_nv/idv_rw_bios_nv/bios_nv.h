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
	    UINT16      var_value;		//0->power on 1->power off
		UINT16      var_value_RtcWake; //0->Disabled 1->Fixed Time 2->Dynamic Time
		UINT16      var_value_hour; //Wake up hour 0~23 (Fixed Time)
		UINT16      var_value_minute; //Wake up minute 0~59 (Fixed Time)
		UINT16      var_value_second; //Wake up second 0~59 (Fixed Time)
		UINT16      var_value_MinIncrease; //Wake up minute increase 1~5 (Dynamic Time)
	    UINT16		RetStatus;
} BIOS_VAR_STRUCTURE;

typedef struct {
	    UINT16		var_name;
	    UINT16      var_value;		//0->power on 1->power off
		UINT16      var_value_RtcWake; //0->Disabled 1->Fixed Time 2->Dynamic Time
		UINT16      var_value_hour; //Wake up hour 0~23 (Fixed Time)
		UINT16      var_value_minute; //Wake up minute 0~59 (Fixed Time)
		UINT16      var_value_second; //Wake up second 0~59 (Fixed Time)
		UINT16      var_value_MinIncrease; //Wake up minute increase 1~5 (Dynamic Time)
	    UINT16		RetStatus;
} BIOS_VAR_STRUCTURE;

#pragma pack()
#endif
