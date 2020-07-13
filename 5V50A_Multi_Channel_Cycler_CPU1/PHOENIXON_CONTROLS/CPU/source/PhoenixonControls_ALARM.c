/*
 * PhoenixonControls_ALARM.c
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(ALARM_VoltageFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_CurrentFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_VolatageInnerFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_VoltageReferenceFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_CurrentReferenceFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_N02VPowerFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_P07VPowerFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_P05VPowerFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_P15VPowerFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_N15VPowerFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_TemperatureFault, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_PowerConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_LineConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_CurrentConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_VoltageConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_HeatConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_CommunicationConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_BatteryConditionCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_BatteryFaultCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_BatteryNoneCheck, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_BatterySensingConnectorCheckOperating, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_BatteryCurrentConnectorCheckOperating, ".TI.ramfunc");
#pragma CODE_SECTION(ALARM_Emergency, ".TI.ramfunc");

Uint16 ALARM_VoltageFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    //전압값이 부호가 다를때 알람 발생
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };
    static Uint16 CommandFlagOld[4] = { 0, };

    if ((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) == (CheckTime[channel] & 0xA)) CheckTime[channel] += 1;

    if (sp_OP[channel].COMMAND == COMMAND_REST) {
        if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > ALARM_VOLTAGE_LIMIT) {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    } else if (sp_OP[channel].COMMAND == COMMAND_CHARGE) {   //충전시 전압 음수, 전류는 양수 측정됨
        if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > ALARM_VOLTAGE_LIMIT)   // 역전압 측정
        {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    } else if (sp_OP[channel].COMMAND == COMMAND_DISCHARGE) {   //방전시 전압 양수, 전류는 음수 측정됨
        if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage < (ALARM_VOLTAGE_LIMIT * (float) (-1.0)))  // 역전압 측정
        {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    }

    if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) >= ALARM_OVER_VOLTAGE_LIMIT)  // 초과전압 측정
    {
        if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }

    if (CheckTime[channel] >= ALARM_VOLTAGE_TIME) {
        CheckTime[channel] = CLEAR;
        SetTime[channel] = CLEAR;
    }

    if (CommandFlagOld[channel] != sp_OP[channel].COMMAND) {
        CheckTime[channel] = CLEAR;
        SetTime[channel] = CLEAR;
    }
    CommandFlagOld[channel] = sp_OP[channel].COMMAND;

    if (SetTime[channel] >= (ALARM_VOLTAGE_TIME / 2)) {
        SetTime[channel] = CLEAR;
        //SetTime[channel] = (ALARM_VOLTAGE_TIME / 2);
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.VoltageFault = SET;
        //return (SET);
    }

    return(sp_OP[channel].VARIABLE_ALARM_WARING.bit.VoltageFault); //SET Or CLEAR
}

Uint16 ALARM_CurrentFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };
    static Uint16 CommandFlagOld[4] = { 0, };
    float32 CurrentRefTemp = 0;

    if(sp_OP[channel].OPERATIN == OPERATION_CHARGE_CC_CP || sp_OP[channel].OPERATIN == OPERATION_DISCHARGE_CC_CP ||
       sp_OP[channel].OPERATIN == OPERATION_CHARGE_CV_CP || sp_OP[channel].OPERATIN == OPERATION_DISCHARGE_CV_CP)
    {
        CurrentRefTemp = sp_OP[channel].VARIABLE_PowerRef;
    }
    else if (sp_OP[channel].OPERATIN == OPERATION_CHARGE_CR || sp_OP[channel].OPERATIN == OPERATION_DISCHARGE_CR)
    {
        CurrentRefTemp = sp_OP[channel].VARIABLE_ResistanceRef;
    }
    else
    {
        CurrentRefTemp = sp_OP[channel].VARIABLE_CurrentRef;
    }

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) == (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].COMMAND == COMMAND_REST)
    {
        if(ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) > ALARM_CURRENT_LIMIT)
        {
            if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }

        if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) > ALARM_REST_OVER_CURRENT_LIMIT) {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    }
    else if(sp_OP[channel].COMMAND == COMMAND_CHARGE)
    {   //충전시 전압 음수, 전류는 양수 측정됨
        if(sp_OP[channel].s_OldADCData.SENSING_Current > (CurrentRefTemp + ALARM_CURRENT_LIMIT) ||//초과전류 측정
           sp_OP[channel].s_OldADCData.SENSING_Current < (ALARM_CURRENT_LIMIT * (float)(-1.0)))//역전류 측정
        {
            if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
        //Check Over Current
        if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) > ALARM_OVER_CURRENT_LIMIT) {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    }
    else if(sp_OP[channel].COMMAND == COMMAND_DISCHARGE)
    {   //방전시 전압 양수, 전류는 음수 측정됨
        if(sp_OP[channel].s_OldADCData.SENSING_Current < ((CurrentRefTemp + ALARM_CURRENT_LIMIT) * (float)(-1.0)) ||//초과전류 측정
           sp_OP[channel].s_OldADCData.SENSING_Current > ALARM_CURRENT_LIMIT)//역전류 측정
        {
            if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
        //Check Over Current
        if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) > ALARM_OVER_CURRENT_LIMIT) {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    }

    if(CheckTime[channel] >= ALARM_CURRENT_TIME || CommandFlagOld[channel] != sp_OP[channel].COMMAND)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    CommandFlagOld[channel] = sp_OP[channel].COMMAND;

    if(SetTime[channel] >= (ALARM_CURRENT_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.CurrentFault = SET;
    }

    return(sp_OP[channel].VARIABLE_ALARM_WARING.bit.CurrentFault); //SET Or CLEAR
}

Uint16 ALARM_VolatageInnerFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };
    static Uint16 CommandFlagOld[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) == (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if (sp_OP[channel].COMMAND == COMMAND_REST) {   //휴지시 전압 음수, 전류는 양수 측정됨
        if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > ((sp_OP[channel].s_OldADCData.SENSING_Ground_Power * (float) (-0.1)) + ALARM_VOLTAGE_LIMIT ) &&  //초과전압 측정
        sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage < ((sp_OP[channel].s_OldADCData.SENSING_Ground_Power * (float) (-0.1)) - ALARM_VOLTAGE_LIMIT ))  //미만전압 측정
        {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    } else if (sp_OP[channel].COMMAND == COMMAND_CHARGE) {   //충전시 전압 음수, 전류는 양수 측정됨
        if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > ((sp_OP[channel].s_OldADCData.SENSING_Ground_Power * (float) (-0.1)) + ALARM_VOLTAGE_LIMIT ) &&  //초과전압 측정
            sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage < ((sp_OP[channel].s_OldADCData.SENSING_Ground_Power * (float) (-0.1)) - ALARM_VOLTAGE_LIMIT ))  //미만전압 측정
        {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    } else if (sp_OP[channel].COMMAND == COMMAND_DISCHARGE) {   //방전시 전압 양수, 전류는 음수 측정됨
        if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > (sp_OP[channel].s_OldADCData.SENSING_Ground_Power + ALARM_VOLTAGE_LIMIT ) &&  //초과전압 측정
            sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage < (sp_OP[channel].s_OldADCData.SENSING_Ground_Power - ALARM_VOLTAGE_LIMIT ))  //미만전압 측정
        {
            if (SetTime[channel] == CLEAR) CheckTime[channel] = CLEAR;
            SetTime[channel]++;
        }
    }

    if (CheckTime[channel] >= ALARM_INNER_VOLTAGE_TIME) {
        CheckTime[channel] = CLEAR;
        SetTime[channel] = CLEAR;
    }

    if (CommandFlagOld[channel] != sp_OP[channel].COMMAND) {
        CheckTime[channel] = CLEAR;
        SetTime[channel] = CLEAR;
    }
    CommandFlagOld[channel] = sp_OP[channel].COMMAND;

    if (SetTime[channel] >= (ALARM_INNER_VOLTAGE_TIME / 2)) {
        SetTime[channel] = CLEAR;
        sp_OP[channel].VARIABLE_ALARM_GRID.bit.LineFault = SET;
    }

    return (sp_OP[channel].VARIABLE_ALARM_GRID.bit.LineFault);  //SET Or CLEAR
}

Uint16 ALARM_VoltageReferenceFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_CurrentReferenceFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_CC_Fault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    //배열로 무빙하고 무비 첫 데이타 더할때 마지막데이타 빼주고그값이CC때 항상 유지가 안되면 펄트
    //이건 발진으로 확인 할 수 있다네 ㅋㅋ
    static Uint16 MovingCounter[4] = { 0, };
    static Uint16 MovingStartFlag[4] = { 0, };
    static float32 DataSum[4] = { 0.0, };
    float32 DataNew = 0.0;
    float32 DtatResult = 0.0;
    float32 CurrentRefTemp = 0;

    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };
    static Uint16 CommandFlagOld[4] = { 0, };

    if(sp_OP[channel].OPERATIN == OPERATION_CHARGE_CC_CP || sp_OP[channel].OPERATIN == OPERATION_DISCHARGE_CC_CP ||
       sp_OP[channel].OPERATIN == OPERATION_CHARGE_CV_CP || sp_OP[channel].OPERATIN == OPERATION_DISCHARGE_CV_CP)
    {
        CurrentRefTemp = sp_OP[channel].VARIABLE_PowerRef;
    }
    else if (sp_OP[channel].OPERATIN == OPERATION_CHARGE_CR || sp_OP[channel].OPERATIN == OPERATION_DISCHARGE_CR)
    {
        CurrentRefTemp = sp_OP[channel].VARIABLE_ResistanceRef;
    }
    else
    {
        CurrentRefTemp = sp_OP[channel].VARIABLE_CurrentRef;
    }

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) == (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_CHARGE_CC       ||
       sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_DISCHARGE_CC    ||
       sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_CHARGE_CC_CP    ||
       sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_DISCHARGE_CC_CP)
    {
        //이동평균 시작
        DataNew = sp_OP[channel].s_OldADCData.SENSING_Current;
//      sp_OP[channel].CC_MovingData[MovingCounter[channel]] = DataNew;
        DataSum[channel] +=DataNew;

        if(++MovingCounter[channel] >= (ALARM_CC_TIME / 4) - 1)
        {
            MovingCounter[channel] = CLEAR;
            MovingStartFlag[channel] = SET;
        }
        if(MovingStartFlag[channel] == SET)
        {
            DtatResult = (float)((float)(DataSum[channel]) / (float)(ALARM_CC_TIME / 4));
            MovingCounter[channel] = (ALARM_CC_TIME / 4);
            DataSum[channel] -= sp_OP[channel].CC_MovingData[MovingCounter[channel]];//이동평균 마지막값 제외
        }
        //이동평균 끝
        if(sp_OP[channel].COMMAND == COMMAND_CHARGE)
        {   //충전시 전압 마이너스, 전류는 플러스 센싱됨
            if(DtatResult > (CurrentRefTemp + ALARM_CURRENT_LIMIT) ||//초과전류 체크
               DtatResult < (CurrentRefTemp - ALARM_CURRENT_LIMIT))//미만전류 체크
            {
                if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
                SetTime[channel]++;
            }
        }
        else if(sp_OP[channel].COMMAND == COMMAND_DISCHARGE)
        {   //방전시 전압 마이너스, 전류는 플러스 센싱됨
            if(DtatResult < ((CurrentRefTemp + ALARM_CURRENT_LIMIT) * (float)(-1.0)) ||//초과전류 체크
               DtatResult > ((CurrentRefTemp - ALARM_CURRENT_LIMIT) * (float)(-1.0)))//미만전류 체크
            {
                if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
                SetTime[channel]++;
            }
        }
    }

    if(CheckTime[channel] >= ALARM_CC_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    if(CommandFlagOld[channel] != sp_OP[channel].COMMAND)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }
    CommandFlagOld[channel] = sp_OP[channel].COMMAND;

    if(SetTime[channel] >= (ALARM_CC_TIME / 2))
    {
        SetTime[channel] = CLEAR;
//      SetTime[channel] = (ALARM_CC_TIME / 2);
//      sp_OP[channel].VARIABLE_ALARM_GRID.bit.CCFault = SET;
//      return (SET);
    }
    return(sp_OP[channel].VARIABLE_ALARM_GRID.bit.CCFault); //SET Or CLEAR
}

Uint16 ALARM_CV_Fault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    //배열로 무빙하고 무비 첫 데이타 더할때 마지막데이타 빼주고그값이CC때 항상 유지가 안되면 펄트
    //이건 발진으로 확인 할 수 있다네 ㅋㅋ
    static Uint16 MovingCounter[4] = { 0, };
    static Uint16 MovingStartFlag[4] = { 0, };
    static float32 DataSum[4] = { 0.0, };
    float32 DataNew = 0.0;
    float32 DtatResult = 0.0;
    float32 VoltageRefTemp = 0;

    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };
    static Uint16 CommandFlagOld[4] = { 0, };

    VoltageRefTemp = sp_OP[channel].VARIABLE_VoltageRef;

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) == (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_CHARGE_CV       ||
       sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_DISCHARGE_CV    ||
       sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_CHARGE_CV_CP    ||
       sp_OP[channel].VARIABLE_OPERATIN == OPERATION_SEND_DISCHARGE_CV_CP)
    {
        //이동평균 시작
        DataNew = sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage;
//      sp_OP[channel].CC_MovingData[MovingCounter[channel]] = DataNew;
        DataSum[channel] +=DataNew;

        if(++MovingCounter[channel] >= (ALARM_CV_TIME / 4) - 1)
        {
            MovingCounter[channel] = CLEAR;
            MovingStartFlag[channel] = SET;
        }
        if(MovingStartFlag[channel] == SET)
        {
            DtatResult = (float)((float)(DataSum[channel]) / (float)(ALARM_CV_TIME / 4));
            MovingCounter[channel] = (ALARM_CV_TIME / 4);
            DataSum[channel] -= sp_OP[channel].CC_MovingData[MovingCounter[channel]];//이동평균 마지막값 제외
        }
        //이동평균 끝
        if(sp_OP[channel].COMMAND == COMMAND_CHARGE)
        {   //충전시 전압 음수, 전류는 양수 센싱됨
            if(DtatResult < ((VoltageRefTemp + ALARM_VOLTAGE_LIMIT) * (float)(-1.0)) ||//초과전압 체크
               DtatResult > ((VoltageRefTemp - ALARM_VOLTAGE_LIMIT) * (float)(-1.0)))//미만전압 체크
            {
                if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
                SetTime[channel]++;
            }
        }
        else if(sp_OP[channel].COMMAND == COMMAND_DISCHARGE)
        {   //방전시 전압 양수, 전류는 음수 센싱됨
            if(DtatResult > (VoltageRefTemp + ALARM_VOLTAGE_LIMIT) ||//초과전압 체크
               DtatResult < (VoltageRefTemp - ALARM_VOLTAGE_LIMIT))//미만전압 체크
            {
                if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
                SetTime[channel]++;
            }
        }
    }

    if(CheckTime[channel] >= ALARM_CV_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }


    if(CommandFlagOld[channel] != sp_OP[channel].COMMAND)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }
    CommandFlagOld[channel] = sp_OP[channel].COMMAND;

    if(SetTime[channel] >= (ALARM_CV_TIME / 2))
    {
        SetTime[channel] = CLEAR;
//      SetTime[channel] = (ALARM_CV_TIME / 2);
//      sp_OP[channel].VARIABLE_ALARM_WARING.bit.CVFault = SET;
//      return (SET);
    }
    return(sp_OP[channel].VARIABLE_ALARM_WARING.bit.CVFault); //SET Or CLEAR
}

Uint16 ALARM_N02VPowerFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) == (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].s_OldADCData.SENSING_SMPS_Negative_0V_Power < ALARM_NEGATIVE_02V_POWER_MIN ||
       sp_OP[channel].s_OldADCData.SENSING_SMPS_Negative_0V_Power > ALARM_NEGATIVE_02V_POWER_MAX)
    {
        if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }
    else
    {
        if(CheckTime[channel] >= ALARM_NEGATIVE_02V_POWER_TIME)
        {
            CheckTime[channel] = CLEAR;
            SetTime[channel]   = CLEAR;
        }
    }

    if(SetTime[channel] >= (ALARM_NEGATIVE_02V_POWER_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        return (SET);
    }

    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_P07VPowerFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) != (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].s_OldADCData.SENSING_SMPS_Positive_7R5V_Power < ALARM_POSITIVE_07V_POWER_MIN ||
       sp_OP[channel].s_OldADCData.SENSING_SMPS_Positive_7R5V_Power > ALARM_POSITIVE_07V_POWER_MAX)
    {
        if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }

    if(CheckTime[channel] >= ALARM_POSITIVE_07V_POWER_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    if(SetTime[channel] >= (ALARM_POSITIVE_07V_POWER_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        return (SET);
    }

    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_P05VPowerFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) != (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].s_OldADCData.SENSING_M_Positive_5V_Power < ALARM_POSITIVE_05V_POWER_MIN ||
       sp_OP[channel].s_OldADCData.SENSING_M_Positive_5V_Power > ALARM_POSITIVE_05V_POWER_MAX)
    {
        if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }

    if(CheckTime[channel] >= ALARM_POSITIVE_05V_POWER_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    if(SetTime[channel] >= (ALARM_POSITIVE_05V_POWER_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        return (SET);
    }

    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_P15VPowerFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) != (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].s_OldADCData.SENSING_M_Positive_15V_Power < ALARM_POSITIVE_15V_POWER_MIN ||
       sp_OP[channel].s_OldADCData.SENSING_M_Positive_15V_Power > ALARM_POSITIVE_15V_POWER_MAX)
    {
        if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }

    if(CheckTime[channel] >= ALARM_POSITIVE_15V_POWER_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    if(SetTime[channel] >= (ALARM_POSITIVE_15V_POWER_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        return (SET);
    }

    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_N15VPowerFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) != (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].s_OldADCData.SENSING_M_Negative_15V_Power < ALARM_NEGATIVE_15V_POWER_MIN ||
       sp_OP[channel].s_OldADCData.SENSING_M_Negative_15V_Power > ALARM_NEGATIVE_15V_POWER_MAX)
    {
        if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }

    if(CheckTime[channel] >= ALARM_NEGATIVE_15V_POWER_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    if(SetTime[channel] >= (ALARM_NEGATIVE_15V_POWER_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        return (SET);
    }

    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_TemperatureFault(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    static Uint16 CheckTime[4] = { 0, };
    static Uint16 SetTime[4] = { 0, };

    if((sp_OP[channel].VARIABLE_TimeSpanResult & 0xA) != (CheckTime[channel] & 0xA))CheckTime[channel] += 1 ;

    if(sp_OP[channel].s_OldADCData.SENSING_Temperature > ALARM_TEMPERATURE_LIMIT)
    {
        if(SetTime[channel] == CLEAR)CheckTime[channel] = CLEAR;
        SetTime[channel]++;
    }

    if(CheckTime[channel] >= ALARM_TEMPERATURE_TIME)
    {
        CheckTime[channel] = CLEAR;
        SetTime[channel]   = CLEAR;
    }

    if(SetTime[channel] >= (ALARM_TEMPERATURE_TIME / 2))
    {
        SetTime[channel] = CLEAR;
        return (SET);
    }

    return(CLEAR); //SET Or CLEAR
}

Uint16 ALARM_PowerConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 CheckAlarm = 0;
    CheckAlarm |= ALARM_N02VPowerFault(sp_OP, channel);
    CheckAlarm |= ALARM_P07VPowerFault(sp_OP, channel);
    CheckAlarm |= ALARM_P05VPowerFault(sp_OP, channel);
    CheckAlarm |= ALARM_P15VPowerFault(sp_OP, channel);
    CheckAlarm |= ALARM_N15VPowerFault(sp_OP, channel);
    if (CheckAlarm) {
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.PowerFault = SET;
    }
    return (sp_OP[channel].VARIABLE_ALARM_WARING.bit.PowerFault);  //SUCCESS Or FAIL
}

Uint16 ALARM_LineConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 CheckAlarm = 0;
    CheckAlarm |= ALARM_VolatageInnerFault(sp_OP,channel);
    if(CheckAlarm)sp_OP[channel].VARIABLE_ALARM_GRID.bit.LineFault = SET;
    return(sp_OP[channel].VARIABLE_ALARM_GRID.bit.LineFault); //SUCCESS Or FAIL
}

Uint16 ALARM_CurrentConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 CheckAlarm = 0;
    CheckAlarm |= ALARM_CurrentFault(sp_OP, channel);
    CheckAlarm |= ALARM_CurrentReferenceFault(sp_OP, channel);
    if (CheckAlarm) sp_OP[channel].VARIABLE_ALARM_WARING.bit.CurrentFault = SET;
    return (sp_OP[channel].VARIABLE_ALARM_WARING.bit.CurrentFault);  //SUCCESS Or FAIL
}

Uint16 ALARM_VoltageConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 CheckAlarm = 0;

    CheckAlarm |= ALARM_VoltageFault(sp_OP,channel);
    CheckAlarm |= ALARM_VoltageReferenceFault(sp_OP,channel);
    if(CheckAlarm)sp_OP[channel].VARIABLE_ALARM_WARING.bit.VoltageFault = SET;
    return(sp_OP[channel].VARIABLE_ALARM_WARING.bit.VoltageFault); //SUCCESS Or FAIL
}

Uint16 ALARM_HeatConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 CheckAlarm = 0;
    CheckAlarm |= ALARM_TemperatureFault(sp_OP,channel);
    if(CheckAlarm)sp_OP[channel].VARIABLE_ALARM_GRID.bit.OverHeatFault = SET;
    return(sp_OP[channel].VARIABLE_ALARM_GRID.bit.OverHeatFault); //SUCCESS Or FAIL
}

Uint16 ALARM_CommunicationConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 UpdateFlag = 0;

    UpdateFlag = sp_OP[channel].VARIABLE_UpdateData;

    if(sp_OP[channel].VARIABLE_UpdateData_OLD == sp_OP[channel].VARIABLE_UpdateData)
        sp_OP[channel].VARIABLE_UpdateAlramTime++;// =CheckTime++;
    else sp_OP[channel].VARIABLE_UpdateAlramTime = CLEAR;

    if(sp_OP[channel].VARIABLE_UpdateAlramTime >= ALARM_COMMUINCATION_TIME)
        sp_OP[channel].VARIABLE_ALARM_DC.bit.CommunicationFault = SET;

    sp_OP[channel].VARIABLE_UpdateData_OLD = UpdateFlag;
    return(sp_OP[channel].VARIABLE_ALARM_DC.bit.CommunicationFault); //SUCCESS Or FAIL
}

Uint16 ALARM_BatteryConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    Uint16 CheckAlarm = 0;

    if(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > ALARM_BATTERY_VOLTAGE_LIMIT)
    {
        //ToDo : 버터리 역삽
        sp_OP[channel].VARIABLE_ALARM_GRID.bit.BatteryFault = SET;
    }

    if(ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) < ALARM_BATTERY_CURRENT_LIMIT)//충전때 부호가 어떻게 되나요?
    {
        //ToDo : 버터리 없음
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.BatteryNone = SET;
    }

    CheckAlarm |= sp_OP[channel].VARIABLE_ALARM_GRID.bit.BatteryFault;
    CheckAlarm |= sp_OP[channel].VARIABLE_ALARM_WARING.bit.BatteryNone;

    return(CheckAlarm); //SUCCESS Or FAIL
}

Uint16 ALARM_BatteryFaultCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    if(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > ALARM_BATTERY_VOLTAGE_LIMIT) //check 100mV
    {
        //ToDo : 버터리 역삽
        sp_OP[channel].VARIABLE_ALARM_GRID.bit.BatteryFault = SET;
    }

    return(sp_OP[channel].VARIABLE_ALARM_GRID.bit.BatteryFault); //SUCCESS Or FAIL
}

Uint16 ALARM_BatteryNoneCheck(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    //If the battery is properly connected, Maintain negative voltage in idle condition
    if (sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage > -0.05)    //check -50mV
    {
        sp_OP[channel].VARIABLE_ALARM_WARING.bit.BatteryNone = SET;
    }

    return(sp_OP[channel].VARIABLE_ALARM_WARING.bit.BatteryNone); //SUCCESS Or FAIL
}

void ALARM_BatterySensingConnectorCheckOperating(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) < 0.05) //check 50mV
    {
        sp_OP[channel].s_Con.sensing_count++;
        if (sp_OP[channel].s_Con.sensing_count >= 100) {
        	sp_OP[channel].VARIABLE_ALARM_WARING.bit.SensingConnector = SET;
        }
    } else {
        sp_OP[channel].s_Con.sensing_count = 0;
    }
}

void ALARM_BatteryCurrentConnectorCheckOperating(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    if (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current) < 0.010)     //check 10mA   190530 차성호 수정
    {
        sp_OP[channel].s_Con.current_count++;
        if (sp_OP[channel].s_Con.current_count >= 100)
        {
        	sp_OP[channel].VARIABLE_ALARM_WARING.bit.CurrentConnector = SET;
        }
    }
    else {
        sp_OP[channel].s_Con.current_count = 0;
    }
}

Uint16 ALARM_Emergency(S_OPERATION_FILE *sp_OP, Uint16 channel)
{
    return(sp_OP[channel].VARIABLE_ALARM_WARING.bit.EMOSwitchOn); //SUCCESS Or FAIL
}

//
// End of file
//


