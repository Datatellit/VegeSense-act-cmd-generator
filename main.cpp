#include "actcmdInterface.h"

int main()
{
    printf("\r\n------------ act-cmd-generator ----------------\r\n");
    CActCmdInterface lv_actCmd;
    UC lv_tp[4] = {0};
    char *lv_str;

    //------------------------------------------------------------------
    // Step 1: 初始化，请输入参数
    // --> 要生成Act-cmd命令组的uid，需要与Action uid对应(1到126之间)
    // --> 要生成Act-cmd命令组包含的命令数量(1至16之间)
    // --> 示例：uid = 1, cmdCount = 4
    lv_actCmd.Init(1, 4);

    //------------------------------------------------------------------
    // Step 2: 依次添加命令，总数就是上面初始化时输入的
    // --> 示例：第1条命令，快速打开风扇
    lv_actCmd.AddCmd_PowerSwitch(DevTag_Fan, DEVICE_SW_ON);
    // --> 示例：第2条命令，上条命令执行后再过10秒钟切换到场景1
    lv_actCmd.AddCmd_ChangeScenario(1, 10);
    // --> 示例：第3条命令，照明亮度用60秒时间调整到100
    lv_actCmd.AddCmd_BRControl(100, 0, 0, 60);
    // --> 示例：第4条命令，上条命令执行后再过15秒钟打开水泵
    lv_actCmd.AddCmd_SetState(DevTag_RelayPump, 1, 900);
    
    //------------------------------------------------------------------
    // Step 3: 生成消息
    lv_actCmd.Build(&lv_str);

    //------------------------------------------------------------------
    // 结果输出、显示
    printf("uid: %d, cmd-count: %d, msg-len: %d, payload-len: %d, streamStr-len: %d\r\n", lv_actCmd.GetUID(), 
        lv_actCmd.GetCmdCount(), lv_actCmd.GetMsgLen(), lv_actCmd.GetPayloadLen(), lv_actCmd.GetMsgStringLen());
    printf("streamStr: %s\r\n", lv_str);
    //getchar();
}
