#include "actcmdInterface.h"

CActCmdInterface::CActCmdInterface()
{
    m_pCmdList = (cmdItem_t *)(m_msg + SCMSG_HEADER_LEN);
}

/***************************************************************
* @func		Init
* @brief    接口初始化：每次生成新的act-cmd命令列表前调用
* @param
*   _uid：要生成Act-cmd命令组的uid，需要与Action uid对应(1到126之间)
*   _cmdCnt：要生成Act-cmd命令组包含的命令数量(1至16之间)
* @retval	
***************************************************************/
void CActCmdInterface::Init(const UC _uid, const UC _cmdCnt) {
    // 数据初始化
    memset(m_msg, 0x00, sizeof(m_msg));
    memset(m_strOutput, 0x00, sizeof(m_strOutput));    
    m_msg[0] = ACT_CMD_MSG_HEAD_0;      // 'C'
    m_msg[1] = ACT_CMD_MSG_HEAD_1;      // 'F'
    m_msg[2] = 1;                       // total_pack
    m_msg[3] = 1;                       // current_pack
    m_nCmdCnt = _cmdCnt;
    m_nPayloadLen = _cmdCnt * sizeof(cmdItem_t);
    m_msg[4] = m_nPayloadLen >> 8;      // len_h
    m_msg[5] = m_nPayloadLen & 0xFF;    // len_low
    m_msg[6] = _uid;
    m_nCmdIndex = 0;
}

