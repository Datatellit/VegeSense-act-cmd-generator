//  xlCommon.h - Xlight common definitions header

#ifndef xliCommon_h
#define xliCommon_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xliBitOperation.h"

// Here, common data types have been given alternative names through #define statements
// Common Data Type
#define BOOL                      bool
#define UC                        unsigned char
#define US                        unsigned short
#define UL                        unsigned long
#define CHAR                      char
#define SHORT                     short
#define LONG                      long

// Device Status
#define STATUS_OFF                0x00        // Power Off
#define STATUS_INIT               0x01        // Initializing
#define STATUS_BMW                0x02        // Basic Mode of Working
#define STATUS_DIS                0x03        // WAN Disconnected
#define STATUS_NWS                0x04        // Normal Working Status
#define STATUS_SLP                0x05        // Sleep
#define STATUS_RST                0x06        // Reseting
#define STATUS_ERR                0x07        // Fatal Error

// Cloud Dependency
#define CLOUD_DISABLE             0x00        // Disable Cloud
#define CLOUD_ENABLE              0x01        // Default, Enable Cloud, use it if available
#define CLOUD_MUST_CONNECT        0x02        // Enable Cloud and get connected anyhow

// Serial Port Speed
#define SERIALPORT_SPEED_LOW      9600
#define SERIALPORT_SPEED_14400    14400
#define SERIALPORT_SPEED_MID      19200
#define SERIALPORT_SPEED_57600    57600
#define SERIALPORT_SPEED_HIGH     115200
#define SERIALPORT_SPEED_DEFAULT  SERIALPORT_SPEED_HIGH

// Sensor Read Speed (in seconds)
#define SEN_DHT_SPEED_LOW         30
#define SEN_DHT_SPEED_NORMAL      5
#define SEN_DHT_SPEED_HIGH        2

#define SEN_ALS_SPEED_LOW         10
#define SEN_ALS_SPEED_NORMAL      5
#define SEN_ALS_SPEED_HIGH        2

#define SEN_PIR_SPEED_LOW         4
#define SEN_PIR_SPEED_NORMAL      2
#define SEN_PIR_SPEED_HIGH        1

#define SEN_BARCODE_SPEED_LOW     10
#define SEN_BARCODE_SPEED_NORMAL  2
#define SEN_BARCODE_SPEED_HIGH    1

// Row State Flags for Sync between Cloud, Flash, and Working Memory
enum EN_FLAG {DISABLED, ENABLED};
enum OP_FLAG {GET, POST, PUT, DELETE};
enum FLASH_FLAG {FF_EEPROM, FF_FLASH};
enum SAVE_FLAG {UNSAVED, SAVED};
enum RUN_FLAG {UNEXECUTED, EXECUTED};
enum LOAD_FLAG {UNLOADED, LOADED};

// enum values for CldJSONCommand()
typedef enum {
  CMD_SERIAL = 0,
  CMD_POWER = 1,
  CMD_COLOR = 2,
  CMD_BRIGHTNESS = 3,
  CMD_SCENARIO = 4,
  CMD_CCT = 5,
  CMD_QUERY = 6,
  CMD_EFFECT = 7,
  CMD_EXT = 8,
  CMD_INTERNAL = 9,

  CMD_MODBUS_READ = 80,
  CMD_MODBUS_WRITE
} COMMAND;

// Switch value for set power command
#define DEVICE_SW_OFF               0       // Turn Off
#define DEVICE_SW_ON                1       // Turn On, can have an optional level parameter 
#define DEVICE_SW_TOGGLE            2       // Toggle
#define DEVICE_SW_DUMMY             3       // Don't change, detail followed

// Update operator for set brightness & CCT command
#define OPERATOR_SET                0
#define OPERATOR_ADD                1
#define OPERATOR_SUB                2
#define OPERATOR_MUL                3
#define OPERATOR_DIV                4

// Filter (special effect)
#define FILTER_SP_EF_NONE           0
#define FILTER_SP_EF_BREATH         1       // Normal breathing light
#define FILTER_SP_EF_FAST_BREATH    2       // Fast breathing light
#define FILTER_SP_EF_FLORID         3       // Randomly altering color
#define FILTER_SP_EF_FAST_FLORID    4       // Fast randomly altering color

// Macros for UID types
#define CLS_ACTION                  'a'
#define CLS_CONFIGURATION           'c'
#define CLS_SENSOR                  'e'
#define CLS_GROUP                   'g'
#define CLS_NOTIFICATION            'i'
#define CLS_NODECONFIG              'j'
#define CLS_SCHEDULE                'm'
#define CLS_NODE_INFO               'n'
#define CLS_NODE_STATUS             'o'
#define CLS_PROGRAM                 'p'
#define CLS_RULE                    'r'
#define CLS_SCENARIO                's'
#define CLS_REPORT                  't'
#define CLS_WEB_RESOURCE            'u'
#define CLS_ZONE                    'z'

// BLE Configuration Settings
#define XLIGHT_BLE_SSID           "Xlight"        // BT module name
#define XLIGHT_BLE_CLASS          0x9A050C        // Class of Device (CoD)
#define XLIGHT_BLE_PIN            "1234"          // Pairing password for the BT module

