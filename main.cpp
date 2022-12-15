#include "actcmdInterface.h"

int main()
{
    printf("\r\n------------ act-cmd-generator ----------------\r\n");
    CActCmdInterface lv_actCmd;
    UC lv_tp[4] = {0};
    char *lv_str;

    //------------------------------------------------------------------
    // Step 1: ��ʼ�������������
    // --> Ҫ����Act-cmd�������uid����Ҫ��Action uid��Ӧ(1��126֮��)
    // --> Ҫ����Act-cmd�������������������(1��16֮��)
    // --> ʾ����uid = 1, cmdCount = 4
    lv_actCmd.Init(1, 4);

    //------------------------------------------------------------------
    // Step 2: �����������������������ʼ��ʱ�����
    // --> ʾ������1��������ٴ򿪷���
    lv_actCmd.AddCmd_PowerSwitch(DevTag_Fan, DEVICE_SW_ON);
    // --> ʾ������2�������������ִ�к��ٹ�10�����л�������1
    lv_actCmd.AddCmd_ChangeScenario(1, 10);
    // --> ʾ������3���������������60��ʱ�������100
    lv_actCmd.AddCmd_BRControl(100, 0, 0, 60);
    // --> ʾ������4�������������ִ�к��ٹ�15���Ӵ�ˮ��
    lv_actCmd.AddCmd_SetState(DevTag_RelayPump, 1, 900);
    
    //------------------------------------------------------------------
    // Step 3: ������Ϣ
    lv_actCmd.Build(&lv_str);

    //------------------------------------------------------------------
    // ����������ʾ
    printf("uid: %d, cmd-count: %d, msg-len: %d, payload-len: %d, streamStr-len: %d\r\n", lv_actCmd.GetUID(), 
        lv_actCmd.GetCmdCount(), lv_actCmd.GetMsgLen(), lv_actCmd.GetPayloadLen(), lv_actCmd.GetMsgStringLen());
    printf("streamStr: %s\r\n", lv_str);
    //getchar();
}
