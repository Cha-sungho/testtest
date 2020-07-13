/*
 * PhoenixonControl(*sp_OP).c
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"
#include "math.h"

#ifdef _DEBUG_PRINTF_CALL
#include "stdio.h"
#endif

#pragma CODE_SECTION(OPERATION_Run, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_ModeCheck, ".TI.ramfunc");

#pragma CODE_SECTION(OPERATION_EndConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_CellCheck, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Start, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Stop, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Alram, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_AlramReset, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Emergency, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_ValueReset, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Reset, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Rest, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Charge, ".TI.ramfunc");

#pragma CODE_SECTION(OPERATION_Sensing, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_Sensing_Timer, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_VoltageControl, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_GateOffsetControl, ".TI.ramfunc");
#pragma CODE_SECTION(OPEARTION_CurrentControl, ".TI.ramfunc");
#pragma CODE_SECTION(OPERATION_IntegrationSlopControl, ".TI.ramfunc");

extern float32 VoltageCharge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH];
extern float32 VoltageDischarge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH];
extern float32 VoltageCharge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH];
extern float32 VoltageDischarge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH];
extern float32 CurrentCharge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentCharge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH];

extern float32 CurrentCharge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentCharge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];

extern float32 CurrentCharge_Low_Current_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_Low_Current_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];

extern float g_capacity_1h;

extern Uint16 plus_conversion_flag;
extern Uint16 minus_conversion_flag;
extern Uint16 conversion_flag;

#if(PID == 1)
extern Uint16 pid_flag = 0;

float32 Kp = 1.0;
float32 Ki = 0.0001;
float32 Kd = 0.0;
float32 dt = 1.0;

Uint16 flag = 0;
Uint16 pid_time = 50;

float32 current_val = 0.0;          // 현재값
float32 err = 0.0;
float32 prev_err = 0.0;             // 오차. 이전 오차
float32 I_err = 0.0;
float32 D_err = 0.0;                // 오차적분. 오차미분
float32 Kp_term = 0.0;
float32 Ki_term = 0.0;
float32 Kd_term = 0.0;              // p항, i항, d항
float32 control_current = 0.0;

#endif

Uint16 OPERATION_Init(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 ui_i = 0;

    OPERATION_Reset(sp_OP, channel);
#if 1
    DELAY_US(100000);  //Time of 100ms
    for (ui_i = 0; ui_i < 10; ui_i++)
    {
        SENSING_ADCResultData(&sp_OP[channel].s_ADCData, SENSING_ALL, channel);
    }
#endif
    return (SUCCESS);  //SUCCESS Or FAIL
}

Uint16 OPERATION_Run(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    E_OPERATION_COM_SELECT Command = COM_NONE;
    static E_OPERATION_COM_SELECT old_Command[4] = { COM_NONE, };

    if (sp_OP[channel].DIRECT_COMMAND != COM_NONE) {
        Command = sp_OP[channel].DIRECT_COMMAND;
        if (sp_OP[channel].VARIABLE_COMMAND_OLD == Command) {
            sp_OP[channel].DIRECT_COMMAND = COM_NONE;
        }
        Command = sp_OP[channel].DIRECT_COMMAND;
    } else {
        if (sp_OP[channel].VARIABLE_ALARM_Flag == CLEAR) {
            Command = sp_OP[channel].COMMAND;
        } else {
            Command = COM_NONE;
        }
    }

    if (old_Command[channel] != Command) {
        old_Command[channel] = Command;

        if (Command == COM_CHARGE || Command == COM_DISCHARGE || Command == COM_REST) {
            sp_OP[channel].s_Con.start_flag = 1;
            sp_OP[channel].s_Con.start_count = 0;
            sp_OP[channel].s_Con.current_count = 0;
            sp_OP[channel].s_Con.sensing_count = 0;
        }
    }

    switch (Command)
    {
        case COM_NONE:
            OPERATION_Stop(sp_OP, channel);
            break;
        case COM_REST:
            OPERATION_Rest(sp_OP, channel);
            if (sp_OP[channel].s_Con.start_flag == 1) {
                sp_OP[channel].s_Con.start_count++;
                if (sp_OP[channel].s_Con.start_count >= 1000) {
                    sp_OP[channel].s_Con.start_flag = 0;
                    sp_OP[channel].s_Con.start_count = 0;
                    sp_OP[channel].s_Con.current_count = 0;
                    sp_OP[channel].s_Con.sensing_count = 0;
                }
            } else {
                ALARM_BatterySensingConnectorCheckOperating(sp_OP, channel);
            }

            break;
        case COM_CHARGE:
        case COM_DISCHARGE:
            OPERATION_Charge(sp_OP, channel);
            if (sp_OP[channel].s_Con.start_flag == 1) {
                sp_OP[channel].s_Con.start_count++;
                if (sp_OP[channel].s_Con.start_count >= 1000) {
                    sp_OP[channel].s_Con.start_flag = 0;
                    sp_OP[channel].s_Con.start_count = 0;
                    sp_OP[channel].s_Con.current_count = 0;
                    sp_OP[channel].s_Con.sensing_count = 0;
                }
            } else {

                ALARM_BatterySensingConnectorCheckOperating(sp_OP, channel);
                ALARM_BatteryCurrentConnectorCheckOperating(sp_OP, channel);
            }
            break;

        case COM_ALRAM_CLEAR:
            OPERATION_AlramReset(sp_OP, channel);
            break;

        case COM_INPUT_RELAY_OFF:
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].VARIABLE_StartFlag = CLEAR;
            break;

        case COM_INPUT_RELAY_ON:
            break;

        case COM_OUTPUT_RELAY_OFF:
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].VARIABLE_StartFlag = CLEAR;
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            break;

        case COM_OUTPUT_RELAY_ON:
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
            break;

        case COM_ALL_RELAY_OFF:
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].VARIABLE_StartFlag = CLEAR;
//            sp_OP[channel].C_VRELAY = CONTROL_VoltageRELAY(channel, CONTROL_OFF);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            break;

        case COM_ALRAM_REQUEST:
            sp_OP[channel].VARIABLE_ALARM_RequestFlag++;
            break;

        case COM_MODE_PARALLEL:
            break;
        case COM_MODE_NORMAL:
            break;
    }
    sp_OP[channel].VARIABLE_COMMAND_OLD = Command;

    return (SUCCESS);  //SUCCESS Or FAIL
}

Uint16 OPERATION_CellCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    OPERATION_Sensing(sp_OP, channel);
#if 1
    if (ALARM_BatteryFaultCheck(sp_OP, channel)) return (FAIL);
    if (ALARM_BatteryNoneCheck(sp_OP, channel)) return (FAIL);
#endif
    return (SUCCESS);  //SUCCESS Or FAIL
}

#if (MODE_CHECK_TYPE == MODE_CHECK_CURRENT || MODE_CHECK_TYPE == MODE_CHECK_CURRENT_POWER)
E_OPERATION_OP_SEND_SELECT OPERATION_ModeCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    E_OPERATION_OP_SEND_SELECT Mode = OP_SEND_DEFAULT;
    static Uint16 CC_To_CV_Change_Flag[4] = { 0, };

    static Uint16 SettingTime[4] = { 0, };
    static E_OPERATION_COM_SELECT Flag[4] = { COM_NONE, };
    static Uint16 Index[4] = { 0, };

    float current_new = 0.0;
    static Uint16 sensing_stable_count[4] = { 0, };
    static Uint16 error_time[4] = { 0, };
    float ref_current = 0.0;

    float ref_power = 0.0;
    float new_power = 0.0;

    if (sp_OP[channel].OPERATIN == OP_CHARGE_CP                 // CP
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CP
            || sp_OP[channel].OPERATIN == OP_CHARGE_CPCV
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {
        sp_OP[channel].VARIABLE_PowerRef = (float) (sp_OP[channel].VARIABLE_CurrentRef / ABS_F(sp_OP[channel].VARIABLE_VoltageResult));//Power
        //CP kW -> W
        //sp_OP[channel].VARIABLE_PowerRef *= (float) (1000.0);

        ref_current = sp_OP[channel].VARIABLE_PowerRef;
        ref_power = sp_OP[channel].VARIABLE_CurrentRef;
    } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CR          // CR
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CR) {
        sp_OP[channel].VARIABLE_ResistanceRef = (float) (ABS_F(sp_OP[channel].VARIABLE_VoltageResult) / sp_OP[channel].VARIABLE_CurrentRef);//Resistance

        ref_current = sp_OP[channel].VARIABLE_ResistanceRef;
    } else {                                                    // CC, CCCV
        ref_current = sp_OP[channel].VARIABLE_CurrentRef;
    }

    switch (sp_OP[channel].COMMAND)
    {
        case COM_CHARGE:
            if (Flag[channel] != COM_CHARGE || Index[channel] != sp_OP[channel].STEP_INDEX) {
                Flag[channel] = COM_CHARGE;
                Index[channel] = sp_OP[channel].STEP_INDEX;
                SettingTime[channel] = 0;

                CC_To_CV_Change_Flag[channel] = CLEAR;
                sensing_stable_count[channel] = 0;
                error_time[channel] = 0;
            }
            if (sensing_stable_count[channel] >= 3) {
                if ((ABS_F(sp_OP[channel].VARIABLE_VoltageResult)) >= (sp_OP[channel].VARIABLE_VoltageRef - OPERATION_VOLTAGE_TOLERANCE )) {
                    if (CC_To_CV_Change_Flag[channel] == CLEAR) {

                        current_new = ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current);
                        new_power = current_new * ABS_F(sp_OP[channel].VARIABLE_VoltageResult);

                        //case 1. Target strange
                        if (current_new < 0.06) {
                            error_time[channel]++;
                            if (error_time[channel] >= 3) {
                                CC_To_CV_Change_Flag[channel] = SET;
                            }
                        } else {
                            error_time[channel] = 0;
                        }

#if (MODE_CHECK_TYPE == MODE_CHECK_CURRENT_POWER)
                        //case 2. Current decrease
                        if (sp_OP[channel].OPERATIN == OP_CHARGE_CCCV) {
                            if ((ref_current - 0.01) > current_new) {

                                SettingTime[channel]++;

                                if (SettingTime[channel] >= OPERATION_MODE_VOLTAGE_CHECK_TIME) {
                                    CC_To_CV_Change_Flag[channel] = SET;
                                }
                            } else {
                                SettingTime[channel] = 0;
                            }
                        } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CPCV) {
                            if ((ref_power - 0.05) > new_power) {

                                SettingTime[channel]++;

                                if (SettingTime[channel] >= OPERATION_MODE_VOLTAGE_CHECK_TIME) {
                                    CC_To_CV_Change_Flag[channel] = SET;
                                }
                            } else {
                                SettingTime[channel] = 0;
                            }
                        }
#else
                        //case 2. Current decrease
                        if ((ref_current - 0.01) > current_new) {

                            SettingTime[channel]++;

                            if (SettingTime[channel] >= OPERATION_MODE_VOLTAGE_CHECK_TIME) {
                                CC_To_CV_Change_Flag[channel] = SET;
                            }
                        } else {
                            SettingTime[channel] = 0;
                        }
#endif
                    }
                }
            } else {
                sensing_stable_count[channel]++;
            }
            break;

        case COM_DISCHARGE:
            if (Flag[channel] != COM_DISCHARGE || Index[channel] != sp_OP[channel].STEP_INDEX) {
                Flag[channel] = COM_DISCHARGE;
                Index[channel] = sp_OP[channel].STEP_INDEX;
                SettingTime[channel] = 0;

                CC_To_CV_Change_Flag[channel] = CLEAR;
                sensing_stable_count[channel] = 0;
                error_time[channel] = 0;
            }
            if (sensing_stable_count[channel] >= 3) {
                if ((ABS_F(sp_OP[channel].VARIABLE_VoltageResult)) <= (sp_OP[channel].VARIABLE_VoltageRef + OPERATION_VOLTAGE_TOLERANCE )) {
                    if (CC_To_CV_Change_Flag[channel] == CLEAR) {

                        current_new = ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current);
                        new_power = current_new * ABS_F(sp_OP[channel].VARIABLE_VoltageResult);

                        //case 1. Target strange
                        if (current_new < 0.06) {
                            error_time[channel]++;
                            if (error_time[channel] >= 3) {
                                CC_To_CV_Change_Flag[channel] = SET;
                            }
                        } else {
                            error_time[channel] = 0;
                        }

#if (MODE_CHECK_TYPE == MODE_CHECK_CURRENT_POWER)
                        //case 2. Current decrease
                        if (sp_OP[channel].OPERATIN == OP_DISCHARGE_CCCV) {
                            if ((ref_current - 0.01) > current_new) {

                                SettingTime[channel]++;

                                if (SettingTime[channel] >= OPERATION_MODE_VOLTAGE_CHECK_TIME) {
                                    CC_To_CV_Change_Flag[channel] = SET;
                                }
                            } else {
                                SettingTime[channel] = 0;
                            }
                        } else if (sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {
                            if ((ref_power - 0.05) > new_power) {

                                SettingTime[channel]++;

                                if (SettingTime[channel] >= OPERATION_MODE_VOLTAGE_CHECK_TIME) {
                                    CC_To_CV_Change_Flag[channel] = SET;
                                }
                            } else {
                                SettingTime[channel] = 0;
                            }
                        }
#else
                        //case 2. Current decrease
                        if ((ref_current - 0.01) > current_new) {

                            SettingTime[channel]++;

                            if (SettingTime[channel] >= OPERATION_MODE_VOLTAGE_CHECK_TIME) {
                                CC_To_CV_Change_Flag[channel] = SET;
                            }
                        } else {
                            SettingTime[channel] = 0;
                        }
#endif
                    }
                }
            } else {
                sensing_stable_count[channel]++;
            }
            break;

        default:
            Flag[channel] = COM_NONE;
            Index[channel] = sp_OP[channel].STEP_INDEX;
            SettingTime[channel] = 0;

            CC_To_CV_Change_Flag[channel] = CLEAR;
            sensing_stable_count[channel] = 0;
            error_time[channel] = 0;
            break;
    }

    if (sp_OP[channel].C_VRELAY.e_OldCommand == CONTROL_ON) Mode = OP_SEND_INPUT_ON;

    switch (sp_OP[channel].OPERATIN)
    {
        case OP_DEFAULT:
            Mode = OP_SEND_DEFAULT;
            break;
        case OP_CHARGE_CC:
            Mode = OP_SEND_CHARGE_CC;
            break;
        case OP_CHARGE_CCCV:
            if (CC_To_CV_Change_Flag[channel] == SET) Mode = OP_SEND_CHARGE_CV;
            else Mode = OP_SEND_CHARGE_CC;
            break;
        case OP_DISCHARGE_CC:
            Mode = OP_SEND_DISCHARGE_CC;
            break;
        case OP_DISCHARGE_CCCV:
            if (CC_To_CV_Change_Flag[channel] == SET) Mode = OP_SEND_DISCHARGE_CV;
            else Mode = OP_SEND_DISCHARGE_CC;
            break;
        case OP_CHARGE_CP:
            Mode = OP_SEND_CHARGE_CP;
            break;
        case OP_DISCHARGE_CP:
            Mode = OP_SEND_DISCHARGE_CP;
            break;
        case OP_CHARGE_CPCV:
            if (CC_To_CV_Change_Flag[channel] == SET) Mode = OP_SEND_CHARGE_CV;
            else Mode = OP_SEND_CHARGE_CP;
            break;
        case OP_DISCHARGE_CPCV:
            if (CC_To_CV_Change_Flag[channel] == SET) Mode = OP_SEND_DISCHARGE_CV;
            else Mode = OP_SEND_DISCHARGE_CP;
            break;
        case OP_CHARGE_CR:
            Mode = OP_SEND_CHARGE_CR;
            break;
        case OP_DISCHARGE_CR:
            Mode = OP_SEND_DISCHARGE_CR;
            break;
    }
    sp_OP[channel].VARIABLE_OPERATIN = Mode;
    return (Mode);  //SUCCESS Or FAIL
}
#else
E_OPERATION_OP_SEND_SELECT OPERATION_ModeCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    E_OPERATION_OP_SEND_SELECT Mode = OP_SEND_DEFAULT;
    Uint16 CC_To_CV_Change_Flag = 0;

    static Uint16 SettingTime[4] = { 0, };
    static E_OPERATION_COM_SELECT Flag[4] = { COM_NONE, };
    static Uint16 Index[4] = { 0, };

    switch (sp_OP[channel].COMMAND)
    {
        case COM_CHARGE:
            if (Flag[channel] != COM_CHARGE || Index[channel] != sp_OP[channel].STEP_INDEX) {
                Flag[channel] = COM_CHARGE;
                Index[channel] = sp_OP[channel].STEP_INDEX;
                SettingTime[channel] = 0;
            }

            if ((ABS_F(sp_OP[channel].VARIABLE_VoltageResult)) >= (sp_OP[channel].VARIABLE_VoltageRef - OPERATION_VOLTAGE_TOLERANCE )) {
                if (++SettingTime[channel] >= OPERATION_VOLTAGE_CHECK_TIME) {
                    SettingTime[channel] = OPERATION_VOLTAGE_CHECK_TIME;
                    CC_To_CV_Change_Flag = SET;
                }
            }
            break;

        case COM_DISCHARGE:
            if (Flag[channel] != COM_DISCHARGE || Index[channel] != sp_OP[channel].STEP_INDEX) {
                Flag[channel] = COM_DISCHARGE;
                Index[channel] = sp_OP[channel].STEP_INDEX;
                SettingTime[channel] = 0;
            }
            if ((ABS_F(sp_OP[channel].VARIABLE_VoltageResult)) <= (sp_OP[channel].VARIABLE_VoltageRef + OPERATION_VOLTAGE_TOLERANCE )) {
                if (++SettingTime[channel] >= OPERATION_VOLTAGE_CHECK_TIME) {
                    SettingTime[channel] = OPERATION_VOLTAGE_CHECK_TIME;
                    CC_To_CV_Change_Flag = SET;
                }
            }
            break;

        default:
            Flag[channel] = COM_NONE;
            Index[channel] = sp_OP[channel].STEP_INDEX;
            SettingTime[channel] = 0;
            break;
    }

    if (sp_OP[channel].C_VRELAY.e_OldCommand == CONTROL_ON) Mode = OP_SEND_INPUT_ON;

    switch (sp_OP[channel].OPERATIN)
    {
        case OP_DEFAULT:
            Mode = OP_SEND_DEFAULT;
            break;
        case OP_CHARGE_CC:
            Mode = OP_SEND_CHARGE_CC;
            break;
        case OP_CHARGE_CCCV:
            if (CC_To_CV_Change_Flag == SET) Mode = OP_SEND_CHARGE_CV;
            else Mode = OP_SEND_CHARGE_CC;
            break;
        case OP_DISCHARGE_CC:
            Mode = OP_SEND_DISCHARGE_CC;
            break;
        case OP_DISCHARGE_CCCV:
            if (CC_To_CV_Change_Flag == SET) Mode = OP_SEND_DISCHARGE_CV;
            else Mode = OP_SEND_DISCHARGE_CC;
            break;
        case OP_CHARGE_CP:
            Mode = OP_SEND_CHARGE_CP;
            break;
        case OP_DISCHARGE_CP:
            Mode = OP_SEND_DISCHARGE_CP;
            break;
        case OP_CHARGE_CPCV:
            if (CC_To_CV_Change_Flag == SET) Mode = OP_SEND_CHARGE_CV;
            else Mode = OP_SEND_CHARGE_CP;
            break;
        case OP_DISCHARGE_CPCV:
            if (CC_To_CV_Change_Flag == SET) Mode = OP_SEND_DISCHARGE_CV;
            else Mode = OP_SEND_DISCHARGE_CP;
            break;
        case OP_CHARGE_CR:
            Mode = OP_SEND_CHARGE_CR;
            break;
        case OP_DISCHARGE_CR:
            Mode = OP_SEND_DISCHARGE_CR;
            break;
    }
    sp_OP[channel].VARIABLE_OPERATIN = Mode;
    return (Mode);  //SUCCESS Or FAIL
}
#endif

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_LOOP(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    if (sp_OP[channel].CYCLE_LOOP_COUNT) {
        sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT++;
        if (sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT >= sp_OP[channel].CYCLE_LOOP_COUNT) {
            sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;
            return (OPERATION_END_CONDITION_GOTO);
        } else {
            return (OPERATION_END_CONDITION_RETRUN);  //SUCCESS Or FAIL
        }
    }
    return (OPERATION_END_CONDITION_NONE);  //SUCCESS Or FAIL
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_GOTO(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    return (OPERATION_END_CONDITION_NONE);  //SUCCESS Or FAIL
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_RETRUN(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    return (OPERATION_END_CONDITION_NONE);  //SUCCESS Or FAIL
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_VOLTAGE(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 SettingTime[4] = { 0, };
    static Uint16 Index[4] = { 0, };

    float EndVoltagConditionValue = 0.0;

    if (sp_OP[channel].END_VOLTAGE_CONDITION) {
        EndVoltagConditionValue = (float) (sp_OP[channel].END_VOLTAGE_CONDITION);

        switch (sp_OP[channel].COMMAND)
        {
            case COM_REST:
                if ((ABS_F(sp_OP[channel].VARIABLE_EndConditionVoltageValueAVG)) > EndVoltagConditionValue) {
                        return (OPERATION_END_CONDITION_VOLTAGE);
                }
                break;

            case COM_CHARGE:
                if (Index[channel] != sp_OP[channel].STEP_INDEX) {
                    Index[channel] = sp_OP[channel].STEP_INDEX;
                    SettingTime[channel] = 0;
                }
                if ((ABS_F(sp_OP[channel].VARIABLE_EndConditionVoltageValueAVG)) > EndVoltagConditionValue) {
                    if (SettingTime[channel]++ >= OPERATION_CHARGE_VOLTAGE_CHECK_TIME) {
                        SettingTime[channel] = 0;
                        return (OPERATION_END_CONDITION_VOLTAGE);
                    }
                } else {
                    SettingTime[channel] = 0;
                }
                break;

            case COM_DISCHARGE:
                if (Index[channel] != sp_OP[channel].STEP_INDEX) {
                    Index[channel] = sp_OP[channel].STEP_INDEX;
                    SettingTime[channel] = 0;
                }
                if ((ABS_F(sp_OP[channel].VARIABLE_EndConditionVoltageValueAVG)) < EndVoltagConditionValue) {
                    if (SettingTime[channel]++ >= OPERATION_DISCHARGE_VOLTAGE_CHECK_TIME) {
                        SettingTime[channel] = 0;
                        return (OPERATION_END_CONDITION_VOLTAGE);
                    }
                } else {
                    SettingTime[channel] = 0;
                }
                break;
        }
    }

    return (OPERATION_END_CONDITION_NONE);
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_TIME(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint32 SettingTime[4] = { 0, };
    static Uint16 Index[4] = { 0, };

    if (sp_OP[channel].END_TIME_CONDITION) {
        if (Index[channel] != sp_OP[channel].STEP_INDEX) {
            Index[channel] = sp_OP[channel].STEP_INDEX;
            SettingTime[channel] = 0;
        }
        if ((sp_OP[channel].VARIABLE_TimeSpanResult % COMMUNICATION_TX_DATA_LENGTH) >= COMMUNICATION_TX_DATA_LENGTH - 1) SettingTime[channel]++;

        if (SettingTime[channel] >= (sp_OP[channel].END_TIME_CONDITION)) {
            sp_OP[channel].VARIABLE_EndConditionTime = SettingTime[channel];
            SettingTime[channel] = 0;
            return (OPERATION_END_CONDITION_TIME);
        }
    } else {
        Index[channel] = sp_OP[channel].STEP_INDEX;
        SettingTime[channel] = 0;
    }

    return (OPERATION_END_CONDITION_NONE);
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_CURRENT(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 RiseFallTime[4] = { 0, };
    static Uint16 SettingTime[4] = { 0, };
    static Uint16 Index[4] = { 0, };
    float EndCurrentConditionValue = 0.0;

    if (Index[channel] != sp_OP[channel].STEP_INDEX) {
        Index[channel] = sp_OP[channel].STEP_INDEX;
        RiseFallTime[channel] = 0;
        SettingTime[channel] = 0;
    }
    if (sp_OP[channel].END_CURRENT_CONDITION) {
        EndCurrentConditionValue = (float) (sp_OP[channel].END_CURRENT_CONDITION);

        if ((sp_OP[channel].VARIABLE_TimeSpanResult & 0x1) != (RiseFallTime[channel] & 0x1)) RiseFallTime[channel] += 1;

        switch (sp_OP[channel].VARIABLE_OPERATIN)
        {
            case OP_SEND_CHARGE_CV:
            case OP_SEND_DISCHARGE_CV:
                if (((ABS_F(sp_OP[channel].VARIABLE_EndConditionCurrentValueAVG)) < EndCurrentConditionValue && (ABS_F(sp_OP[channel].VARIABLE_EndConditionCurrentValueAVG)) >= EndCurrentConditionValue - 0.005) || (ABS_F(sp_OP[channel].VARIABLE_EndConditionCurrentValueAVG)) < EndCurrentConditionValue - 0.010) {
                    if (SettingTime[channel]++ >= OPERATION_CURRENT_CHECK_TIME) {
                        SettingTime[channel] = 0;
                        return (OPERATION_END_CONDITION_CURRENT);
                    }
                }
                break;
        }
    }

    return (OPERATION_END_CONDITION_NONE);
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_CAPACITY(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    float EndCapacityConditionValue = 0.0;

    if (sp_OP[channel].END_CAPACITY_CONDITION) {
        EndCapacityConditionValue = (float) (sp_OP[channel].END_CAPACITY_CONDITION);

        switch (sp_OP[channel].COMMAND)
        {
            case COM_CHARGE:
            case COM_DISCHARGE:
                if ((ABS_F(sp_OP[channel].VARIABLE_CapacityResult)) >= (EndCapacityConditionValue)) {
                    return (OPERATION_END_CONDITION_CAPACITY);
                }
                break;
        }
    } 

    return (OPERATION_END_CONDITION_NONE);
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck_WATT(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 RiseFallTime[4] = { 0, };
    static Uint16 SettingTime[4] = { 0, };
    static Uint16 Index[4] = { 0, };
    float EndWattConditionValue = 0.0;

    if (sp_OP[channel].END_WATT_CONDITION) {
        EndWattConditionValue = (float) (sp_OP[channel].END_WATT_CONDITION);

        if ((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) != (RiseFallTime[channel] & 0xA)) RiseFallTime[channel] += 1;

        switch (sp_OP[channel].COMMAND)
        {
            case COM_CHARGE:
                if (Index[channel] != sp_OP[channel].STEP_INDEX) {
                    Index[channel] = sp_OP[channel].STEP_INDEX;
                    RiseFallTime[channel] = 0;
                    SettingTime[channel] = 0;
                }
                if ((ABS_F(sp_OP[channel].VARIABLE_EndConditionWattValueAVG)) >= EndWattConditionValue) {
                    if (SettingTime[channel]++ >= OPERATION_POWER_CHECK_TIME) {
                        SettingTime[channel] = 0;
                        return (OPERATION_END_CONDITION_WATT);
                    }
                }
                break;

            case COM_DISCHARGE:
                if (Index[channel] != sp_OP[channel].STEP_INDEX) {
                    Index[channel] = sp_OP[channel].STEP_INDEX;
                    RiseFallTime[channel] = 0;
                    SettingTime[channel] = 0;
                }
                if ((ABS_F(sp_OP[channel].VARIABLE_EndConditionWattValueAVG)) <= EndWattConditionValue) {
                    if (++SettingTime[channel] >= OPERATION_POWER_CHECK_TIME) {
                        SettingTime[channel] = 0;
                        return (OPERATION_END_CONDITION_WATT);
                    }
                }
                break;
        }
    }

    return (OPERATION_END_CONDITION_NONE);
}

E_OPERATION_END_CONDITION OPERATION_EndConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint32 ValueTemp[4] = { 0.0, };
    static Uint16 ReaultTemp[4] = { 0, };
    static Uint16 Flag[4] = { CLEAR, };

    E_OPERATION_END_CONDITION EndCondition = OPERATION_END_CONDITION_NONE;
    E_OPERATION_END_CONDITION EndConditionTemp = OPERATION_END_CONDITION_NONE;

#if 0
    if (sp_OP[channel].COMMAND == COM_REST) {

        EndConditionTemp = OPERATION_EndConditionCheck_TIME(sp_OP, channel);

        if(EndConditionTemp) {
            if(OPERATION_EndConditionCheck_VOLTAGE(sp_OP, channel) == OPERATION_END_CONDITION_VOLTAGE) {
                EndCondition = OPERATION_END_CONDITION_VOLTAGE;
            } else {
                EndCondition = OPERATION_END_CONDITION_TIME;
            }
        }
    } else {
        EndConditionTemp = OPERATION_EndConditionCheck_CURRENT(sp_OP, channel);
        if (EndConditionTemp) EndCondition = EndConditionTemp;

        EndConditionTemp = OPERATION_EndConditionCheck_CAPACITY(sp_OP, channel);
        if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;

        EndConditionTemp = OPERATION_EndConditionCheck_WATT(sp_OP, channel);
        if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;

        EndConditionTemp = OPERATION_EndConditionCheck_VOLTAGE(sp_OP, channel);
        if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;

        EndConditionTemp = OPERATION_EndConditionCheck_TIME(sp_OP, channel);
        if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;
    }
#else
    EndConditionTemp = OPERATION_EndConditionCheck_CURRENT(sp_OP, channel);
    if (EndConditionTemp) EndCondition = EndConditionTemp;

    EndConditionTemp = OPERATION_EndConditionCheck_CAPACITY(sp_OP, channel);
    if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;

    EndConditionTemp = OPERATION_EndConditionCheck_WATT(sp_OP, channel);
    if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;

    EndConditionTemp = OPERATION_EndConditionCheck_VOLTAGE(sp_OP, channel);
    if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;

    EndConditionTemp = OPERATION_EndConditionCheck_TIME(sp_OP, channel);
    if (EndConditionTemp && !EndCondition) EndCondition = EndConditionTemp;
#endif

    if (EndCondition && Flag[channel] == CLEAR) {
        Flag[channel] = SET;
        ReaultTemp[channel] = (EndCondition);  //1[ Time ], 2[ Ampere ], 3[ Capacity ], 4[ Watt ], 5[ Voltage ]

        switch (EndCondition)
        {
            case OPERATION_END_CONDITION_TIME:
                ValueTemp[channel] = (Uint32) (sp_OP[channel].VARIABLE_EndConditionTime & 0xFFFFFFFF);
                break;
            case OPERATION_END_CONDITION_VOLTAGE:
                ValueTemp[channel] = (Uint32) (ABS_F(sp_OP[channel].VARIABLE_VoltageResult) * 10000.0);
                break;
            case OPERATION_END_CONDITION_CURRENT:
                ValueTemp[channel] = (Uint32) (ABS_F(sp_OP[channel].VARIABLE_CurrentResult) * 10000.0);
                break;
            case OPERATION_END_CONDITION_CAPACITY:
                ValueTemp[channel] = (Uint32) (ABS_F(sp_OP[channel].VARIABLE_CapacityResult) * 10000.0);
                break;
            case OPERATION_END_CONDITION_WATT:
                ValueTemp[channel] = (Uint32) (ABS_F(sp_OP[channel].VARIABLE_PowerResult) * 1000.0);
                break;
        }
    }

    if ((sp_OP[channel].VARIABLE_TimeSpanResult % COMMUNICATION_TX_DATA_LENGTH) >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {
#if 1
        sp_OP[channel].VARIABLE_EndConditionResult = ReaultTemp[channel];   //1[ Time ], 2[ Ampere ], 3[ Capacity ], 4[ Watt ], 5[ Voltage ]
        sp_OP[channel].VARIABLE_EndConditionValue = ValueTemp[channel];     //1[ Time ], 2[ Ampere ], 3[ Capacity ], 4[ Watt ], 5[ Voltage ]

        Flag[channel] = CLEAR;
        ReaultTemp[channel] = CLEAR;
        ValueTemp[channel] = CLEAR;

        if (EndCondition) {
            EndConditionTemp = OPERATION_EndConditionCheck_LOOP(sp_OP, channel);
            if (EndConditionTemp) EndCondition = EndConditionTemp;
        }
#else
        if (EndCondition) {
            if (sp_OP[channel].CYCLE_LOOP_COUNT) {
                sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT++;

                if (sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT >= sp_OP[channel].CYCLE_LOOP_COUNT) {
                    sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;

                    EndCondition = OPERATION_END_CONDITION_GOTO;

                    if (ReaultTemp[channel]) {
                        switch (ReaultTemp[channel])
                        {
                            case OPERATION_END_CONDITION_TIME:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_TIME_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_VOLTAGE:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_VOLTAGE_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_CURRENT:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_CURRENT_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_CAPACITY:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_CAPACITY_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_WATT:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_WATT_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                        }
                    }
                } else {
                    EndCondition = OPERATION_END_CONDITION_RETRUN;  //SUCCESS Or FAIL

                    if (ReaultTemp[channel]) {
                        switch (ReaultTemp[channel])
                        {
                            case OPERATION_END_CONDITION_TIME:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_TIME_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_VOLTAGE:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_VOLTAGE_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_CURRENT:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_CURRENT_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_CAPACITY:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_CAPACITY_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                            case OPERATION_END_CONDITION_WATT:
                                if ((sp_OP[channel].STEP_INDEX + 1) != sp_OP[channel].END_WATT_INDEX) {
                                    EndCondition = ReaultTemp[channel];
                                }
                                break;
                        }
                    }
                }
            }

        }
        sp_OP[channel].VARIABLE_EndConditionResult = ReaultTemp[channel];   //1[ Time ], 2[ Ampere ], 3[ Capacity ], 4[ Watt ], 5[ Voltage ]
        sp_OP[channel].VARIABLE_EndConditionValue = ValueTemp[channel];     //1[ Time ], 2[ Ampere ], 3[ Capacity ], 4[ Watt ], 5[ Voltage ]

        Flag[channel] = CLEAR;
        ReaultTemp[channel] = CLEAR;
        ValueTemp[channel] = CLEAR;
#endif
    }

    return (EndCondition);    //SUCCESS Or FAIL
}

Uint16 OPERATION_Start(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    return (OPERATION_CellCheck(sp_OP, channel));//SUCCESS Or FAIL
}

Uint16 OPERATION_Stop(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    //STOP START
    //Step 1. Rest all of Mux

    sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_REST);
    sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_REST);
    sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);

    //Step 2. Zero of Reference All

    sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, 0.0);
    sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, 0.0);
    sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, 0.0);
    sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel, 0.0);

    //Step 3. Off of Relay All

    if (sp_OP[channel].VARIABLE_StartFlag == CLEAR) sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    //Step 3.Reset of Integration All

    sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_GND);  // CONTROL_NONE Or CONTROL_GND
    sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_ON);  // Reset On

    DELAY_US(OPERATION_INTEGRATION_RESET_TIME_US_REST);

    sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_OFF);  // Reset Off

    //STOP END

    return (SUCCESS);
}

Uint16 OPERATION_AlramReset(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    sp_OP[channel].VARIABLE_ALARM_DC.all = 0x0000;
    sp_OP[channel].VARIABLE_ALARM_FAULT.all = 0x0000;
    sp_OP[channel].VARIABLE_ALARM_GRID.all = 0x0000;
    sp_OP[channel].VARIABLE_ALARM_WARING.all = 0x0000;
    sp_OP[channel].VARIABLE_ALARM_Flag = CLEAR;
    sp_OP[channel].VARIABLE_ALARM_UpDateFlag = CLEAR;
    return (SUCCESS);  //SUCCESS Or FAIL
}

Uint16 OPERATION_Alram(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
#if 1
#if (VI_ADC_TUNE == 1)
    OPERATION_Emergency(sp_OP, channel);
//    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_PowerConditionCheck(sp_OP, channel);

    if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) < 0.02 ||
            ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) > 4.6) {
        sp_OP[channel].VARIABLE_ALARM_Flag |= (*sp_OP).VARIABLE_ALARM_WARING.bit.VoltageFault = SET;
    }

    if (ABS_F((*sp_OP).s_OldADCData.SENSING_Current) > ALARM_OVER_CURRENT_LIMIT) {
        sp_OP[channel].VARIABLE_ALARM_Flag |= (*sp_OP).VARIABLE_ALARM_WARING.bit.CurrentFault = SET;
    }

    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_HeatConditionCheck(sp_OP, channel);

    sp_OP[channel].VARIABLE_ALARM_Flag |= sp_OP[channel].VARIABLE_ALARM_WARING.bit.EMOSwitchOn;
#else
    OPERATION_Emergency(sp_OP, channel);
//    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_PowerConditionCheck(sp_OP, channel);
    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_LineConditionCheck(sp_OP, channel);
    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_CurrentConditionCheck(sp_OP, channel);
    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_VoltageConditionCheck(sp_OP, channel);
    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_HeatConditionCheck(sp_OP, channel);
    if (sp_OP[channel].DEBUG_FLAG == CLEAR) sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_CommunicationConditionCheck(sp_OP, channel);

    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_CC_Fault(sp_OP, channel);
    sp_OP[channel].VARIABLE_ALARM_Flag |= ALARM_CV_Fault(sp_OP, channel);
    sp_OP[channel].VARIABLE_ALARM_Flag |= sp_OP[channel].VARIABLE_ALARM_GRID.bit.BatteryFault;
    sp_OP[channel].VARIABLE_ALARM_Flag |= sp_OP[channel].VARIABLE_ALARM_WARING.bit.EMOSwitchOn;
    sp_OP[channel].VARIABLE_ALARM_Flag |= sp_OP[channel].VARIABLE_ALARM_WARING.bit.BatteryNone;
    sp_OP[channel].VARIABLE_ALARM_Flag |= sp_OP[channel].VARIABLE_ALARM_WARING.bit.SensingConnector;
    sp_OP[channel].VARIABLE_ALARM_Flag |= sp_OP[channel].VARIABLE_ALARM_WARING.bit.CurrentConnector;
#endif
#endif
    return (sp_OP[channel].VARIABLE_ALARM_Flag);  //SUCCESS Or FAIL
}

Uint16 OPERATION_Emergency(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    if (SENSING_Emergency() == SENSING_EMERGENCY_ON) {
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.EMOSwitchOn = SET;
    } else {
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.EMOSwitchOn = CLEAR;
    }
    return (sp_OP[channel].VARIABLE_ALARM_WARING.bit.EMOSwitchOn);    //SUCCESS Or FAIL
}

Uint16 OPERATION_ValueReset(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    sp_OP[channel].DEBUG_FLAG = CLEAR;
    sp_OP[channel].VARIABLE_UpdateData = CLEAR;
    sp_OP[channel].VARIABLE_UpdateData_OLD = CLEAR;
    sp_OP[channel].VARIABLE_UpdateAlramTime = CLEAR;

    sp_OP[channel].VARIABLE_StartFlag = CLEAR;
    sp_OP[channel].VARIABLE_VoltageRef = 0.0;
    sp_OP[channel].VARIABLE_CurrentRef = 0.0;
    sp_OP[channel].VARIABLE_PowerRef = 0.0;
    sp_OP[channel].VARIABLE_ResistanceRef = 0.0;
    sp_OP[channel].VARIABLE_VoltageResult = 0.0;
    sp_OP[channel].VARIABLE_CurrentResult = 0.0;
    sp_OP[channel].VARIABLE_PowerResult = 0.0;
    sp_OP[channel].VARIABLE_CapacityResult = 0.0;
    sp_OP[channel].VARIABLE_TimeSpanResult = 0.0;
    sp_OP[channel].VARIABLE_EndConditionResult = CLEAR;

    sp_OP[channel].VARIABLE_EndConditionTime = CLEAR;
    sp_OP[channel].VARIABLE_EndConditionResult = CLEAR;
    sp_OP[channel].VARIABLE_EndConditionValue = CLEAR;

    sp_OP[channel].VARIABLE_ALARM_Flag = CLEAR;
    sp_OP[channel].VARIABLE_ALARM_RequestFlag = CLEAR;
    sp_OP[channel].VARIABLE_ALARM_UpDateFlag = CLEAR;     //DL 2019-07-15
    sp_OP[channel].VARIABLE_ALARM_WARING.all = CLEAR;
    sp_OP[channel].VARIABLE_ALARM_GRID.all = CLEAR;
    sp_OP[channel].VARIABLE_ALARM_DC.all = CLEAR;
    sp_OP[channel].VARIABLE_ALARM_FAULT.all = CLEAR;

    sp_OP[channel].CV_MovingData[0] = CLEAR;
    sp_OP[channel].CC_MovingData[0] = CLEAR;

    sp_OP[channel].COMMAND = COM_NONE;
    sp_OP[channel].VARIABLE_COMMAND_OLD = COM_NONE;
    sp_OP[channel].DIRECT_COMMAND = COM_NONE;
    sp_OP[channel].MODE = MODE_NONE;
    sp_OP[channel].VARIABLE_MODE_OLD = MODE_NONE;
    sp_OP[channel].OPERATIN = OP_DEFAULT;
    sp_OP[channel].VARIABLE_OPERATIN = OP_SEND_DEFAULT;
    sp_OP[channel].SAMPLING_TIME = CLEAR;
    sp_OP[channel].SEND_TIME = CLEAR;
    sp_OP[channel].STEP_INDEX = CLEAR;
    sp_OP[channel].VARIABLE_STEP_INDEX = CLEAR;
    sp_OP[channel].CYCLE_LOOP_COUNT = CLEAR;
    sp_OP[channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;
    sp_OP[channel].GOTO_INDEX = CLEAR;
    sp_OP[channel].RETURN_INDEX = CLEAR;
    sp_OP[channel].END_TIME_INDEX = CLEAR;
    sp_OP[channel].END_VOLTAGE_INDEX = CLEAR;
    sp_OP[channel].END_CURRENT_INDEX = CLEAR;
    sp_OP[channel].END_CAPACITY_INDEX = CLEAR;
    sp_OP[channel].END_WATT_INDEX = CLEAR;
    sp_OP[channel].END_TIME_CONDITION = CLEAR;
    sp_OP[channel].END_VOLTAGE_CONDITION = CLEAR;
    sp_OP[channel].END_CURRENT_CONDITION = CLEAR;
    sp_OP[channel].END_WATT_CONDITION = CLEAR;
    sp_OP[channel].END_CAPACITY_CONDITION = CLEAR;
    sp_OP[channel].PATTERN_PRIOD = CLEAR;

    sp_OP[channel].s_Con.start_flag = 1;
    sp_OP[channel].s_Con.start_count = 0;
    sp_OP[channel].s_Con.current_count = 0;
    sp_OP[channel].s_Con.sensing_count = 0;

    sp_OP[channel].s_VolCon.flag = CLEAR;
    sp_OP[channel].s_VolCon.time = 30;

    return (SUCCESS);  //SUCCESS Or FAIL
}

Uint16 OPERATION_Reset(S_OPERATION_FILE *sp_OP, Uint16 channel)
{

    OPERATION_ValueReset(sp_OP, channel);
    OPERATION_AlramReset(sp_OP, channel);

//    sp_OP[channel].C_VRELAY = CONTROL_VoltageRELAY(channel, CONTROL_ON);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);    //셀 전압 검출을 위해

    sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_OFF); // Reset Off
    sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_GND);

    sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel,0.0);

    sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_REST);
    sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_REST);

    sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, 0.0);
    sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, 0.0);
    sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, 0.0);

    return(SUCCESS);//SUCCESS Or FAIL
}

Uint16 OPERATION_Rest(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    //STOP START

    if(sp_OP[channel].VARIABLE_COMMAND_OLD != sp_OP[channel].COMMAND)
    {
        //Step 1. Rest all of Mux

        sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_REST);
        sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_REST);

    sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);    //셀 전압 검출을 위해

        //Step 2. Zero of Reference All

        sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, 0.0);     // VREF 0으로
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, 0.0);     // IREF 0으로
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, 0.0);

        sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel, 0.0);     // VINT 0으로

        //Step 3. Reset of Integration All

        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_GND);  // CONTROL_NONE Or CONTROL_GND

        sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_ON);  // Reset On

        DELAY_US(OPERATION_INTEGRATION_RESET_TIME_US_REST);//Test Setting

        sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_OFF);  // Reset Off

        //STOP END
    }
    return(SUCCESS);
}

Uint16 OPERATION_Charge(S_OPERATION_FILE *sp_OP, Uint16 channel)     // Command가 Charge 아니면 Discharge로 들어옴.
{
    E_OPERATION_COM_SELECT Command = COM_NONE;
    E_CONTROL_OPERATION_MODE_SELECT e_Flag = CONTROL_REST;

    Command = sp_OP[channel].COMMAND;

    if (Command == COM_CHARGE) e_Flag = CONTROL_CHARGE;
    else e_Flag = CONTROL_DISCHARGE;

    if (sp_OP[channel].VARIABLE_COMMAND_OLD != Command)
    {
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, (float32) (0.0));           //reference, 1us
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, (float32) (0.0));
        sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel, (float32) (0.0));           //reference, 1us

        sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_OPERATION_INIT);    //mux, 10us
        sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_OPERATION_INIT);    //mux, 2us
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_OPERATION_INIT);      //mux, 2us
        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_OPEN);            //intergration, 1us
        sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_ON);            //intergration, 1us

        if (Command == COM_CHARGE) DELAY_US(OPERATION_INTEGRATION_RESET_TIME_US_CHA);
        else DELAY_US(OPERATION_INTEGRATION_RESET_TIME_US_DIS);

        sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_OFF);           //intergration, 1us
    }

    OPERATION_IntegrationSlopControl(sp_OP, channel);

    if (sp_OP[channel].VARIABLE_COMMAND_OLD != Command)
    {
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, e_Flag);                      // Operation MUX를 Command 값에 따라 Charge나 Discharge로 설정, 2us
        OPERATION_GateOffsetControl(sp_OP, channel);                                                //CONTROL_GateOffsetDAC

        if (Command == COM_CHARGE) {
            sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, OPERATION_LIMIT_MAX_VOLTAGE);  // 입력된 command가 Charge이면, VREF를 5V로(CC Mode에서 전류를 Full로 밀어넣기 위해)
        } else {
            sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, (OPERATION_LIMIT_MIN_VOLTAGE * (float) (-1.0)));  // Discharge이면, VREF를 -1V로(CC Mode에서 전류를 Full로 밀어넣기 위해)
        }

        sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, e_Flag);                  // 입력된 command에 맞게 MUX를 Charge 혹은 Discharge로 연결, 2us
        sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, e_Flag);                  // 입력된 command에 맞게 AMP를 Charge 혹은 Discharge로 연결, 10us
    }

    OPEARTION_CurrentControl(sp_OP, channel);
    OPERATION_VoltageControl(sp_OP, channel);

    return (SUCCESS);              //SUCCESS Or FAIL
}

Uint16 OPERATION_Sensing_Timer(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 Index[4] = { 0, };
    if (Index[channel] != sp_OP[channel].STEP_INDEX) {
        Index[channel] = sp_OP[channel].STEP_INDEX;
        sp_OP[channel].VARIABLE_CapacityResult = CLEAR;
    }
    sp_OP[channel].VARIABLE_CapacityResult += (ABS_F(sp_OP[channel].VARIABLE_CurrentResult) * (float)(1.0 / 3600.0) * (float)(0.001));
    return (SUCCESS);                                                       //SUCCESS Or FAIL
}

Uint16 OPERATION_Sensing(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    float32 VoltageTemp = 0.0;
    float32 CurrentTemp = 0.0;
    static Uint64 old_time_span[4] = { 0, };
    float ms_count = 0;
    static Uint16 Index[4] = { 0, };

    if (sp_OP[channel].OPERATIN == OP_CHARGE_CP
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CP
            || sp_OP[channel].OPERATIN == OP_CHARGE_CPCV
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {

        sp_OP[channel].VARIABLE_PowerRef = (float) (sp_OP[channel].VARIABLE_CurrentRef / ABS_F(sp_OP[channel].VARIABLE_VoltageResult));
        //CP kW -> W
        //sp_OP[channel].VARIABLE_PowerRef *= (float) (1000.0);

    } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CR
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CR) {

        sp_OP[channel].VARIABLE_ResistanceRef = (float) (ABS_F(sp_OP[channel].VARIABLE_VoltageResult) / sp_OP[channel].VARIABLE_CurrentRef);
    }

    SENSING_ADCResultData(&sp_OP[channel].s_ADCData, SENSING_ALL, channel); // SENSING_AD7608, SENSING_DSP_ADC

    sp_OP[channel].s_OldADCData = sp_OP[channel].s_ADCData;

    if (sp_OP[channel].COMMAND == COM_DISCHARGE) {
        VoltageTemp = CALIBRATION_ConversionVoltageADC(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage * (float) (-1.0),
                                                       VoltageDischarge_ADC[channel],
                                                       CALIBRATION_VOLATAGE_MIN_VALUE,
                                                       CALIBRATION_VOLATAGE_UNIT_VALUE,
                                                       CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH);
    } else {       // Charge
        VoltageTemp = CALIBRATION_ConversionVoltageADC(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage * (float) (-1.0),
                                                       VoltageCharge_ADC[channel],
                                                       CALIBRATION_VOLATAGE_MIN_VALUE,
                                                       CALIBRATION_VOLATAGE_UNIT_VALUE,
                                                       CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH);
        VoltageTemp *= (float) (-1.0);
    }

    sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage = VoltageTemp;

    if (sp_OP[channel].COMMAND == COM_DISCHARGE) {
        if(ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) <= 1.0)
        {
            sp_OP[channel].s_OldADCData.SENSING_Current = CALIBRATION_ConversionCurrentADC(sp_OP[channel].s_OldADCData.SENSING_Current * (float) (-1.0),
                                                                                                   CurrentDischarge_Low_ADC[channel],
                                                                                                   CurrentDischarge_Low_REF[channel],
                                                                                                   CALIBRATION_LOW_CURRENT_MIN_VALUE,
                                                                                                   CALIBRATION_LOW_CURRENT_UNIT_VALUE,
                                                                                                   CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH);
//#if(LOW_CURRENT_CALIBRATION == 1)
//            if(sp_OP[channel].OPERATIN == OP_DISCHARGE_CC){
//                if(ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) <= sp_OP[channel].VARIABLE_CurrentRef - OPERATION_CURRENT_CALIBRATION_RANGE)
//                {
//                    plus_conversion_flag = SET;
//                }
//                else if(ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) >= sp_OP[channel].VARIABLE_CurrentRef + OPERATION_CURRENT_CALIBRATION_RANGE){
//                    minus_conversion_flag = SET;
//                }
//            }
//#endif
        }
        else
        {
            sp_OP[channel].s_OldADCData.SENSING_Current = CALIBRATION_ConversionCurrentADC(sp_OP[channel].s_OldADCData.SENSING_Current * (float) (-1.0),
                                                                                                   CurrentDischarge_ADC[channel],
                                                                                                   CurrentDischarge_REF[channel],
                                                                                                   CALIBRATION_CURRENT_MIN_VALUE,
                                                                                                   CALIBRATION_CURRENT_UNIT_VALUE,
                                                                                                   CALIBRATION_CURRENT_MAX_ARRAY_LENGTH);
        }


        sp_OP[channel].s_OldADCData.SENSING_Current *= (float) (-1.0);

        CurrentTemp = sp_OP[channel].s_OldADCData.SENSING_Current;
    } else {                                                       //  Come_Charge
        if(ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) <= 1.0)
        {
            sp_OP[channel].s_OldADCData.SENSING_Current = CALIBRATION_ConversionCurrentADC(sp_OP[channel].s_OldADCData.SENSING_Current,
                                                                                           CurrentCharge_Low_ADC[channel],
                                                                                           CurrentCharge_Low_REF[channel],
                                                                                           CALIBRATION_LOW_CURRENT_MIN_VALUE,
                                                                                           CALIBRATION_LOW_CURRENT_UNIT_VALUE,
                                                                                           CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH);
//#if(LOW_CURRENT_CALIBRATION == 1)
//            if(sp_OP[channel].OPERATIN == OP_CHARGE_CC){
//                if(sp_OP[channel].s_OldADCData.SENSING_Current <= sp_OP[channel].VARIABLE_CurrentRef - OPERATION_CURRENT_CALIBRATION_RANGE)
//                {
//                    plus_conversion_flag = SET;
//                }
//                else if(sp_OP[channel].s_OldADCData.SENSING_Current >= sp_OP[channel].VARIABLE_CurrentRef + OPERATION_CURRENT_CALIBRATION_RANGE)
//                {
//                    minus_conversion_flag = SET;
//                }
//            }
//#endif
        }
        else
        {
            sp_OP[channel].s_OldADCData.SENSING_Current = CALIBRATION_ConversionCurrentADC(sp_OP[channel].s_OldADCData.SENSING_Current,
                                                                                           CurrentCharge_ADC[channel],
                                                                                           CurrentCharge_REF[channel],
                                                                                           CALIBRATION_CURRENT_MIN_VALUE,
                                                                                           CALIBRATION_CURRENT_UNIT_VALUE,
                                                                                           CALIBRATION_CURRENT_MAX_ARRAY_LENGTH);
        }
        CurrentTemp = sp_OP[channel].s_OldADCData.SENSING_Current;
    }

    sp_OP[channel].VARIABLE_VoltageResult = sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage;

    if (sp_OP[channel].COMMAND == COM_CHARGE || sp_OP[channel].COMMAND == COM_DISCHARGE) {
        sp_OP[channel].VARIABLE_CurrentResult = CurrentTemp;
    } else {
        sp_OP[channel].VARIABLE_CurrentResult = 0.0;
    }

    sp_OP[channel].VARIABLE_PowerResult = ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) * ABS_F(CurrentTemp);

#if (SENSING_TYPE == SENSING_MAIN)

    if (Index[channel] != sp_OP[channel].STEP_INDEX) {
        Index[channel] = sp_OP[channel].STEP_INDEX;
        sp_OP[channel].VARIABLE_CapacityResult = CLEAR;
    }
    if (old_time_span[channel] != sp_OP[channel].VARIABLE_TimeSpanResult) {

        if(sp_OP[channel].VARIABLE_TimeSpanResult > old_time_span[channel]) {
            ms_count = (float) (sp_OP[channel].VARIABLE_TimeSpanResult - old_time_span[channel]);
        } else {
            ms_count = (float) ((0xFFFFFFFFFFFFFFFF - old_time_span[channel]) + sp_OP[channel].VARIABLE_TimeSpanResult);
        }

        old_time_span[channel] = sp_OP[channel].VARIABLE_TimeSpanResult;
        ms_count = (ms_count / 1000.0);

        sp_OP[channel].VARIABLE_CapacityResult += (ABS_F(sp_OP[channel].VARIABLE_CurrentResult) * g_capacity_1h * ms_count);
    }

#elif (SENSING_TYPE == SENSING_TIMER)

    if (Index[channel] != sp_OP[channel].STEP_INDEX) {
        Index[channel] = sp_OP[channel].STEP_INDEX;
        sp_OP[channel].VARIABLE_CapacityResult = CLEAR;
    }

    sp_OP[channel].VARIABLE_CapacityResult += (ABS_F(sp_OP[channel].VARIABLE_CurrentResult) * (float)(1.0 / 3600.0) * (float)(0.001));
    }
#endif



    return (SUCCESS);                                                       //SUCCESS Or FAIL
}

Uint16 OPERATION_VoltageControl(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    // 여기 CV걸리면 CC 처리 않하게 프로그램 처리 해야함.
//  static Uint16 CV_Flag = CLEAR;

    float32 RefVoltage = 0.0;

    float32 NewVoltage = 0.0;
    static float Voltage[4] = { 0.0, };
    static Uint16 Index[4] = { 0, };

    NewVoltage = sp_OP[channel].VARIABLE_VoltageRef;

#if 0
    static Uint16 Timer[4] = { 30, };  //30ms
    if (sp_OP[channel].COMMAND != sp_OP[channel].VARIABLE_COMMAND_OLD) {
        Timer[channel] = 30;
    } else {
        if (Timer[channel]) {
            Timer[channel]--;
        } else {
            Timer[channel] = CLEAR;
        }
    }

    if ((!(Timer[channel]) && NewVoltage != Voltage[channel]) || sp_OP[channel].COMMAND != sp_OP[channel].VARIABLE_COMMAND_OLD) {
#else
    if (sp_OP[channel].COMMAND != sp_OP[channel].VARIABLE_COMMAND_OLD) {
        sp_OP[channel].s_VolCon.time = 30;
        sp_OP[channel].s_VolCon.flag = CLEAR;
    } else {
        sp_OP[channel].s_VolCon.flag = SET;
    }

    if ((!(sp_OP[channel].s_VolCon.time) && NewVoltage != Voltage[channel]) || sp_OP[channel].COMMAND != sp_OP[channel].VARIABLE_COMMAND_OLD || Index[channel] != sp_OP[channel].STEP_INDEX) {
#endif
        Index[channel] = sp_OP[channel].STEP_INDEX;

        if (sp_OP[channel].COMMAND == COM_CHARGE) {
            if (sp_OP[channel].OPERATIN == OP_CHARGE_CC) {
                sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, OPERATION_LIMIT_MAX_VOLTAGE);
            } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CCCV || sp_OP[channel].OPERATIN == OP_CHARGE_CPCV) {
                RefVoltage = CALIBRATION_ConversionREF(
                        sp_OP[channel].VARIABLE_VoltageRef,
                        VoltageCharge_REF[channel],
                        CALIBRATION_VOLATAGE_MIN_VALUE,
                        CALIBRATION_VOLATAGE_UNIT_VALUE,
                        CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH);
                sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, RefVoltage);
            }
            Voltage[channel] = sp_OP[channel].VARIABLE_VoltageRef;
        } else  // Discharge
        {
            if (sp_OP[channel].OPERATIN == OP_DISCHARGE_CC) {
                sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, OPERATION_LIMIT_MIN_VOLTAGE * (float) (-1.0));
            } else if (sp_OP[channel].OPERATIN == OP_DISCHARGE_CCCV || sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {
                RefVoltage = CALIBRATION_ConversionREF(
                        sp_OP[channel].VARIABLE_VoltageRef,
                        VoltageDischarge_REF[channel],
                        CALIBRATION_VOLATAGE_MIN_VALUE,
                        CALIBRATION_VOLATAGE_UNIT_VALUE,
                        CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH);
                sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, RefVoltage * (float) (-1.0));
            }
            Voltage[channel] = sp_OP[channel].VARIABLE_VoltageRef;
        }
    }
    return(SUCCESS);
}

Uint16 OPERATION_GateOffsetControl(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    float32 TotalCurrent  = 0.0;
    float32 Value = 0.0;

    if (sp_OP[channel].OPERATIN == OP_CHARGE_CP
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CP
            || sp_OP[channel].OPERATIN == OP_CHARGE_CPCV
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {
        sp_OP[channel].VARIABLE_PowerRef = (float) (sp_OP[channel].VARIABLE_CurrentRef / ABS_F(sp_OP[channel].VARIABLE_VoltageResult));
        //CP kW -> W
        //sp_OP[channel].VARIABLE_PowerRef *= (float) (1000.0);

        TotalCurrent = sp_OP[channel].VARIABLE_PowerRef;
    } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CR
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CR) {
        sp_OP[channel].VARIABLE_ResistanceRef = (float) (ABS_F(sp_OP[channel].VARIABLE_VoltageResult) / sp_OP[channel].VARIABLE_CurrentRef);

        TotalCurrent = sp_OP[channel].VARIABLE_ResistanceRef;
    } else {
        TotalCurrent = sp_OP[channel].VARIABLE_CurrentRef;
    }

    if(sp_OP[channel].COMMAND == COM_DISCHARGE)
    {
#if 0
        //Value = (OPERATION_GATEOFFSET_VOLT_GAIN_DIS / TotalCurrent) + OPERATION_GATEOFFSET_VOLT_OFFSET_DIS;
        Value = (float) ((30.0 - TotalCurrent) * 0.006) + ((5.0 - ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage)) * 0.5);
#else
        Value = 0.0;
#endif
    }
    else        // Charge
    {
#if 0
        Value = (float) ((30.0 - TotalCurrent) * 0.008) + (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) * 1.0);
#else

        if(TotalCurrent < 0.5) {
            Value = 3.0;
        } else{
            Value = 5.0;
        }
#endif
    }

    sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel , Value);

    return(SUCCESS);//SUCCESS Or FAIL
}

Uint16 OPEARTION_CurrentControl(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    float32 NewCurrent = 0.0;
    static float32 Current[4] = { 0.0, };
    Uint32 Temp32 = 0;
    Uint32 FineTemp32 = 0;
    Uint32 Temp32_x100 = 0;
    float32 Coarse = 0.0;
    float32 Fine = 0.0;
    float32 f_Temp32 = 0.0;


    if (sp_OP[channel].OPERATIN == OP_CHARGE_CP
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CP
            || sp_OP[channel].OPERATIN == OP_CHARGE_CPCV
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {
        sp_OP[channel].VARIABLE_PowerRef = (float) (sp_OP[channel].VARIABLE_CurrentRef / ABS_F(sp_OP[channel].VARIABLE_VoltageResult));//Power
        //CP kW -> W
        //sp_OP[channel].VARIABLE_PowerRef *= (float) (1000.0);

        NewCurrent = sp_OP[channel].VARIABLE_PowerRef;
    } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CR
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CR) {
        sp_OP[channel].VARIABLE_ResistanceRef = (float) (ABS_F(sp_OP[channel].VARIABLE_VoltageResult) / sp_OP[channel].VARIABLE_CurrentRef);//Resistance

        NewCurrent = sp_OP[channel].VARIABLE_ResistanceRef;
    } else {
        NewCurrent = sp_OP[channel].VARIABLE_CurrentRef;
    }

    if (sp_OP[channel].COMMAND == COM_DISCHARGE) {      // Discharge일 경우
        if(NewCurrent <= 1.0){
            f_Temp32 = (CALIBRATION_ConversionREF(
                    NewCurrent,
                    CurrentDischarge_Low_REF[channel],
                    CALIBRATION_LOW_CURRENT_MIN_VALUE,
                    CALIBRATION_LOW_CURRENT_UNIT_VALUE,
                    CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH
                    ) * 1000.0);

//#if(LOW_CURRENT_CALIBRATION == 1)
//            if(sp_OP[channel].OPERATIN == OP_DISCHARGE_CC){
//                if(plus_conversion_flag == 1){
//                    f_Temp32 += 15.634;
//                    plus_conversion_flag = 0;
//                }else if(minus_conversion_flag == 1){
//                    f_Temp32 -= 15.634;
//                    minus_conversion_flag = 0;
//                }
//            }
//#endif
            Temp32 = (Uint32) (f_Temp32);
        }else{
            f_Temp32 = (CALIBRATION_ConversionREF(
                    NewCurrent,
                    CurrentDischarge_REF[channel],
                    CALIBRATION_CURRENT_MIN_VALUE,
                    CALIBRATION_CURRENT_UNIT_VALUE,
                    CALIBRATION_CURRENT_MAX_ARRAY_LENGTH
                    ) * 1000.0);
            Temp32 = (Uint32) (f_Temp32);
        }
    }
    else {                                            // Charge일 경우
        if(NewCurrent <= 1.0){
            f_Temp32 = (CALIBRATION_ConversionREF(
                    NewCurrent,
                    CurrentCharge_Low_REF[channel],
                    CALIBRATION_LOW_CURRENT_MIN_VALUE,
                    CALIBRATION_LOW_CURRENT_UNIT_VALUE,
                    CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH
                    ) * 1000.0);
//#if(LOW_CURRENT_CALIBRATION == 1)
//            if(sp_OP[channel].OPERATIN == OP_CHARGE_CC){
//                if(plus_conversion_flag == 1){
//                    f_Temp32 += 15.634;
//                    plus_conversion_flag = 0;
//                }else if(minus_conversion_flag == 1){
//                    f_Temp32 -= 15.634;
//                    minus_conversion_flag = 0;
//                }
//            }
//#endif
            Temp32 = (Uint32) (f_Temp32);

        }else{
            f_Temp32 = (CALIBRATION_ConversionREF(
                    NewCurrent,
                    CurrentCharge_REF[channel],
                    CALIBRATION_CURRENT_MIN_VALUE,
                    CALIBRATION_CURRENT_UNIT_VALUE,
                    CALIBRATION_CURRENT_MAX_ARRAY_LENGTH
                    ) * 1000.0);
            Temp32 = (Uint32) (f_Temp32);
        }
    }

    Temp32_x100 = (Uint32) (f_Temp32 * 100.0);


#if(PID == 1)

    if(flag == 1)
    {
        if (sp_OP[channel].OPERATIN == OP_CHARGE_CC || sp_OP[channel].OPERATIN == OP_CHARGE_CCCV
                || sp_OP[channel].OPERATIN == OP_DISCHARGE_CC || sp_OP[channel].OPERATIN == OP_DISCHARGE_CCCV) {
            if (sp_OP[channel].COMMAND == COM_CHARGE) {
                current_val = sp_OP[channel].s_OldADCData.SENSING_Current;
            } else {
                current_val = (float) (-1.0) * (sp_OP[channel].s_OldADCData.SENSING_Current);
            }
            err = NewCurrent - current_val;         // 오차 = 목표치-현재값
            Kp_term = Kp * err;                     // p항 = Kp*오차

            I_err += err * dt;                      // 오차적분 += 오차*dt
            if (I_err < I_err_min) {                  // I_err은 계속 누적되므로 범위를 정해놓아야 함.
                I_err = I_err_min;
            } else if (I_err > I_err_max) {
                I_err = I_err_max;
            }
            Ki_term = Ki * I_err;                   // i항 = Ki*오차적분

            D_err = (err - prev_err) / dt;              // 오차미분 = (현재오차-이전오차)/dt
            Kd_term = Kd * D_err;                   // d항 = Kd*오차미분

            prev_err = err;                         // 현재오차를 이전오차로

            control_current = Kp_term + Ki_term + Kd_term;  // 제어량 = p항+i항+d항

            if (control_current >= NewCurrent + 200.0) {
                control_current = 200.0;            // 0.2A
            }
            f_Temp32 += control_current;
            //        pid_count = 0;
        }
    }

#endif

    if (Temp32 < (OPERATION_CURRENT_FINE_CONTROL_RANGE + 200))
    {
        Coarse = 0.2;                           //0.2A
        Fine = (f_Temp32 * 0.001) - Coarse;

        if(Current[channel] != NewCurrent || (sp_OP[channel].COMMAND) != (sp_OP[channel].VARIABLE_COMMAND_OLD)) {
            Current[channel] = NewCurrent;
            sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse);
            sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine);
        }
    }
    else
    {
        FineTemp32 = (Temp32_x100 - OPERATION_CURRENT_FINE_OFFSET * 100) % (OPERATION_CURRENT_MOD * 100);
        FineTemp32 = FineTemp32 + (OPERATION_CURRENT_FINE_OFFSET * 100);
        Temp32_x100 = Temp32_x100 - FineTemp32;

        Fine = (float32) FineTemp32 * (float32) (0.00001);
        Coarse = (float32) (Temp32_x100) * (float32) (0.00001);

        if(Current[channel] != NewCurrent || sp_OP[channel].COMMAND != sp_OP[channel].VARIABLE_COMMAND_OLD)
        {
            Current[channel] = NewCurrent;
            sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse);
            sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine);
        }
    }

    return(SUCCESS);
}


Uint16 OPERATION_IntegrationSlopControl(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    float32 new_current = 0.0;
    static float32 current[4] = { 0.0, };

    if (sp_OP[channel].OPERATIN == OP_CHARGE_CP
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CP
            || sp_OP[channel].OPERATIN == OP_CHARGE_CPCV
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CPCV) {
        sp_OP[channel].VARIABLE_PowerRef = (float) (sp_OP[channel].VARIABLE_CurrentRef / ABS_F(sp_OP[channel].VARIABLE_VoltageResult));
        //CP kW -> W
        //sp_OP[channel].VARIABLE_PowerRef *= (float) (1000.0);

        new_current = sp_OP[channel].VARIABLE_PowerRef;
    } else if (sp_OP[channel].OPERATIN == OP_CHARGE_CR
            || sp_OP[channel].OPERATIN == OP_DISCHARGE_CR) {
        sp_OP[channel].VARIABLE_ResistanceRef = (float) (ABS_F(sp_OP[channel].VARIABLE_VoltageResult) / sp_OP[channel].VARIABLE_CurrentRef);

        new_current = sp_OP[channel].VARIABLE_ResistanceRef;
    } else {
        new_current = sp_OP[channel].VARIABLE_CurrentRef;
    }

    if (current[channel] != new_current || sp_OP[channel].COMMAND != sp_OP[channel].VARIABLE_COMMAND_OLD) {

        current[channel] = new_current;

        switch (sp_OP[channel].COMMAND)
        {
        case COM_CHARGE:
            if (new_current <= 0.5) {
                   sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_1K);              // 1k
               } else if (new_current <= 2.0) {
                   sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_3R9K);            // 3.9k
               } else {
                   sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_13K);            // 13k
               }
            break;
        case COM_DISCHARGE:
            if (new_current <= 0.5) {
                        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_1K);            // 1k
                    } else if (new_current <= 2.0) {
                        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_3R9K);             // 3.9k
                    } else if (new_current <= 7.0) {
                        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_13K);             // 13k
                    } else if (new_current <= 15.0) {
                        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_24K);             // 24k
                    } else if (new_current <= 25.0) {
                        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_36K);             // 36k
                    } else {
                        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_47K);             // 47k
                    }
            break;
        }
    }
    return (SUCCESS);
}
