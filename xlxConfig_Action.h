// xlxConfig_Action.h - Xlight Data Structure for Action

#ifndef __XLX_CONFIG_ACTION_H__
#define __XLX_CONFIG_ACTION_H__

#include "xliCommon.h"

/* 指定按1字节对齐 */ 
#pragma pack(1)

// Maximum commands within an action
#ifndef MAX_COMMAND_PER_ACTION
#define MAX_COMMAND_PER_ACTION      16
#endif

#ifndef PACK
#define PACK
#endif

//------------------------------------------------------------------
// VegeSense Action Table Structures
//------------------------------------------------------------------
typedef struct			// 3 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC sw;				// 开关，0-关，1-开，2-切换；
	US delayopTime;		// 延时操作时间
} V_STATUS_t;			// with delayed operation parameters

typedef struct			// 4 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC opt;				// 控制方式，0-设置亮度值，1-增加亮度（相对值），2-减少亮度（相对值）；
	UC br;				// 亮度，亮度具体值10-100
	US delayopTime;		// 延时操作时间
} V_PERCENTAGE_t;

typedef struct			// 2 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC opt;				// 控制方式，0-设置风速值，1-增加（相对值），2-减少（相对值）；
	UC speed;			// 档位，1-0档  2-1档  3-2档 4-3档
} V_WIND_t;

typedef struct			// 5 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC opt;				// 控制方式，0-设置色温或波长值，1-增加色温或波长（相对值），2-减少色温或波长（相对值）；
	US wl;				// 色温或波长，具体色温值2700-6500占2个字节，高8位在前，低8位在后
	US delayopTime;		// 延时操作时间
} V_LEVEL_t;

typedef struct			// 6 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC br;				// 亮度，亮度具体值10-100
	US wl;				// 色温或波长，具体色温值2700-6500占2个字节，高8位在前，低8位在后
	US delayopTime;		// 延时操作时间
	UC opt;				// 低4位为br控制方式，高4位是cct控制方式
} V_HUE_t;

/* 时长 */
#define rSet_ActCmd_DelayDuration(_msg, _val)    BF_SET((_msg).delayTime, _val, 0, 7)
#define mGet_ActCmd_DelayDuration(_msg)         BF_GET((_msg).delayTime, 0, 7)
/* 单位：0-秒，1-分钟 */
#define rSet_ActCmd_DelayUnit(_msg, _val)      	BF_SET((_msg).delayTime, _val, 7, 1)
#define mGet_ActCmd_DelayUnit(_msg)           	BF_GET((_msg).delayTime, 7, 1)

typedef struct			// Exact 4 bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC devTag;          // DevTag
	UC cmd;				// Action Command: 参见COMMAND，目前支持：
						// CMD_SERIAL, CMD_POWER, CMD_COLOR, CMD_BRIGHTNESS, CMD_SCENARIO, CMD_CCT
						// 其中CMD_SERIAL相当于PowerSwitch云命令
	UC delayTime	:8;	// Optional: 单位为秒，DTIT压缩时长字节格式
	//UC duration ：7;
	//UC unit : 1;
	UC paramLen;		// Length (bytes) of parameters (exact length of payload, must shorter than 12 bytes)
} cmdItem_head_t;

typedef union       // Exact 8 bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC						sw;				// 快速开关, cmd = CMD_SERIAL
	UC						scenario_id;	// 场景ID, cmd = CMD_SCENARIO, devTag = DevTag_GW
	V_STATUS_t 				status;			// 照明开关, cmd = CMD_POWER, devTag = DevTag_LED
	V_PERCENTAGE_t 			percentage;		// 照明亮度, cmd = CMD_BRIGHTNESS, devTag = DevTag_LED
	V_LEVEL_t				wavL;			// 照明波长, cmd = CMD_CCT, devTag = DevTag_LED
	V_HUE_t					br_wl;			// 亮度及波长, cmd = CMD_COLOR, devTag = DevTag_LED
	V_WIND_t				wind;			// 风速, cmd = CMD_BRIGHTNESS, devTag = DevTag_Fan
	UC data[8];
} cmdItem_body_t;

// 12 Bytes
typedef struct
#ifdef PACK
	__attribute__((packed))
#endif
{
	cmdItem_head_t head;       	// Exact 4 bytes
	cmdItem_body_t body;       	// Exact 8 bytes
} cmdItem_t;

// Action Commands
typedef struct
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC uid                   : 8;		// 1-based index for physical storage, not necessary, only used for validation
	US crc16;
	cmdItem_t list[MAX_COMMAND_PER_ACTION];		// 16 * 12 = 192
} ActionCmd_t;

// Information of an action object
typedef struct			// Exact 4 bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC uid                   : 8;		// 1-based index for physical storage, not necessary, only used for validation
	UC cmdCount;
	UC calledTimes;
	UC reserved;
} ActionInfo_t;

#define ACT_ROW_SIZE		256			// Size of each action record
#define MAX_ACT_ROWS		126			// 128 * 256 bytes + 512 (ActionInfo) = 32KB

// Action List Type
// 126 * 4 Bytes = 504 Bytes
typedef struct
#ifdef PACK
	__attribute__((packed))
#endif
{
	ActionInfo_t list[MAX_ACT_ROWS];
} ActionList_t;

#endif /* __XLX_CONFIG_ACTION_H__ */