/***************************************************************
* @func		AddCmd_PowerSwitch
* @brief    添加命令：快速开关命令
* @param
*   _devTag：DevTag，参见eDeviceTag_t
*   _sw: 开关状态，取值[0..2]，既DEVICE_SW_OFF, DEVICE_SW_ON, DEVICE_SW_TOGGLE
*   _type：命令类型，可选参数，目前不需要
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_PowerSwitch(const UC _devTag, const UC _sw, const UC _type) {
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = _devTag;                  // DevTag
    lv_cmd->head.cmd = CMD_SERIAL;                  // 快速开关
    lv_cmd->head.type = _type;                      // 命令类型
    lv_cmd->head.paramLen = 1;                      // 快速开关命令参数长度
    lv_cmd->body.sw = _sw;                          // 开关状态

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_ChangeScenario
* @brief    添加命令：场景切换命令
* @param    _scenario：场景ID
*           _type：命令类型，可选参数，目前不需要
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_ChangeScenario(const UC _scenario, const UC _type)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_GW;                // DevTag必须为DevTag_GW
    lv_cmd->head.cmd = CMD_SCENARIO;                // 切换场景
    lv_cmd->head.type = _type;                      // 命令类型
    lv_cmd->head.paramLen = 1;                      // 场景切换命令参数长度
    lv_cmd->body.scenario_id = _scenario;           // 场景ID

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_SetState
* @brief    添加命令：设备开关命令
* @param
*   _devTag：DevTag，参见eDeviceTag_t
*   _sw: 开关状态，取值[0..2]，既DEVICE_SW_OFF, DEVICE_SW_ON, DEVICE_SW_TOGGLE
*   _type：命令类型，可选参数，目前不需要
*   _opTime: 渐变操作时长（秒），可选参数，仅对DevTag_LED设备有效
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_SetState(const UC _devTag, const UC _sw, const UC _type, const US _opTime)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = _devTag;                  // DevTag
    lv_cmd->head.cmd = CMD_POWER;                   // 设备开关
    lv_cmd->head.type = _type;                      // 命令类型
    // 设备开关命令参数长度
    if(_devTag == DevTag_LED) {
        lv_cmd->head.paramLen = sizeof(V_STATUS_t);
        lv_cmd->body.status.sw = _sw;               // 开关状态
        lv_cmd->body.status.delayopTime = _opTime;  // 渐变操作时长（秒）
    } else {
        lv_cmd->head.paramLen = 1;
        lv_cmd->body.sw = _sw;                      // 开关状态
    }

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_BRControl
* @brief    添加命令：照明亮度
* @param
*   _br：亮度，亮度具体值0-100
*   _op：控制方式，0-设置亮度值，1-增加亮度（相对值），2-减少亮度（相对值）；
*   _type：命令类型，可选参数，目前不需要
*   _opTime: 渐变操作时长（秒），可选参数，仅对DevTag_LED设备有效
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_BRControl(const UC _br, const UC _op, const UC _type, const US _opTime)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_LED;               // DevTag必须为DevTag_LED
    lv_cmd->head.cmd = CMD_BRIGHTNESS;              // 级别调整
    lv_cmd->head.type = _type;                      // 命令类型
    lv_cmd->head.paramLen = sizeof(V_PERCENTAGE_t); // 照明亮度命令参数长度
    lv_cmd->body.percentage.br = _br;               // 亮度
    lv_cmd->body.percentage.opt = _op;              // 操作符
    lv_cmd->body.percentage.delayopTime = _opTime;  // 渐变操作时长（秒）

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_WLControl
* @brief    添加命令：照明波长
* @param
*   _wl：色温或波长，具体色温值2700-6500占2个字节，高8位在前，低8位在后
*   _op：控制方式，0-设置色温或波长值，1-增加色温或波长（相对值），2-减少色温或波长（相对值）；
*   _type：命令类型，可选参数，目前不需要
*   _opTime: 渐变操作时长（秒），可选参数，仅对DevTag_LED设备有效
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_WLControl(const US _wl, const UC _op, const UC _type, const US _opTime)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_LED;               // DevTag必须为DevTag_LED
    lv_cmd->head.cmd = CMD_CCT;                     // 照明波长
    lv_cmd->head.type = _type;                      // 命令类型
    lv_cmd->head.paramLen = sizeof(V_LEVEL_t);      // 照明波长命令参数长度
    lv_cmd->body.wavL.wl = _wl;                     // 色温或波长
    lv_cmd->body.wavL.opt = _op;                    // 操作符
    lv_cmd->body.wavL.delayopTime = _opTime;        // 渐变操作时长（秒）

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_LightControl
* @brief    添加命令：调节亮度及波长命令
* @param
*   _br：亮度，亮度具体值0-100
*   _wl：色温或波长，具体色温值2700-6500占2个字节，高8位在前，低8位在后
*   _op：控制方式，低4位为br控制方式，高4位是cct/wl控制方式；
*   _type：命令类型，可选参数，目前不需要
*   _opTime: 渐变操作时长（秒），可选参数，仅对DevTag_LED设备有效
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_LightControl(const UC _br, const US _wl, const UC _op, const UC _type, const US _opTime)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_LED;               // DevTag必须为DevTag_LED
    lv_cmd->head.cmd = CMD_COLOR;                   // 亮度及波长
    lv_cmd->head.type = _type;                      // 命令类型
    lv_cmd->head.paramLen = sizeof(V_HUE_t);        // 亮度及波长命令参数长度
    lv_cmd->body.br_wl.br = _br;                    // 亮度
    lv_cmd->body.br_wl.wl = _wl;                    // 色温或波长
    lv_cmd->body.br_wl.opt = _op;                   // 操作符
    lv_cmd->body.br_wl.delayopTime = _opTime;       // 渐变操作时长（秒）

    // 下一条
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_FanControl
* @brief    添加命令：风扇控制命令
* @param
*   _speed：风扇开关和风速
*   _op：操作符，OPERATOR_SET(0), OPERATOR_ADD(1), OPERATOR_SUB(2)，默认为0
*   _type：命令类型，可选参数，目前不需要
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_FanControl(const UC _speed, const UC _op, const UC _type)
{
    // 指向当前命令
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_Fan;               // DevTag必须为DevTag_Fan
    lv_cmd->head.cmd = CMD_BRIGHTNESS;              // 级别调整
    lv_cmd->head.type = _type;                      // 命令类型
    lv_cmd->head.paramLen = sizeof(V_WIND_t);       // 风速命令参数长度
    lv_cmd->body.wind.speed = _speed;               // 风扇开关和风速
    lv_cmd->body.wind.opt = _op;                   // 操作符

    // 下一条
    m_nCmdIndex++;
}

// 生成命令字符串：返回字符串指针和长度
US CActCmdInterface::Build(char **_pstrActCmd)
{
    // 计算crc16
    US lv_crc16 = usCRC16(m_msg + SCMSG_HEADER_LEN, m_nPayloadLen);
    m_msg[7] = lv_crc16 >> 8;
    m_msg[8] = lv_crc16 & 0xFF;

    // 字节数组 转 Hex字符串（注意：长度翻倍）
    ByteToString(m_msg, m_strOutput, GetMsgLen());

    // 返回字符串指针和长度
    *_pstrActCmd = m_strOutput;
    return(GetMsgStringLen());
}
