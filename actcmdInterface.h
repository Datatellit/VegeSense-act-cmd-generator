#ifndef __XLX_ACT_CMD_INTERFACE_H__
#define __XLX_ACT_CMD_INTERFACE_H__

#include "xlxConfig_Action.h"

#define VERSION_ACTION_COMMAND_API      0x11        // V1.1

// Message Format:
//   0    1        2           3          4       5      6        7           8     9 -
// ['C']['F'][total_pack][current_pack][len_h][len_low][uid][crc16_high][crc16_low][cmdlist data: sizeof(cmdItem_t) * _cmdCnt]
#define SCMSG_HEADER_LEN                9
#define ACT_CMD_MSG_HEAD_0              'C'
#define ACT_CMD_MSG_HEAD_1              'F'

// Act-cmd 命令生成器接口
class CActCmdInterface
{
public:
    CActCmdInterface();

    //------------------------------------------------------------------
    // Step 1
    //------------------------------------------------------------------
    // 接口初始化：每次生成新的act-cmd命令列表前调用
    void Init(const UC _uid, const UC _cmdCnt);

    //------------------------------------------------------------------
    // Step 2
    //------------------------------------------------------------------
    // 添加命令：快速开关命令
    void AddCmd_PowerSwitch(const UC _devTag, const UC _sw, const UC _delay = 0);

    // 添加命令：场景切换命令
    void AddCmd_ChangeScenario(const UC _scenario, const UC _delay = 0);

    // 添加命令：设备开关命令
    void AddCmd_SetState(const UC _devTag, const UC _sw, const UC _delay = 0, const US _opTime = 0);

    // 添加命令：照明亮度
    void AddCmd_BRControl(const UC _br, const UC _op = 0, const UC _delay = 0, const US _opTime = 0);

    // 添加命令：照明波长
    void AddCmd_WLControl(const US _wl, const UC _op = 0, const UC _delay = 0, const US _opTime = 0);

    // 添加命令：调节亮度及波长命令
    void AddCmd_LightControl(const UC _br, const US _wl, const UC _op = 0, const UC _delay = 0, const US _opTime = 0);

    // 添加命令：风扇控制命令
    void AddCmd_FanControl(const UC _speed, const UC _op = 0, const UC _delay = 0);

    //------------------------------------------------------------------
    // Step 3
    //------------------------------------------------------------------
    // 生成命令字符串：返回字符串指针和长度
    US Build(char **_pstrActCmd);

    //------------------------------------------------------------------
    // 返回uid
    UC GetUID() { return m_msg[6]; }
    // 返回act-cmd列表中的命令数量
    UC GetCmdCount() { return m_nCmdCnt; }

    // 返回act-cmd列表的字节数：命令数量 * 一条命令字节数（18字节）
    US GetPayloadLen() { return m_nPayloadLen; }

    // 返回命令消息总长度：GetPayloadLen() + 命令消息头长度（9字节）
    US GetMsgLen() { return(m_nPayloadLen + SCMSG_HEADER_LEN); }

    // 返回生成的命令字符串长度：命令消息总长度 * 2
    US GetMsgStringLen() { return strlen(m_strOutput); }

private:
    UC m_nCmdCnt;               // act-cmd列表中的命令数量
    UC m_nCmdIndex;             // 当前命令序号，从0开始，到_cmdCnt - 1，请依次添加
    US m_nPayloadLen;           // act-cmd列表的字节数 = m_nCmdCnt * sizeof(cmdItem_t)

    //------------------------------------------------------------------
    // 消息数据结构
    UC m_msg[SCMSG_HEADER_LEN + MAX_COMMAND_PER_ACTION * sizeof(cmdItem_t)];
    cmdItem_t *m_pCmdList;
    char m_strOutput[sizeof(m_msg) * 2 + 1];    
};

#endif  // __XLX_ACT_CMD_INTERFACE_H__