// Sensor scope
#define SR_SCOPE_CONTROLLER         0     // Sensor on controller
#define SR_SCOPE_NODE               1     // Sensor on specific node
#define SR_SCOPE_ANY                2     // Sensor on any node under the controller
#define SR_SCOPE_GROUP              3     // Sensor on within the same node group

// Sensor logic symbols
#define SR_SYM_EQ                   0     // Equals
#define SR_SYM_NE                   1     // Not Equal
#define SR_SYM_GT                   2     // Greater Than
#define SR_SYM_GE                   3     // Greater than or Equal to
#define SR_SYM_LT                   4     // Less Than
#define SR_SYM_LE                   5     // Less than or Equal to
#define SR_SYM_BW                   6     // Between
#define SR_SYM_NB                   7     // Not Between

// Condition logic symbols
#define COND_SYM_NOT                0     // NOT
#define COND_SYM_AND                1     // AND
#define COND_SYM_OR                 2     // OR

// Time Unit
#define TM_NONE_UNIT                0
#define TM_SECOND_UNIT              1
#define TM_MINUTE_UNIT              2
#define TM_HOUR_UNIT                3

// Get & Set Working Object Flag
/// EN_FLAG en_flag             : 1;
/// OP_FLAG op_flag			    : 2;
/// FLASH_FLAG flash_flag       : 1;
/// SAVE_FLAG save_flag			: 1;
/// RUN_FLAG run_flag           : 1;
/// LOAD_FLAG load_flag			: 1;
#define SET_FLAG_EN(_flag, _val)    BF_SET(_flag, _val, 0, 1)
#define GET_FLAG_EN(_flag)          (EN_FLAG)BF_GET(_flag, 0, 1)
#define SET_FLAG_OP(_flag, _val)    BF_SET(_flag, _val, 1, 2)
#define GET_FLAG_OP(_flag)          (OP_FLAG)BF_GET(_flag, 1, 2)
#define SET_FLAG_FLASH(_flag, _val) BF_SET(_flag, _val, 3, 1)
#define GET_FLAG_FLASH(_flag)       (FLASH_FLAG)BF_GET(_flag, 3, 1)
#define SET_FLAG_SAVE(_flag, _val)  BF_SET(_flag, _val, 4, 1)
#define GET_FLAG_SAVE(_flag)        (SAVE_FLAG)BF_GET(_flag, 4, 1)
#define SET_FLAG_RUN(_flag, _val)   BF_SET(_flag, _val, 5, 1)
#define GET_FLAG_RUN(_flag)         (RUN_FLAG)BF_GET(_flag, 5, 1)
#define SET_FLAG_LOAD(_flag, _val)  BF_SET(_flag, _val, 6, 1)
#define GET_FLAG_LOAD(_flag)        (LOAD_FLAG)BF_GET(_flag, 6, 1)

/* 设备名和设备Tag对应关系 */
typedef enum {
  DevTag_GW = 0,
  DevTag_Relay,         // 1
  DevTag_Door,          // 2
  DevTag_LED,           // 3
  DevTag_Fan,           // 4
  DevTag_Zensor,        // 5
  DevTag_WaterLvl,      // 6
  DevTag_Dummy
} eDeviceTag_t;

/* 继电器通道 */
typedef enum {
  RelayKey_All = 0,
  RelayKey_Fan,         // 继电器1：换气扇
  RelayKey_Pump,        // 继电器2：水泵
  RelayKey_Valve,       // 继电器3：电磁阀
  RelayKey_Cooler,    	// 继电器4：冷风
  RelayKey_Internal,    // 继电器5：内部保留（系统控制，不对外）
  RelayKey_Dummy
} eRelayKey_t;

/* 设备名和设备Tag对应关系 */
extern const char FStr_DevName[DevTag_Dummy + RelayKey_Dummy][10];

/* 继电器设备衍生定义 */
#define DevTag_RelayFan             (DevTag_Dummy + RelayKey_Fan)
#define DevTag_RelayPump            (DevTag_Dummy + RelayKey_Pump)
#define DevTag_RelayValve           (DevTag_Dummy + RelayKey_Valve)
#define DevTag_RelayCooler          (DevTag_Dummy + RelayKey_Cooler)
#define DevTag_RelayInternal        (DevTag_Dummy + RelayKey_Internal)

//--------------------------------------------------
// Tools & Helpers
//--------------------------------------------------
#ifndef ABS_RETURN
#define ABS_RETURN(x,y)         	((x < y) ? (y-x) : (x-y))
#endif

#ifndef MIN_RETURN
#define MIN_RETURN(x,y)         	((x < y) ? (x) : (y))
#endif

#ifndef MAX_RETURN
#define MAX_RETURN(x,y)         	((x < y) ? (y) : (x))
#endif

UC h2i(const char c);
UC *StringToByte(char *pString, UC *pByte);
char *ByteToString(UC *pByte, char *pString, const US len);
US usCRC16(const UC * pucFrame, US usLen);

#endif /* xliCommon_h */
