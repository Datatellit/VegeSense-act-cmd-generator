#include "actcmdInterface.h"

CActCmdInterface::CActCmdInterface()
{
    m_pCmdList = (cmdItem_t *)(m_msg + SCMSG_HEADER_LEN);
}

/***************************************************************
* @func		Init
* @brief    �ӿڳ�ʼ����ÿ�������µ�act-cmd�����б�ǰ����
* @param
*   _uid��Ҫ����Act-cmd�������uid����Ҫ��Action uid��Ӧ(1��126֮��)
*   _cmdCnt��Ҫ����Act-cmd�������������������(1��16֮��)
* @retval	
***************************************************************/
void CActCmdInterface::Init(const UC _uid, const UC _cmdCnt) {
    // ���ݳ�ʼ��
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
* @brief    ���������ٿ�������
* @param
*   _devTag��DevTag���μ�eDeviceTag_t
*   _sw: ����״̬��ȡֵ[0..2]����DEVICE_SW_OFF, DEVICE_SW_ON, DEVICE_SW_TOGGLE
*   _type���������ͣ���ѡ������Ŀǰ����Ҫ
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_PowerSwitch(const UC _devTag, const UC _sw, const UC _type) {
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = _devTag;                  // DevTag
    lv_cmd->head.cmd = CMD_SERIAL;                  // ���ٿ���
    lv_cmd->head.type = _type;                      // ��������
    lv_cmd->head.paramLen = 1;                      // ���ٿ��������������
    lv_cmd->body.sw = _sw;                          // ����״̬

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_ChangeScenario
* @brief    �����������л�����
* @param    _scenario������ID
*           _type���������ͣ���ѡ������Ŀǰ����Ҫ
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_ChangeScenario(const UC _scenario, const UC _type)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_GW;                // DevTag����ΪDevTag_GW
    lv_cmd->head.cmd = CMD_SCENARIO;                // �л�����
    lv_cmd->head.type = _type;                      // ��������
    lv_cmd->head.paramLen = 1;                      // �����л������������
    lv_cmd->body.scenario_id = _scenario;           // ����ID

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_SetState
* @brief    �������豸��������
* @param
*   _devTag��DevTag���μ�eDeviceTag_t
*   _sw: ����״̬��ȡֵ[0..2]����DEVICE_SW_OFF, DEVICE_SW_ON, DEVICE_SW_TOGGLE
*   _type���������ͣ���ѡ������Ŀǰ����Ҫ
*   _opTime: �������ʱ�����룩����ѡ����������DevTag_LED�豸��Ч
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_SetState(const UC _devTag, const UC _sw, const UC _type, const US _opTime)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = _devTag;                  // DevTag
    lv_cmd->head.cmd = CMD_POWER;                   // �豸����
    lv_cmd->head.type = _type;                      // ��������
    // �豸���������������
    if(_devTag == DevTag_LED) {
        lv_cmd->head.paramLen = sizeof(V_STATUS_t);
        lv_cmd->body.status.sw = _sw;               // ����״̬
        lv_cmd->body.status.delayopTime = _opTime;  // �������ʱ�����룩
    } else {
        lv_cmd->head.paramLen = 1;
        lv_cmd->body.sw = _sw;                      // ����״̬
    }

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_BRControl
* @brief    ��������������
* @param
*   _br�����ȣ����Ⱦ���ֵ0-100
*   _op�����Ʒ�ʽ��0-��������ֵ��1-�������ȣ����ֵ����2-�������ȣ����ֵ����
*   _type���������ͣ���ѡ������Ŀǰ����Ҫ
*   _opTime: �������ʱ�����룩����ѡ����������DevTag_LED�豸��Ч
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_BRControl(const UC _br, const UC _op, const UC _type, const US _opTime)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_LED;               // DevTag����ΪDevTag_LED
    lv_cmd->head.cmd = CMD_BRIGHTNESS;              // �������
    lv_cmd->head.type = _type;                      // ��������
    lv_cmd->head.paramLen = sizeof(V_PERCENTAGE_t); // �������������������
    lv_cmd->body.percentage.br = _br;               // ����
    lv_cmd->body.percentage.opt = _op;              // ������
    lv_cmd->body.percentage.delayopTime = _opTime;  // �������ʱ�����룩

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_WLControl
* @brief    ��������������
* @param
*   _wl��ɫ�»򲨳�������ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
*   _op�����Ʒ�ʽ��0-����ɫ�»򲨳�ֵ��1-����ɫ�»򲨳������ֵ����2-����ɫ�»򲨳������ֵ����
*   _type���������ͣ���ѡ������Ŀǰ����Ҫ
*   _opTime: �������ʱ�����룩����ѡ����������DevTag_LED�豸��Ч
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_WLControl(const US _wl, const UC _op, const UC _type, const US _opTime)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_LED;               // DevTag����ΪDevTag_LED
    lv_cmd->head.cmd = CMD_CCT;                     // ��������
    lv_cmd->head.type = _type;                      // ��������
    lv_cmd->head.paramLen = sizeof(V_LEVEL_t);      // �������������������
    lv_cmd->body.wavL.wl = _wl;                     // ɫ�»򲨳�
    lv_cmd->body.wavL.opt = _op;                    // ������
    lv_cmd->body.wavL.delayopTime = _opTime;        // �������ʱ�����룩

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_LightControl
* @brief    �������������ȼ���������
* @param
*   _br�����ȣ����Ⱦ���ֵ0-100
*   _wl��ɫ�»򲨳�������ɫ��ֵ2700-6500ռ2���ֽڣ���8λ��ǰ����8λ�ں�
*   _op�����Ʒ�ʽ����4λΪbr���Ʒ�ʽ����4λ��cct/wl���Ʒ�ʽ��
*   _type���������ͣ���ѡ������Ŀǰ����Ҫ
*   _opTime: �������ʱ�����룩����ѡ����������DevTag_LED�豸��Ч
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_LightControl(const UC _br, const US _wl, const UC _op, const UC _type, const US _opTime)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_LED;               // DevTag����ΪDevTag_LED
    lv_cmd->head.cmd = CMD_COLOR;                   // ���ȼ�����
    lv_cmd->head.type = _type;                      // ��������
    lv_cmd->head.paramLen = sizeof(V_HUE_t);        // ���ȼ����������������
    lv_cmd->body.br_wl.br = _br;                    // ����
    lv_cmd->body.br_wl.wl = _wl;                    // ɫ�»򲨳�
    lv_cmd->body.br_wl.opt = _op;                   // ������
    lv_cmd->body.br_wl.delayopTime = _opTime;       // �������ʱ�����룩

    // ��һ��
    m_nCmdIndex++;
}

/***************************************************************
* @func		AddCmd_FanControl
* @brief    ���������ȿ�������
* @param
*   _speed�����ȿ��غͷ���
*   _op����������OPERATOR_SET(0), OPERATOR_ADD(1), OPERATOR_SUB(2)��Ĭ��Ϊ0
*   _type���������ͣ���ѡ������Ŀǰ����Ҫ
* @retval	
***************************************************************/
void CActCmdInterface::AddCmd_FanControl(const UC _speed, const UC _op, const UC _type)
{
    // ָ��ǰ����
    cmdItem_t *lv_cmd = m_pCmdList + m_nCmdIndex;

    lv_cmd->head.devTag = DevTag_Fan;               // DevTag����ΪDevTag_Fan
    lv_cmd->head.cmd = CMD_BRIGHTNESS;              // �������
    lv_cmd->head.type = _type;                      // ��������
    lv_cmd->head.paramLen = sizeof(V_WIND_t);       // ���������������
    lv_cmd->body.wind.speed = _speed;               // ���ȿ��غͷ���
    lv_cmd->body.wind.opt = _op;                   // ������

    // ��һ��
    m_nCmdIndex++;
}

// ���������ַ����������ַ���ָ��ͳ���
US CActCmdInterface::Build(char **_pstrActCmd)
{
    // ����crc16
    US lv_crc16 = usCRC16(m_msg + SCMSG_HEADER_LEN, m_nPayloadLen);
    m_msg[7] = lv_crc16 >> 8;
    m_msg[8] = lv_crc16 & 0xFF;

    // �ֽ����� ת Hex�ַ�����ע�⣺���ȷ�����
    ByteToString(m_msg, m_strOutput, GetMsgLen());

    // �����ַ���ָ��ͳ���
    *_pstrActCmd = m_strOutput;
    return(GetMsgStringLen());
}
