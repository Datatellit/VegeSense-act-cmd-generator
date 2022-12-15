// xlxConfig_Action.h - Xlight Data Structure for Action

#ifndef __XLX_CONFIG_ACTION_H__
#define __XLX_CONFIG_ACTION_H__

#include "xliCommon.h"

/* ָ����1�ֽڶ��� */ 
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
	UC sw;				// ���أ�0-�أ�1-����2-�л���
	US delayopTime;		// ��ʱ����ʱ��
} V_STATUS_t;			// with delayed operation parameters

typedef struct			// 4 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC opt;				// ���Ʒ�ʽ��0-��������ֵ��1-�������ȣ����ֵ����2-�������ȣ����ֵ����
	UC br;				// ���ȣ����Ⱦ���ֵ10-100
	US delayopTime;		// ��ʱ����ʱ��
} V_PERCENTAGE_t;

typedef struct			// 2 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC opt;				// ���Ʒ�ʽ��0-���÷���ֵ��1-���ӣ����ֵ����2-���٣����ֵ����
	UC speed;			// ��λ��1-0��  2-1��  3-2�� 4-3��
} V_WIND_t;

typedef struct			// 5 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC opt;				// ���Ʒ�ʽ��0-����ɫ�»򲨳�ֵ��1-����ɫ�»򲨳������ֵ����2-����ɫ�»򲨳������ֵ����
	US wl;				// ɫ�»򲨳�������ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
	US delayopTime;		// ��ʱ����ʱ��
} V_LEVEL_t;

typedef struct			// 6 Bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC br;				// ���ȣ����Ⱦ���ֵ10-100
	US wl;				// ɫ�»򲨳�������ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
	US delayopTime;		// ��ʱ����ʱ��
	UC opt;				// ��4λΪbr���Ʒ�ʽ����4λ��cct���Ʒ�ʽ
} V_HUE_t;

/* ʱ�� */
#define rSet_ActCmd_DelayDuration(_msg, _val)    BF_SET((_msg).delayTime, _val, 0, 7)
#define mGet_ActCmd_DelayDuration(_msg)         BF_GET((_msg).delayTime, 0, 7)
/* ��λ��0-�룬1-���� */
#define rSet_ActCmd_DelayUnit(_msg, _val)      	BF_SET((_msg).delayTime, _val, 7, 1)
#define mGet_ActCmd_DelayUnit(_msg)           	BF_GET((_msg).delayTime, 7, 1)

typedef struct			// Exact 4 bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC devTag;          // DevTag
	UC cmd;				// Action Command: �μ�COMMAND��Ŀǰ֧�֣�
						// CMD_SERIAL, CMD_POWER, CMD_COLOR, CMD_BRIGHTNESS, CMD_SCENARIO, CMD_CCT
						// ����CMD_SERIAL�൱��PowerSwitch������
	UC delayTime	:8;	// Optional: ��λΪ�룬DTITѹ��ʱ���ֽڸ�ʽ
	//UC duration ��7;
	//UC unit : 1;
	UC paramLen;		// Length (bytes) of parameters (exact length of payload, must shorter than 12 bytes)
} cmdItem_head_t;

typedef union       // Exact 8 bytes
#ifdef PACK
	__attribute__((packed))
#endif
{
	UC						sw;				// ���ٿ���, cmd = CMD_SERIAL
	UC						scenario_id;	// ����ID, cmd = CMD_SCENARIO, devTag = DevTag_GW
	V_STATUS_t 				status;			// ��������, cmd = CMD_POWER, devTag = DevTag_LED
	V_PERCENTAGE_t 			percentage;		// ��������, cmd = CMD_BRIGHTNESS, devTag = DevTag_LED
	V_LEVEL_t				wavL;			// ��������, cmd = CMD_CCT, devTag = DevTag_LED
	V_HUE_t					br_wl;			// ���ȼ�����, cmd = CMD_COLOR, devTag = DevTag_LED
	V_WIND_t				wind;			// ����, cmd = CMD_BRIGHTNESS, devTag = DevTag_Fan
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