/*
 * PhoenixonControls_OPERATION.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_OPERATION_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_OPERATION_H_

#define OPERATION_LIMIT_MAX_VOLTAGE                 (float)(5.0)
#define OPERATION_LIMIT_MIN_VOLTAGE                 (float)(1.0)

#define OPERATION_INTEGRATION_RESET_TIME_US_CHA     80 //Time us
#define OPERATION_INTEGRATION_RESET_TIME_US_DIS     800
#define OPERATION_INTEGRATION_RESET_TIME_US_REST    80

#define OPERATION_GATEOFFSET_VOLT_GAIN_CHA          (float)(0.0) //y= (a/x) +b
#define OPERATION_GATEOFFSET_VOLT_OFFSET_CHA        (float)(5.0)

#define OPERATION_GATEOFFSET_VOLT_GAIN_DIS          (float)(0.0) //y= (a/x) +b
#define OPERATION_GATEOFFSET_VOLT_OFFSET_DIS        (float)(1.5)

#define OPERATION_VOLTAGE_TOLERANCE                 (float)(0.005)//1mV
#define OPERATION_CURRENT_TOLERANCE                 (float)(0.005)//1mA
#define OPERATION_POWER_TOLERANCE                   (float)(0.2)//0.2W

#define OPERATION_CHARGE_VOLTAGE_CHECK_TIME         (Uint16)(1)
#define OPERATION_DISCHARGE_VOLTAGE_CHECK_TIME      (Uint16)(2)
#define OPERATION_CURRENT_CHECK_TIME                (Uint16)(1)
#define OPERATION_POWER_CHECK_TIME                  (Uint16)(1)

#define OPERATION_MODE_VOLTAGE_CHECK_TIME           (Uint16)(50)
                                                    // 50A
#define OPERATION_CURRENT_FINE_CONTROL_RANGE        (Uint16)(500)   //0.5A
#define OPERATION_CURRENT_FINE_OFFSET               (Uint16)(200)   //0.2A
#define OPERATION_CURRENT_MOD                       (Uint16)(200)   //0.2A

#define OPERATION_CURRENT_CALIBRATION_RANGE         (float)(0.0005) //500uA


#if(PID == 1)
//#define Kp      1
//#define Ki      0.0001
//#define Kd      0
//#define dt      1
#define I_err_min     (-100.0) //0.1A
#define I_err_max     100.0
#endif

typedef enum
{
    OPERATION_MODE_REST,
    OPERATION_MODE_START,
    OPERATION_MODE_STOP,
    OPERATION_MODE_ALRAM,
    OPERATION_MODE_EMERGENCY,
    OPERATION_MODE_RESET,
    OPERATION_MODE_NOMAL_CC,
    OPERATION_MODE_NOMAL_CV,
    OPERATION_MODE_NOMAL_CP,
    OPERATION_MODE_NOMAL_PATTERN_CC,
    OPERATION_MODE_NOMAL_PATTERN_CV,
    OPERATION_MODE_NOMAL_PATTERN_CP,
    OPERATION_MODE_PARALLEL_CC,
    OPERATION_MODE_PARALLEL_CV,
    OPERATION_MODE_PARALLEL_CP,
    OPERATION_MODE_PARALLEL_PATTERN_CC,
    OPERATION_MODE_PARALLEL_PATTERN_CV,
    OPERATION_MODE_PARALLEL_PATTERN_CP,
}E_OPERATION_MODE_SELECT;

typedef enum
{
    OPERATION_MASTER,
    OPERATION_SLAVE,
    OPERATION_ALL,
}E_OPERATION_CHARGE_CHANNEL_SELECT;

typedef enum
{
    OPERATION_CHARGE,
    OPERATION_DISCHARGE,
}E_OPERATION_CHARGE_MODE_SELECT;

typedef enum
{
    OPERATION_NONE,
    OPERATION_REVERSE,
    OPERATION_OVER_VOLTAGE,
    OPEARTION_UNDER_VOLTAGE,
    OPEARTION_NORMAL,
}E_OPERATION_CELL_STATE;

typedef enum
{
    OPERATION_PAUSE_STOP,
    OPERATION_PAUSE_PAUSE,
    OPERATION_PAUSE_COMPLETE,
    OPERATION_PAUSE_RESTART,
}E_OPERATION_PAUSE;

typedef enum
{
    OPERATION_END_CONDITION_NONE,
    OPERATION_END_CONDITION_TIME,
    OPERATION_END_CONDITION_CURRENT,
    OPERATION_END_CONDITION_CAPACITY,
    OPERATION_END_CONDITION_WATT,
    OPERATION_END_CONDITION_VOLTAGE,
    OPERATION_END_CONDITION_GOTO,
    OPERATION_END_CONDITION_RETRUN,
    OPERATION_END_CONDITION_NONE_OP,        //JUMP END CONDITION
    OPERATION_END_CONDITION_UPDATE_END,     //PAUSE FORCE STOP END CONDITION
    OPERATION_END_CONDITION_ALARM,          //ALARM END CONDITION
    OPERATION_END_CONDITION_PAUSE,          //PAUSE END CONDITION (Except PAUSE FORCE STOP END CONDITIO)
}E_OPERATION_END_CONDITION;

typedef enum
{
    COM_NONE                = 0x0,
    COM_REST                = 0x81,
    COM_CHARGE              = 0x82,
    COM_DISCHARGE           = 0x83,
    COM_ALRAM_CLEAR         = 0x84,
    COM_INPUT_RELAY_OFF     = 0x85,
    COM_INPUT_RELAY_ON      = 0x86,
    COM_OUTPUT_RELAY_OFF    = 0x87,
    COM_OUTPUT_RELAY_ON     = 0x88,
    COM_ALL_RELAY_OFF       = 0x89,
    COM_ALRAM_REQUEST       = 0x90,
    COM_MODE_PARALLEL       = 0x91,
    COM_MODE_NORMAL         = 0x92,

}E_OPERATION_COM_SELECT;

typedef enum
{
    OP_DEFAULT           = 0x0,
    OP_CHARGE_CC         = 0x1,
    OP_CHARGE_CCCV       = 0x2,
    OP_CHARGE_CP         = 0x3,
    OP_CHARGE_CPCV       = 0x4,
    OP_DISCHARGE_CC      = 0x5,
    OP_DISCHARGE_CCCV    = 0x6,
    OP_DISCHARGE_CP      = 0x7,
    OP_DISCHARGE_CPCV    = 0x8,
    OP_CHARGE_CR         = 0x9,
    OP_DISCHARGE_CR      = 0xA,
}E_OPERATION_OP_SELECT;

typedef enum
{
    OP_SEND_DEFAULT          = 0x0,
    OP_SEND_INPUT_ON         = 0x1,
    OP_SEND_OUTPUT_ON        = 0x2,
    OP_SEND_CHARGE_CC        = 0x3,
    OP_SEND_CHARGE_CV        = 0x4,
    OP_SEND_DISCHARGE_CC     = 0x5,
    OP_SEND_DISCHARGE_CV     = 0x6,
    OP_SEND_CHARGE_CP        = 0x7,
    OP_SEND_DISCHARGE_CP     = 0x8,

    //Reserved              = 0x9;
    //Reserved              = 0xA;

    OP_SEND_CHARGE_CR        = 0xB,
    OP_SEND_DISCHARGE_CR     = 0xC,
}E_OPERATION_OP_SEND_SELECT;


typedef struct
{
    E_OPERATION_CHARGE_MODE_SELECT Mode;
    float Voltage;
    float Current;
}S_OPERATION_CHARGE_DATA;

typedef struct
{
    Uint16 EMOSwitchOn          :1; //Bit00
    Uint16 BatteryNone          :1; //Bit01
    Uint16 PowerFault           :1; //Bit02
    Uint16 CurrentFault         :1; //Bit03
    Uint16 VoltageFault         :1; //Bit04
    Uint16 CVFault              :1; //Bit05
    Uint16 SensingConnector     :1; //Bit06
    Uint16 CurrentConnector     :1; //Bit07
    Uint16 Reserve07            :1; //Bit08
    Uint16 Reserve09            :1; //Bit09
    Uint16 Reserve10            :1; //Bit10
    Uint16 Reserve11            :1; //Bit11
    Uint16 Reserve12            :1; //Bit12
    Uint16 Reserve13            :1; //Bit13
    Uint16 Reserve14            :1; //Bit14
//  Uint16 Reserve15            :1; //Bit15
}S_OPERATION_RX_ALARM_WARING_BIT;

typedef union
{
    Uint16 all;
    S_OPERATION_RX_ALARM_WARING_BIT bit;
}U_OPERATION_RX_ALARM_WARING;

typedef struct
{
    Uint16 VoltageRelayOn       :1; //Bit00
    Uint16 RelayOn              :1; //Bit01
    Uint16 OverHeatFault        :1; //Bit02
    Uint16 BatteryFault         :1; //Bit03
    Uint16 LineFault            :1; //Bit04
    Uint16 CCFault              :1; //Bit05
    Uint16 Reserve06            :1; //Bit06
    Uint16 Reserve07            :1; //Bit07
    Uint16 Reserve08            :1; //Bit08
    Uint16 Reserve09            :1; //Bit09
    Uint16 Reserve10            :1; //Bit10
    Uint16 Reserve11            :1; //Bit11
    Uint16 Reserve12            :1; //Bit12
    Uint16 Reserve13            :1; //Bit13
    Uint16 Reserve14            :1; //Bit14
//  Uint16 Reserve15            :1; //Bit15
}S_OPERATION_RX_ALARM_GRID_BIT;

typedef union
{
    Uint16 all;
    S_OPERATION_RX_ALARM_GRID_BIT bit;
}U_OPERATION_RX_ALARM_GRID;

typedef struct
{
    Uint16 RelayOn              :1; //Bit00
    Uint16 CommunicationFault   :1; //Bit01
    Uint16 MaximumVoltage       :1; //Bit02
    Uint16 Reserve03            :1; //Bit03
    Uint16 MinumumVoltage       :1; //Bit04
    Uint16 MaximumTime          :1; //Bit05
    Uint16 Reserve06            :1; //Bit06
    Uint16 Reserve07            :1; //Bit07
    Uint16 Reserve08            :1; //Bit08
    Uint16 Reserve09            :1; //Bit09
    Uint16 Reserve10            :1; //Bit10
    Uint16 Reserve11            :1; //Bit11
    Uint16 Reserve12            :1; //Bit12
    Uint16 Reserve13            :1; //Bit13
    Uint16 Reserve14            :1; //Bit14
//  Uint16 Reserve15            :1; //Bit15
}S_OPERATION_RX_ALARM_DC_BIT;

typedef union
{
    Uint16 all;
    S_OPERATION_RX_ALARM_DC_BIT bit;
}U_OPERATION_RX_ALARM_DC;

typedef struct
{
    Uint16 MaximumWatt          :1; //Bit00
    Uint16 MaximumWattHour      :1; //Bit01
    Uint16 MinumumCVAmpere      :1; //Bit02
    Uint16 Reserve03            :1; //Bit03
    Uint16 Reserve04            :1; //Bit04
    Uint16 Reserve05            :1; //Bit05
    Uint16 Reserve06            :1; //Bit06
    Uint16 Reserve07            :1; //Bit07
    Uint16 Reserve08            :1; //Bit08
    Uint16 Reserve09            :1; //Bit09
    Uint16 Reserve10            :1; //Bit10
    Uint16 Reserve11            :1; //Bit11
    Uint16 Reserve12            :1; //Bit12
    Uint16 Reserve13            :1; //Bit13
    Uint16 Reserve14            :1; //Bit14
//  Uint16 Reserve15            :1; //Bit15
}S_OPERATION_RX_ALARM_FAULT_BIT;
typedef union
{
    Uint16 all;
    S_OPERATION_RX_ALARM_FAULT_BIT bit;
}U_OPERATION_RX_ALARM_FAULT;

typedef struct
{
    Uint16 start_flag;
    Uint16 start_count;
    Uint16 current_count;
    Uint16 sensing_count;
} S_CONNECTOR_CHECK;

typedef struct
{
    Uint16 flag;
    Uint16 time;
} S_VOLTAGE_CONTOROL;

typedef struct
{
    Uint16 DEBUG_FLAG;
    E_OPERATION_COM_SELECT COMMAND; //Step 에서 사용
    float32 VARIABLE_VoltageRef;
    float32 VARIABLE_CurrentRef;

    Uint16 VARIABLE_UpdateData; //ipc 사용
    Uint16 VARIABLE_UpdateData_OLD; //ipc 사용
    Uint16 VARIABLE_UpdateAlramTime; //ipc 사용
    Uint16 VARIABLE_StartFlag;
//  E_OPERATION_MODE_SELECT VARIABLE_Operation;

    float32 VARIABLE_PowerRef;
    float32 VARIABLE_ResistanceRef;

    float32 VARIABLE_VoltageResult;
    float32 VARIABLE_CurrentResult;
    float32 VARIABLE_PowerResult;
    float64 VARIABLE_CapacityResult;

    Uint64  VARIABLE_TimeSpanResult;

    float32  VARIABLE_EndConditionVoltageValueAVG;
    float32  VARIABLE_EndConditionCurrentValueAVG;
    float32  VARIABLE_EndConditionWattValueAVG;


    Uint32  VARIABLE_EndConditionTime;
    Uint16  VARIABLE_EndConditionResult;
    float32 VARIABLE_EndConditionValue;

    Uint16  VARIABLE_ALARM_Flag;
    Uint16  VARIABLE_ALARM_RequestFlag;

    Uint16  VARIABLE_ALARM_UpDateFlag;              //DL 2019-07-15

    //DL 2019-06-09
    Uint16  CYCLE_INDEX;
    Uint16  STEP_INDEX_OLD;
    //End DL 2019-06-09

    U_OPERATION_RX_ALARM_WARING VARIABLE_ALARM_WARING;
    U_OPERATION_RX_ALARM_GRID   VARIABLE_ALARM_GRID;
    U_OPERATION_RX_ALARM_DC     VARIABLE_ALARM_DC;
    U_OPERATION_RX_ALARM_FAULT  VARIABLE_ALARM_FAULT;
    float32 CV_MovingData[50];
    float32 CC_MovingData[50];

    E_OPERATION_COM_SELECT VARIABLE_COMMAND_OLD;
    E_OPERATION_COM_SELECT DIRECT_COMMAND;//Command 명령이 왔을때 사용

    Uint16 MODE;
    Uint16 VARIABLE_MODE_OLD;

    E_OPERATION_OP_SELECT OPERATIN;
    E_OPERATION_OP_SEND_SELECT VARIABLE_OPERATIN;

    Uint16 SAMPLING_TIME;
    Uint16 SEND_TIME;

    Uint16 STEP_INDEX;
    Uint16 VARIABLE_STEP_INDEX;

    Uint16 CYCLE_LOOP_COUNT;
    Uint16 VARIABLE_CYCLE_LOOP_COUNT;

    Uint16 GOTO_INDEX;
    Uint16 RETURN_INDEX;
    Uint16 END_TIME_INDEX;
    Uint16 END_VOLTAGE_INDEX;
    Uint16 END_CURRENT_INDEX;
    Uint16 END_CAPACITY_INDEX;
    Uint16 END_WATT_INDEX;

    Uint32  END_TIME_CONDITION;
    float32 END_VOLTAGE_CONDITION;
    float32 END_CURRENT_CONDITION;
    float32 END_WATT_CONDITION;
    float32 END_CAPACITY_CONDITION;
    Uint16  PATTERN_PRIOD;

    S_CONTROL_VRELAY    C_VRELAY;
    S_CONTROL_RELAY     C_IRELAY;
    S_CONTROL_MUX C_OperationMUX;
    S_CONTROL_INTEGRATION_RESET_MUX C_IntResetMUX;
    S_CONTROL_INTEGRATION_SOLP_MUX C_IntSlopMUX;
    S_CONTROL_DAC C_IntSlopPMDAC;
    S_CONTROL_VDAC C_GateOffsetDAC;
    S_CONTROL_MUX C_GateOutputMUX;
    S_CONTROL_MUX C_GateOutputAMP;
    S_CONTROL_VDAC C_VRefDAC;
    S_CONTROL_DAC C_IRefDAC;
    S_CONTROL_DAC C_IFINERefDAC;

    S_SENSING_ADC_RESULT_DATA s_ADCData;
    S_SENSING_ADC_RESULT_DATA s_OldADCData;

    S_CONNECTOR_CHECK s_Con;
    S_VOLTAGE_CONTOROL s_VolCon;
}S_OPERATION_FILE;

Uint16 OPERATION_Init(S_OPERATION_FILE *sp_OP, Uint16 i);

Uint16 OPERATION_Run(S_OPERATION_FILE *sp_OP, Uint16 i);

E_OPERATION_OP_SEND_SELECT OPERATION_ModeCheck(S_OPERATION_FILE *sp_OP, Uint16 channel);

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_LOOP(S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_GOTO(S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_RETRUN  (S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_VOLTAGE (S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_TIME    (S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_CURRENT (S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_CAPACITY(S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_WATT    (S_OPERATION_FILE *sp_OP, Uint16 channel);
E_OPERATION_END_CONDITION OPERATION_EndConditionCheck         (S_OPERATION_FILE *sp_OP, Uint16 channel);

Uint16 OPERATION_CellCheck              (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Start                  (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Stop                   (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Alram                  (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_AlramReset             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Emergency              (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_ValueReset             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Reset                  (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Rest                   (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Charge                 (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Sensing                (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_Sensing_Timer          (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_VoltageControl         (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_IntegrationSlopControl (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPERATION_GateOffsetControl      (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 OPEARTION_CurrentControl         (S_OPERATION_FILE *sp_OP, Uint16 channel);


#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_OPERATION_H_ */
