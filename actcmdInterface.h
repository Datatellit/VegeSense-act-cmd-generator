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

// Act-cmd �����������ӿ�
class CActCmdInterface
{
public:
    CActCmdInterface();

    //------------------------------------------------------------------
    // Step 1
    //------------------------------------------------------------------
    // �ӿڳ�ʼ����ÿ�������µ�act-cmd�����б�ǰ����
    void Init(const UC _uid, const UC _cmdCnt);

    //------------------------------------------------------------------
    // Step 2
    //------------------------------------------------------------------
    // ���������ٿ�������
    void AddCmd_PowerSwitch(const UC _devTag, const UC _sw, const UC _delay = 0);

    // �����������л�����
    void AddCmd_ChangeScenario(const UC _scenario, const UC _delay = 0);

    // �������豸��������
    void AddCmd_SetState(const UC _devTag, const UC _sw, const UC _delay = 0, const US _opTime = 0);

    // ��������������
    void AddCmd_BRControl(const UC _br, const UC _op = 0, const UC _delay = 0, const US _opTime = 0);

    // ��������������
    void AddCmd_WLControl(const US _wl, const UC _op = 0, const UC _delay = 0, const US _opTime = 0);

    // �������������ȼ���������
    void AddCmd_LightControl(const UC _br, const US _wl, const UC _op = 0, const UC _delay = 0, const US _opTime = 0);

    // ���������ȿ�������
    void AddCmd_FanControl(const UC _speed, const UC _op = 0, const UC _delay = 0);

    //------------------------------------------------------------------
    // Step 3
    //------------------------------------------------------------------
    // ���������ַ����������ַ���ָ��ͳ���
    US Build(char **_pstrActCmd);

    //------------------------------------------------------------------
    // ����uid
    UC GetUID() { return m_msg[6]; }
    // ����act-cmd�б��е���������
    UC GetCmdCount() { return m_nCmdCnt; }

    // ����act-cmd�б���ֽ������������� * һ�������ֽ�����18�ֽڣ�
    US GetPayloadLen() { return m_nPayloadLen; }

    // ����������Ϣ�ܳ��ȣ�GetPayloadLen() + ������Ϣͷ���ȣ�9�ֽڣ�
    US GetMsgLen() { return(m_nPayloadLen + SCMSG_HEADER_LEN); }

    // �������ɵ������ַ������ȣ�������Ϣ�ܳ��� * 2
    US GetMsgStringLen() { return strlen(m_strOutput); }

private:
    UC m_nCmdCnt;               // act-cmd�б��е���������
    UC m_nCmdIndex;             // ��ǰ������ţ���0��ʼ����_cmdCnt - 1�����������
    US m_nPayloadLen;           // act-cmd�б���ֽ��� = m_nCmdCnt * sizeof(cmdItem_t)

    //------------------------------------------------------------------
    // ��Ϣ���ݽṹ
    UC m_msg[SCMSG_HEADER_LEN + MAX_COMMAND_PER_ACTION * sizeof(cmdItem_t)];
    cmdItem_t *m_pCmdList;
    char m_strOutput[sizeof(m_msg) * 2 + 1];    
};

#endif  // __XLX_ACT_CMD_INTERFACE_H__