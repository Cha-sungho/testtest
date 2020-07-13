/*
 * PhoenixonControls_TIMER.c
 *
 *  Created on: 2018. 5. 21.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"
#include <string.h>

extern volatile S_SCHEDULE_DATA SCHEDULE_DATA[CONTROL_CHANNEL_PER_BOARD];
extern volatile S_CELL_SCAN_DATA CELL_SCAN_DATA[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 GSRAM_DATA_BANK_INDEX_CPU1[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 GSRAM_DATA_BANK_INDEX_CPU2[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 SYNC_SIGNAL_CPU1;
extern volatile Uint32 SYNC_SIGNAL_CPU2;
extern volatile Uint32 ui_UpdateflagCPU1[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 ui_UpdateflagCPU2[CONTROL_CHANNEL_PER_BOARD];

extern volatile Uint32 ui_UpdateSTEPCPU1[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 ui_UpdateSTEPCPU2[CONTROL_CHANNEL_PER_BOARD];

extern volatile Uint32 ui_UpdateDATACPU1[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 ui_UpdateDATACPU2[CONTROL_CHANNEL_PER_BOARD];

extern volatile S_COMMUNICATION_TX_DATA s_DATA[CONTROL_CHANNEL_PER_BOARD][COMMUNICATION_TX_DEVICE_BANK];
extern volatile S_COMMUNICATION_RX_JUMP s_JUMP;
extern volatile S_COMMUNICATION_RX_COMMAD s_COMMAND;
extern volatile S_COMMUNICATION_RX_START s_START;
extern volatile S_COMMUNICATION_RX_PAUSE s_PAUSE;

extern volatile S_COMMUNICATION_TX_END s_SENSING_END;
extern volatile S_COMMUNICATION_TX_ALARM s_SENSING_ALARM;

#pragma CODE_SECTION(DSP_Timer0_ISR, ".TI.ramfunc");
#pragma CODE_SECTION(DSP_Timer1_ISR, ".TI.ramfunc");
#pragma CODE_SECTION(DSP_Timer2_ISR, ".TI.ramfunc");

extern S_OPERATION_FILE s_Operation[CONTROL_CHANNEL_PER_BOARD];

volatile Uint16 FlagPause[CONTROL_CHANNEL_PER_BOARD] = { CLEAR, };
volatile Uint16 UpdateFlagStepStart[CONTROL_CHANNEL_PER_BOARD] = { CLEAR, };
volatile Uint16 UpdateFlagStepEnd[CONTROL_CHANNEL_PER_BOARD] = { CLEAR, };
volatile float32 VoltageAVG[CONTROL_CHANNEL_PER_BOARD] = { 0.0, };
volatile float32 CurrentAVG[CONTROL_CHANNEL_PER_BOARD] = { 0.0, };
volatile float32 WattAVG[CONTROL_CHANNEL_PER_BOARD] = { 0.0, };
volatile Uint32 AVGCounter[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile Uint16 DataCount[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile Uint16 Bank[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile E_OPERATION_COM_SELECT SaveCOMMAND[CONTROL_CHANNEL_PER_BOARD] = { COM_NONE, };
volatile E_OPERATION_END_CONDITION EndConditionState[CONTROL_CHANNEL_PER_BOARD] = { OPERATION_END_CONDITION_NONE, };

Uint16 g_old_step_index[4] = { 0, };

void DSP_TimerInit(void)
{
    //
    // Initialize timer period to maximum
    //
    CPUTimer_setPeriod(CPUTIMER0_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER1_BASE, 0xFFFFFFFF);
    CPUTimer_setPeriod(CPUTIMER2_BASE, 0xFFFFFFFF);

    //
    // Initialize pre-scale counter to divide by 1 (SYSCLKOUT)
    //
    CPUTimer_setPreScaler(CPUTIMER0_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER1_BASE, 0);
    CPUTimer_setPreScaler(CPUTIMER2_BASE, 0);

    //
    // Make sure timer is stopped
    //
    CPUTimer_stopTimer(CPUTIMER0_BASE);
    CPUTimer_stopTimer(CPUTIMER1_BASE);
    CPUTimer_stopTimer(CPUTIMER2_BASE);

    //
    // Reload all counter register with period value
    //
    CPUTimer_reloadTimerCounter(CPUTIMER0_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER1_BASE);
    CPUTimer_reloadTimerCounter(CPUTIMER2_BASE);

    DSP_Configimer(CPUTIMER0_BASE, DEVICE_SYSCLK_FREQ, DSP_TIMER0_TIME);
    DSP_Configimer(CPUTIMER1_BASE, DEVICE_SYSCLK_FREQ, DSP_TIMER1_TIME);
    DSP_Configimer(CPUTIMER2_BASE, DEVICE_SYSCLK_FREQ, DSP_TIMER2_TIME);
}

void DSP_Configimer(Uint32 Timer, float Frequency, float Period)
{
    Uint32 ul_Temp;

    //
    // Initialize timer period:
    //
    ul_Temp = (Uint32) (Frequency / 1000000 * Period);
    CPUTimer_setPeriod(Timer, ul_Temp);

    //
    // Set pre-scale counter to divide by 1 (SYSCLKOUT):
    //
    CPUTimer_setPreScaler(Timer, 0);

    //
    // Initializes timer control register. The timer is stopped, reloaded,
    // free run disabled, and interrupt enabled.
    // Additionally, the free and soft bits are set
    //
    CPUTimer_stopTimer(Timer);
    CPUTimer_reloadTimerCounter(Timer);
    CPUTimer_setEmulationMode(Timer, CPUTIMER_EMULATIONMODE_STOPAFTERNEXTDECREMENT);
    CPUTimer_enableInterrupt(Timer);
}

Uint16 int_ch_check = 0;

__interrupt void DSP_Timer0_ISR(void)
{
    //For loop time measurement
    //GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;

    //channel 0
    Bank[0] = (GSRAM_DATA_BANK_INDEX_CPU1[0] + 1) % COMMUNICATION_TX_DEVICE_BANK;
    DataCount[0] = (s_Operation[0].VARIABLE_TimeSpanResult % COMMUNICATION_TX_DATA_LENGTH);

    //Moved to the main loop
#if (SENSING_TYPE == SENSING_TIMER)
    OPERATION_Sensing(s_Operation, 0);
#elif (SENSING_TYPE == SENSING_SEPERATE)
    OPERATION_Sensing_Timer(s_Operation, 0);
#endif

    OPERATION_ModeCheck(s_Operation, 0);
    //OPERATION_Alram(s_Operation, 0);

    COMMUNICATION_COMMANDRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0);

    //Removed empty functions
    //COMMUNICATION_JUMPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0);
    //COMMUNICATION_PAUSERead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0);

    if (COMMUNICATION_STARTRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0)) {
        DataCount[0] = CLEAR;
    }

#if 0
    //For Timespan initial
    if (g_old_step_index[0] != s_Operation[0].STEP_INDEX) {
        g_old_step_index[0] = s_Operation[0].STEP_INDEX;
        s_Operation[0].VARIABLE_TimeSpanResult = 0;
    }
#endif

    VoltageAVG[0] += s_Operation[0].VARIABLE_VoltageResult;
    CurrentAVG[0] += s_Operation[0].VARIABLE_CurrentResult;
    WattAVG[0] += s_Operation[0].VARIABLE_PowerResult;
    AVGCounter[0]++;

    if (DataCount[0] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        s_Operation[0].VARIABLE_EndConditionVoltageValueAVG = VoltageAVG[0] / (float32) (AVGCounter[0]);
        s_Operation[0].VARIABLE_EndConditionCurrentValueAVG = CurrentAVG[0] / (float32) (AVGCounter[0]);
        s_Operation[0].VARIABLE_EndConditionWattValueAVG = WattAVG[0] / (float32) (AVGCounter[0]);

        if (UpdateFlagStepStart[0] == CLEAR && EndConditionState[0] == OPERATION_END_CONDITION_UPDATE_END) {
            EndConditionState[0] = OPERATION_EndConditionCheck(s_Operation, 0);
        } else {
            EndConditionState[0] = OPERATION_END_CONDITION_NONE_OP;
        }

        VoltageAVG[0] = 0.0;
        CurrentAVG[0] = 0.0;
        WattAVG[0] = 0.0;
        AVGCounter[0] = 0;
    }

    COMMUNICATION_DATAWrite(SCHEDULE_DATA, &(s_DATA[0][Bank[0]].DATA[DataCount[0]]), s_Operation, 0);

    if (DataCount[0] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        //Jump 구간
        if (CELL_SCAN_DATA[0].CPU2_Jump != SCHEDULE_DATA[0].CPU1_Jump && s_Operation[0].VARIABLE_StartFlag == SET)  //짬푸할때는 엔드 조건이 0이어야 합니다.
        {
            if (SCHEDULE_DATA[0].CPU1_STEP[0].STEP_INDEX == SCHEDULE_DATA[0].CPU1_JUMP.STEP_INDEX) {
                COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0, 0);
                //DL 2019-06-09 (JUMP ECHO)
                if ((s_Operation[0].CYCLE_INDEX == s_Operation[0].PATTERN_PRIOD) && (s_Operation[0].STEP_INDEX < s_Operation[0].STEP_INDEX_OLD)) {
                    s_Operation[0].VARIABLE_CYCLE_LOOP_COUNT++;
                }
                CELL_SCAN_DATA[0].CPU2_Jump = SCHEDULE_DATA[0].CPU1_Jump;
            } else {
                s_Operation[0].STEP_INDEX = SCHEDULE_DATA[0].CPU1_JUMP.STEP_INDEX;
            }
            EndConditionState[0] = OPERATION_END_CONDITION_NONE_OP;

            //Modified Jump end condition (To be Updated)
            s_DATA[0][Bank[0]].DATA[DataCount[0]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_NONE_OP;

        }  //Pause 구간
        else if (CELL_SCAN_DATA[0].CPU2_Pause != SCHEDULE_DATA[0].CPU1_Pause) {
            switch (SCHEDULE_DATA[0].CPU1_PAUSE.COMMAND)
            {
                case OPERATION_PAUSE_STOP:
                    SaveCOMMAND[0] = COM_NONE;
                    s_DATA[0][Bank[0]].DATA[DataCount[0]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_PAUSE:
                    if (FlagPause[0] == CLEAR) {
                        SaveCOMMAND[0] = s_Operation[0].COMMAND;
                    }
                    s_Operation[0].COMMAND = COM_REST;
                    FlagPause[0] = SET;
                    EndConditionState[0] = OPERATION_END_CONDITION_NONE_OP;
                    s_DATA[0][Bank[0]].DATA[DataCount[0]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_COMPLETE:
                    SaveCOMMAND[0] = COM_NONE;
                    FlagPause[0] = CLEAR;

                    s_Operation[0].STEP_INDEX = INDEX_STEP_END;
                    EndConditionState[0] = OPERATION_END_CONDITION_NONE_OP;

                    CELL_SCAN_DATA[0].CPU2_Pause = SCHEDULE_DATA[0].CPU1_Pause;
                    s_Operation[0].VARIABLE_ALARM_UpDateFlag = CLEAR;  //DL 2019-07-15
                    s_DATA[0][Bank[0]].DATA[DataCount[0]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_UPDATE_END;  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_RESTART:
                    s_Operation[0].COMMAND = SaveCOMMAND[0];
                    SaveCOMMAND[0] = COM_NONE;
                    FlagPause[0] = CLEAR;
                    EndConditionState[0] = OPERATION_END_CONDITION_NONE;
                    CELL_SCAN_DATA[0].CPU2_Pause = SCHEDULE_DATA[0].CPU1_Pause;
                    s_DATA[0][Bank[0]].DATA[DataCount[0]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;
            }
        }

        if (s_Operation[0].VARIABLE_ALARM_Flag) {

            s_Operation[0].OPERATIN = OP_DEFAULT;
            s_Operation[0].STEP_INDEX = INDEX_STEP_END;
            EndConditionState[0] = OPERATION_END_CONDITION_NONE_OP;
            s_DATA[0][Bank[0]].DATA[DataCount[0]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_ALARM);  //DL 2019-07-11 (SW engineer request)
        }

        //Simply changed to the condition
        if (FlagPause[0] == CLEAR) {
            if (s_Operation[0].VARIABLE_StartFlag == SET) {
                UpdateFlagStepStart[0] = SET;

                if (UpdateFlagStepEnd[0] == SET) {
                    COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0, (Uint16) EndConditionState[0]);
                    EndConditionState[0] = OPERATION_END_CONDITION_UPDATE_END;
                    UpdateFlagStepEnd[0] = CLEAR;
                }
            } else {
                s_Operation[0].STEP_INDEX = OPERATION_END_CONDITION_NONE;
            }
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 0);

        COMMUNICATION_ALARMWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 0);

        s_DATA[0][Bank[0]].Header = HEADER_DATA;
        s_DATA[0][Bank[0]].DataCount = COMMUNICATION_INTEGER_LENGTH_DATA * 2;
        s_DATA[0][Bank[0]].ChannelIndex = (1 << (0 + CONTROL_CHANNEL_START_INDEX ));

        CELL_SCAN_DATA[0].CPU2_DATA = s_DATA[0][Bank[0]];

        CELL_SCAN_DATA[0].CPU2_DataBank = Bank[0];
        CELL_SCAN_DATA[0].CPU2_StepIndex = s_Operation[0].STEP_INDEX;

        CELL_SCAN_DATA[0].CPU2_Data = Bank[0];

        GSRAM_DATA_BANK_INDEX_CPU1[0]++;
        GSRAM_DATA_BANK_INDEX_CPU1[0] %= COMMUNICATION_TX_DEVICE_BANK;
        ui_UpdateDATACPU1[0]++;

        if (s_Operation[0].STEP_INDEX == INDEX_STEP_END) {
            OPERATION_Stop(s_Operation, 0);
            s_Operation[0].C_IRELAY = CONTROL_CurrentRELAY(0, CONTROL_OFF);
        }
    }

    //Moved to the main loop
    //OPERATION_Run(s_Operation, 0);

    s_Operation[0].VARIABLE_TimeSpanResult++;

    //Simply changed to the condition
    if (s_Operation[0].VARIABLE_StartFlag == SET) {
        if (FlagPause[0] == SET) {
            if (ui_UpdateSTEPCPU1[0] != ui_UpdateSTEPCPU2[0]) {
                s_Operation[0].VARIABLE_UpdateData++;
                ui_UpdateSTEPCPU1[0] = ui_UpdateSTEPCPU2[0];
            }
        } else {
            if (UpdateFlagStepStart[0] == SET) {
                if (ui_UpdateSTEPCPU1[0] != ui_UpdateSTEPCPU2[0]) {

                    s_Operation[0].VARIABLE_UpdateData++;
                    if (s_Operation[0].STEP_INDEX == SCHEDULE_DATA[0].CPU1_STEP[0].STEP_INDEX) {

                        COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0, 0);
                        UpdateFlagStepStart[0] = CLEAR;
                        UpdateFlagStepEnd[0] = SET;
                    }
                    ui_UpdateSTEPCPU1[0] = ui_UpdateSTEPCPU2[0];
                }
            }
        }
    }

    //channel 1
    Bank[1] = (GSRAM_DATA_BANK_INDEX_CPU1[1] + 1) % COMMUNICATION_TX_DEVICE_BANK;
    DataCount[1] = (s_Operation[1].VARIABLE_TimeSpanResult % COMMUNICATION_TX_DATA_LENGTH);

    //Moved to the main loop
#if (SENSING_TYPE == SENSING_TIMER)
    OPERATION_Sensing(s_Operation, 1);
#elif (SENSING_TYPE == SENSING_SEPERATE)
    OPERATION_Sensing_Timer(s_Operation, 1);
#endif
    OPERATION_ModeCheck(s_Operation, 1);
    //OPERATION_Alram(s_Operation, 1);

    COMMUNICATION_COMMANDRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1);

    //Removed empty functions
    //COMMUNICATION_JUMPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1);
    //COMMUNICATION_PAUSERead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1);

    if (COMMUNICATION_STARTRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1)) {
        DataCount[1] = CLEAR;
    }

#if 0
    //For Timespan initial
    if (g_old_step_index[1] != s_Operation[1].STEP_INDEX) {
        g_old_step_index[1] = s_Operation[1].STEP_INDEX;
        s_Operation[1].VARIABLE_TimeSpanResult = 0;
    }
#endif

    VoltageAVG[1] += s_Operation[1].VARIABLE_VoltageResult;
    CurrentAVG[1] += s_Operation[1].VARIABLE_CurrentResult;
    WattAVG[1] += s_Operation[1].VARIABLE_PowerResult;
    AVGCounter[1]++;

    if (DataCount[1] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        s_Operation[1].VARIABLE_EndConditionVoltageValueAVG = VoltageAVG[1] / (float32) (AVGCounter[1]);
        s_Operation[1].VARIABLE_EndConditionCurrentValueAVG = CurrentAVG[1] / (float32) (AVGCounter[1]);
        s_Operation[1].VARIABLE_EndConditionWattValueAVG = WattAVG[1] / (float32) (AVGCounter[1]);

        if (UpdateFlagStepStart[1] == CLEAR && EndConditionState[1] == OPERATION_END_CONDITION_UPDATE_END) {
            EndConditionState[1] = OPERATION_EndConditionCheck(s_Operation, 1);
        } else {
            EndConditionState[1] = OPERATION_END_CONDITION_NONE_OP;
        }

        VoltageAVG[1] = 0.0;
        CurrentAVG[1] = 0.0;
        WattAVG[1] = 0.0;
        AVGCounter[1] = 0;
    }

    COMMUNICATION_DATAWrite(SCHEDULE_DATA, &(s_DATA[1][Bank[1]].DATA[DataCount[1]]), s_Operation, 1);

    if (DataCount[1] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        //Jump 구간
        if (CELL_SCAN_DATA[1].CPU2_Jump != SCHEDULE_DATA[1].CPU1_Jump && s_Operation[1].VARIABLE_StartFlag == SET)  //짬푸할때는 엔드 조건이 0이어야 합니다.
        {

            if (SCHEDULE_DATA[1].CPU1_STEP[0].STEP_INDEX == SCHEDULE_DATA[1].CPU1_JUMP.STEP_INDEX) {
                COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1, 0);
                //DL 2019-06-09 (JUMP ECHO)
                if ((s_Operation[1].CYCLE_INDEX == s_Operation[1].PATTERN_PRIOD) && (s_Operation[1].STEP_INDEX < s_Operation[1].STEP_INDEX_OLD)) {
                    s_Operation[1].VARIABLE_CYCLE_LOOP_COUNT++;
                }
                CELL_SCAN_DATA[1].CPU2_Jump = SCHEDULE_DATA[1].CPU1_Jump;
            } else {
                s_Operation[1].STEP_INDEX = SCHEDULE_DATA[1].CPU1_JUMP.STEP_INDEX;
            }
            EndConditionState[1] = OPERATION_END_CONDITION_NONE_OP;

            //Modified Jump end condition (To be Updated)
            s_DATA[1][Bank[1]].DATA[DataCount[1]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_NONE_OP;

        }  //Pause 구간
        else if (CELL_SCAN_DATA[1].CPU2_Pause != SCHEDULE_DATA[1].CPU1_Pause) {
            switch (SCHEDULE_DATA[1].CPU1_PAUSE.COMMAND)
            {
                case OPERATION_PAUSE_STOP:
                    SaveCOMMAND[1] = COM_NONE;
                    s_DATA[1][Bank[1]].DATA[DataCount[1]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);
                    break;

                case OPERATION_PAUSE_PAUSE:
                    if (FlagPause[1] == CLEAR) {
                        SaveCOMMAND[1] = s_Operation[1].COMMAND;
                    }
                    s_Operation[1].COMMAND = COM_REST;
                    FlagPause[1] = SET;
                    EndConditionState[1] = OPERATION_END_CONDITION_NONE_OP;
                    s_DATA[1][Bank[1]].DATA[DataCount[1]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_COMPLETE:
                    SaveCOMMAND[1] = COM_NONE;
                    FlagPause[1] = CLEAR;

                    s_Operation[1].STEP_INDEX = INDEX_STEP_END;
                    EndConditionState[1] = OPERATION_END_CONDITION_NONE_OP;

                    CELL_SCAN_DATA[1].CPU2_Pause = SCHEDULE_DATA[1].CPU1_Pause;
                    s_Operation[1].VARIABLE_ALARM_UpDateFlag = CLEAR;  //DL 2019-07-15
                    s_DATA[1][Bank[1]].DATA[DataCount[1]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_UPDATE_END;  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_RESTART:
                    s_Operation[1].COMMAND = SaveCOMMAND[1];
                    SaveCOMMAND[1] = COM_NONE;
                    FlagPause[1] = CLEAR;
                    EndConditionState[1] = OPERATION_END_CONDITION_NONE;
                    CELL_SCAN_DATA[1].CPU2_Pause = SCHEDULE_DATA[1].CPU1_Pause;
                    s_DATA[1][Bank[1]].DATA[DataCount[1]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;
            }
        }

        if (s_Operation[1].VARIABLE_ALARM_Flag) {

            s_Operation[1].OPERATIN = OP_DEFAULT;
            s_Operation[1].STEP_INDEX = INDEX_STEP_END;
            EndConditionState[1] = OPERATION_END_CONDITION_NONE_OP;
            s_DATA[1][Bank[1]].DATA[DataCount[1]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_ALARM);  //DL 2019-07-11 (SW engineer request)
        }

        //Simply changed to the condition
        if (FlagPause[1] == CLEAR) {
            if (s_Operation[1].VARIABLE_StartFlag == SET) {
                UpdateFlagStepStart[1] = SET;

                if (UpdateFlagStepEnd[1] == SET) {
                    COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1, (Uint16) EndConditionState[1]);
                    EndConditionState[1] = OPERATION_END_CONDITION_UPDATE_END;
                    UpdateFlagStepEnd[1] = CLEAR;
                }
            } else {
                s_Operation[1].STEP_INDEX = OPERATION_END_CONDITION_NONE;
            }
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 1);

        COMMUNICATION_ALARMWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 1);

        s_DATA[1][Bank[1]].Header = HEADER_DATA;
        s_DATA[1][Bank[1]].DataCount = COMMUNICATION_INTEGER_LENGTH_DATA * 2;
        s_DATA[1][Bank[1]].ChannelIndex = (1 << (1 + CONTROL_CHANNEL_START_INDEX ));

        CELL_SCAN_DATA[1].CPU2_DATA = s_DATA[1][Bank[1]];

        CELL_SCAN_DATA[1].CPU2_DataBank = Bank[1];
        CELL_SCAN_DATA[1].CPU2_StepIndex = s_Operation[1].STEP_INDEX;

        CELL_SCAN_DATA[1].CPU2_Data = Bank[1];

        GSRAM_DATA_BANK_INDEX_CPU1[1]++;
        GSRAM_DATA_BANK_INDEX_CPU1[1] %= COMMUNICATION_TX_DEVICE_BANK;
        ui_UpdateDATACPU1[1]++;

        if (s_Operation[1].STEP_INDEX == INDEX_STEP_END) {
            OPERATION_Stop(s_Operation, 1);
            s_Operation[1].C_IRELAY = CONTROL_CurrentRELAY(1, CONTROL_OFF);
        }
    }

    //Moved to the main loop
    //OPERATION_Run(s_Operation, 1);

    s_Operation[1].VARIABLE_TimeSpanResult++;

    //Simply changed to the condition
    if (s_Operation[1].VARIABLE_StartFlag == SET) {
        if (FlagPause[1] == SET) {
            if (ui_UpdateSTEPCPU1[1] != ui_UpdateSTEPCPU2[1]) {
                s_Operation[1].VARIABLE_UpdateData++;
                ui_UpdateSTEPCPU1[1] = ui_UpdateSTEPCPU2[1];
            }
        } else {
            if (UpdateFlagStepStart[1] == SET) {
                if (ui_UpdateSTEPCPU1[1] != ui_UpdateSTEPCPU2[1]) {

                    s_Operation[1].VARIABLE_UpdateData++;
                    if (s_Operation[1].STEP_INDEX == SCHEDULE_DATA[1].CPU1_STEP[0].STEP_INDEX) {

                        COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1, 0);
                        UpdateFlagStepStart[1] = CLEAR;
                        UpdateFlagStepEnd[1] = SET;
                    }
                    ui_UpdateSTEPCPU1[1] = ui_UpdateSTEPCPU2[1];
                }
            }
        }
    }

    //For loop time measurement
    //GpioDataRegs.GPDSET.bit.GPIO99 = 1;
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}

__interrupt void DSP_Timer1_ISR(void)
{
#if 1
    Bank[2] = (GSRAM_DATA_BANK_INDEX_CPU1[2] + 1) % COMMUNICATION_TX_DEVICE_BANK;
    DataCount[2] = (s_Operation[2].VARIABLE_TimeSpanResult % COMMUNICATION_TX_DATA_LENGTH);

    //Moved to the main loop
#if (SENSING_TYPE == SENSING_TIMER)
    OPERATION_Sensing(s_Operation, 2);
#elif (SENSING_TYPE == SENSING_SEPERATE)
    OPERATION_Sensing_Timer(s_Operation, 2);
#endif
    OPERATION_ModeCheck(s_Operation, 2);
    //OPERATION_Alram(s_Operation, 2);

    COMMUNICATION_COMMANDRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2);

    //Removed empty functions
    //COMMUNICATION_JUMPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2);
    //COMMUNICATION_PAUSERead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2);

    if (COMMUNICATION_STARTRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2)) {
        DataCount[2] = CLEAR;
    }

#if 0
    //For Timespan initial
    if (g_old_step_index[2] != s_Operation[2].STEP_INDEX) {
        g_old_step_index[2] = s_Operation[2].STEP_INDEX;
        s_Operation[2].VARIABLE_TimeSpanResult = 0;
    }
#endif

    VoltageAVG[2] += s_Operation[2].VARIABLE_VoltageResult;
    CurrentAVG[2] += s_Operation[2].VARIABLE_CurrentResult;
    WattAVG[2] += s_Operation[2].VARIABLE_PowerResult;
    AVGCounter[2]++;

    if (DataCount[2] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        s_Operation[2].VARIABLE_EndConditionVoltageValueAVG = VoltageAVG[2] / (float32) (AVGCounter[2]);
        s_Operation[2].VARIABLE_EndConditionCurrentValueAVG = CurrentAVG[2] / (float32) (AVGCounter[2]);
        s_Operation[2].VARIABLE_EndConditionWattValueAVG = WattAVG[2] / (float32) (AVGCounter[2]);

        if (UpdateFlagStepStart[2] == CLEAR && EndConditionState[2] == OPERATION_END_CONDITION_UPDATE_END) {
            EndConditionState[2] = OPERATION_EndConditionCheck(s_Operation, 2);
        } else {
            EndConditionState[2] = OPERATION_END_CONDITION_NONE_OP;
        }

        VoltageAVG[2] = 0.0;
        CurrentAVG[2] = 0.0;
        WattAVG[2] = 0.0;
        AVGCounter[2] = 0;
    }

    COMMUNICATION_DATAWrite(SCHEDULE_DATA, &(s_DATA[2][Bank[2]].DATA[DataCount[2]]), s_Operation, 2);

    if (DataCount[2] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        //Jump 구간
        if (CELL_SCAN_DATA[2].CPU2_Jump != SCHEDULE_DATA[2].CPU1_Jump && s_Operation[2].VARIABLE_StartFlag == SET)  //짬푸할때는 엔드 조건이 0이어야 합니다.
        {
            if (SCHEDULE_DATA[2].CPU1_STEP[0].STEP_INDEX == SCHEDULE_DATA[2].CPU1_JUMP.STEP_INDEX) {
                COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2, 0);
                //DL 2019-06-09 (JUMP ECHO)
                if ((s_Operation[2].CYCLE_INDEX == s_Operation[2].PATTERN_PRIOD) && (s_Operation[2].STEP_INDEX < s_Operation[2].STEP_INDEX_OLD)) {
                    s_Operation[2].VARIABLE_CYCLE_LOOP_COUNT++;
                }
                CELL_SCAN_DATA[2].CPU2_Jump = SCHEDULE_DATA[2].CPU1_Jump;
            } else {
                s_Operation[2].STEP_INDEX = SCHEDULE_DATA[2].CPU1_JUMP.STEP_INDEX;
            }
            EndConditionState[2] = OPERATION_END_CONDITION_NONE_OP;

            //Modified Jump end condition (To be Updated)
            s_DATA[2][Bank[2]].DATA[DataCount[2]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_NONE_OP;

        }  //Pause 구간
        else if (CELL_SCAN_DATA[2].CPU2_Pause != SCHEDULE_DATA[2].CPU1_Pause) {
            switch (SCHEDULE_DATA[2].CPU1_PAUSE.COMMAND)
            {
                case OPERATION_PAUSE_STOP:
                    SaveCOMMAND[2] = COM_NONE;
                    s_DATA[2][Bank[2]].DATA[DataCount[2]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);
                    break;

                case OPERATION_PAUSE_PAUSE:
                    if (FlagPause[2] == CLEAR) {
                        SaveCOMMAND[2] = s_Operation[2].COMMAND;
                    }
                    s_Operation[2].COMMAND = COM_REST;
                    FlagPause[2] = SET;
                    EndConditionState[2] = OPERATION_END_CONDITION_NONE_OP;
                    s_DATA[2][Bank[2]].DATA[DataCount[2]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_COMPLETE:
                    SaveCOMMAND[2] = COM_NONE;
                    FlagPause[2] = CLEAR;

                    s_Operation[2].STEP_INDEX = INDEX_STEP_END;
                    EndConditionState[2] = OPERATION_END_CONDITION_NONE_OP;

                    CELL_SCAN_DATA[2].CPU2_Pause = SCHEDULE_DATA[2].CPU1_Pause;
                    s_Operation[2].VARIABLE_ALARM_UpDateFlag = CLEAR;  //DL 2019-07-15
                    s_DATA[2][Bank[2]].DATA[DataCount[2]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_UPDATE_END;  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_RESTART:
                    s_Operation[2].COMMAND = SaveCOMMAND[2];
                    SaveCOMMAND[2] = COM_NONE;
                    FlagPause[2] = CLEAR;
                    EndConditionState[2] = OPERATION_END_CONDITION_NONE;
                    CELL_SCAN_DATA[2].CPU2_Pause = SCHEDULE_DATA[2].CPU1_Pause;
                    s_DATA[2][Bank[2]].DATA[DataCount[2]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;
            }
        }

        if (s_Operation[2].VARIABLE_ALARM_Flag) {

            s_Operation[2].OPERATIN = OP_DEFAULT;
            s_Operation[2].STEP_INDEX = INDEX_STEP_END;
            EndConditionState[2] = OPERATION_END_CONDITION_NONE_OP;
            s_DATA[2][Bank[2]].DATA[DataCount[2]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_ALARM);  //DL 2019-07-11 (SW engineer request)
        }

        //Simply changed to the condition
        if (FlagPause[2] == CLEAR) {
            if (s_Operation[2].VARIABLE_StartFlag == SET) {
                UpdateFlagStepStart[2] = SET;

                if (UpdateFlagStepEnd[2] == SET) {
                    COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2, (Uint16) EndConditionState[2]);
                    EndConditionState[2] = OPERATION_END_CONDITION_UPDATE_END;
                    UpdateFlagStepEnd[2] = CLEAR;
                }
            } else {
                s_Operation[2].STEP_INDEX = OPERATION_END_CONDITION_NONE;
            }
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 2);

        COMMUNICATION_ALARMWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 2);

        s_DATA[2][Bank[2]].Header = HEADER_DATA;
        s_DATA[2][Bank[2]].DataCount = COMMUNICATION_INTEGER_LENGTH_DATA * 2;
        s_DATA[2][Bank[2]].ChannelIndex = (1 << (2 + CONTROL_CHANNEL_START_INDEX ));

        CELL_SCAN_DATA[2].CPU2_DATA = s_DATA[2][Bank[2]];

        CELL_SCAN_DATA[2].CPU2_DataBank = Bank[2];
        CELL_SCAN_DATA[2].CPU2_StepIndex = s_Operation[2].STEP_INDEX;

        CELL_SCAN_DATA[2].CPU2_Data = Bank[2];

        GSRAM_DATA_BANK_INDEX_CPU1[2]++;
        GSRAM_DATA_BANK_INDEX_CPU1[2] %= COMMUNICATION_TX_DEVICE_BANK;
        ui_UpdateDATACPU1[2]++;

        if (s_Operation[2].STEP_INDEX == INDEX_STEP_END) {
            OPERATION_Stop(s_Operation, 2);
            s_Operation[2].C_IRELAY = CONTROL_CurrentRELAY(2, CONTROL_OFF);
        }
    }

    //Moved to the main loop
    //OPERATION_Run(s_Operation, 2);

    s_Operation[2].VARIABLE_TimeSpanResult++;

    //Simply changed to the condition
    if (s_Operation[2].VARIABLE_StartFlag == SET) {
        if (FlagPause[2] == SET) {
            if (ui_UpdateSTEPCPU1[2] != ui_UpdateSTEPCPU2[2]) {
                s_Operation[2].VARIABLE_UpdateData++;
                ui_UpdateSTEPCPU1[2] = ui_UpdateSTEPCPU2[2];
            }
        } else {
            if (UpdateFlagStepStart[2] == SET) {
                if (ui_UpdateSTEPCPU1[2] != ui_UpdateSTEPCPU2[2]) {

                    s_Operation[2].VARIABLE_UpdateData++;
                    if (s_Operation[2].STEP_INDEX == SCHEDULE_DATA[2].CPU1_STEP[0].STEP_INDEX) {

                        COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2, 0);
                        UpdateFlagStepStart[2] = CLEAR;
                        UpdateFlagStepEnd[2] = SET;
                    }
                    ui_UpdateSTEPCPU1[2] = ui_UpdateSTEPCPU2[2];
                }
            }
        }
    }

#endif
}

__interrupt void DSP_Timer2_ISR(void)
{
    //For loop time measurement
    //GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;
#if 1

#if 1
    Uint16 i;

    //For OPERATION_VoltageControl function Timer
    for (i = 0; i < 4; i++) {
        if (s_Operation[i].s_VolCon.flag == SET) {
            if (s_Operation[i].s_VolCon.time) {
                s_Operation[i].s_VolCon.time--;
            } else {
                s_Operation[i].s_VolCon.time = CLEAR;
            }
        }
    }
#endif

    Bank[3] = (GSRAM_DATA_BANK_INDEX_CPU1[3] + 1) % COMMUNICATION_TX_DEVICE_BANK;
    DataCount[3] = (s_Operation[3].VARIABLE_TimeSpanResult % COMMUNICATION_TX_DATA_LENGTH);

    //Moved to the main loop
#if (SENSING_TYPE == SENSING_TIMER)
    OPERATION_Sensing(s_Operation, 3);
#elif (SENSING_TYPE == SENSING_SEPERATE)
    OPERATION_Sensing_Timer(s_Operation, 3);
#endif
    OPERATION_ModeCheck(s_Operation, 3);
    //OPERATION_Alram(s_Operation, 3);

    COMMUNICATION_COMMANDRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3);

    //Removed empty functions
    //COMMUNICATION_JUMPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3);
    //COMMUNICATION_PAUSERead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3);

    if (COMMUNICATION_STARTRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3)) {
        DataCount[3] = CLEAR;
    }

#if 0
    //For Timespan initial
    if (g_old_step_index[3] != s_Operation[3].STEP_INDEX) {
        g_old_step_index[3] = s_Operation[3].STEP_INDEX;
        s_Operation[3].VARIABLE_TimeSpanResult = 0;
    }
#endif

    VoltageAVG[3] += s_Operation[3].VARIABLE_VoltageResult;
    CurrentAVG[3] += s_Operation[3].VARIABLE_CurrentResult;
    WattAVG[3] += s_Operation[3].VARIABLE_PowerResult;
    AVGCounter[3]++;

    if (DataCount[3] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        s_Operation[3].VARIABLE_EndConditionVoltageValueAVG = VoltageAVG[3] / (float32) (AVGCounter[3]);
        s_Operation[3].VARIABLE_EndConditionCurrentValueAVG = CurrentAVG[3] / (float32) (AVGCounter[3]);
        s_Operation[3].VARIABLE_EndConditionWattValueAVG = WattAVG[3] / (float32) (AVGCounter[3]);

        if (UpdateFlagStepStart[3] == CLEAR && EndConditionState[3] == OPERATION_END_CONDITION_UPDATE_END) {
            EndConditionState[3] = OPERATION_EndConditionCheck(s_Operation, 3);
        } else {
            EndConditionState[3] = OPERATION_END_CONDITION_NONE_OP;
        }

        VoltageAVG[3] = 0.0;
        CurrentAVG[3] = 0.0;
        WattAVG[3] = 0.0;
        AVGCounter[3] = 0;
    }

    COMMUNICATION_DATAWrite(SCHEDULE_DATA, &(s_DATA[3][Bank[3]].DATA[DataCount[3]]), s_Operation, 3);

    if (DataCount[3] >= COMMUNICATION_MAX_LENGTH_TX_DATA - 1) {

        //Jump 구간
        if (CELL_SCAN_DATA[3].CPU2_Jump != SCHEDULE_DATA[3].CPU1_Jump && s_Operation[3].VARIABLE_StartFlag == SET)  //짬푸할때는 엔드 조건이 0이어야 합니다.
        {
            if (SCHEDULE_DATA[3].CPU1_STEP[0].STEP_INDEX == SCHEDULE_DATA[3].CPU1_JUMP.STEP_INDEX) {
                COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3, 0);
                //DL 2019-06-09 (JUMP ECHO)
                if ((s_Operation[3].CYCLE_INDEX == s_Operation[3].PATTERN_PRIOD) && (s_Operation[3].STEP_INDEX < s_Operation[3].STEP_INDEX_OLD)) {
                    s_Operation[3].VARIABLE_CYCLE_LOOP_COUNT++;
                }
                CELL_SCAN_DATA[3].CPU2_Jump = SCHEDULE_DATA[3].CPU1_Jump;
            } else {
                s_Operation[3].STEP_INDEX = SCHEDULE_DATA[3].CPU1_JUMP.STEP_INDEX;
            }
            EndConditionState[3] = OPERATION_END_CONDITION_NONE_OP;

            //Modified Jump end condition (To be Updated)
            s_DATA[3][Bank[3]].DATA[DataCount[3]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_NONE_OP;

        }  //Pause 구간
        else if (CELL_SCAN_DATA[3].CPU2_Pause != SCHEDULE_DATA[3].CPU1_Pause) {
            switch (SCHEDULE_DATA[3].CPU1_PAUSE.COMMAND)
            {
                case OPERATION_PAUSE_STOP:
                    SaveCOMMAND[3] = COM_NONE;
                    s_DATA[3][Bank[3]].DATA[DataCount[3]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);
                    break;

                case OPERATION_PAUSE_PAUSE:
                    if (FlagPause[3] == CLEAR) {
                        SaveCOMMAND[3] = s_Operation[3].COMMAND;
                    }
                    s_Operation[3].COMMAND = COM_REST;
                    FlagPause[3] = SET;
                    EndConditionState[3] = OPERATION_END_CONDITION_NONE_OP;
                    s_DATA[3][Bank[3]].DATA[DataCount[3]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_COMPLETE:
                    SaveCOMMAND[3] = COM_NONE;
                    FlagPause[3] = CLEAR;

                    s_Operation[3].STEP_INDEX = INDEX_STEP_END;
                    EndConditionState[3] = OPERATION_END_CONDITION_NONE_OP;

                    CELL_SCAN_DATA[3].CPU2_Pause = SCHEDULE_DATA[3].CPU1_Pause;
                    s_Operation[3].VARIABLE_ALARM_UpDateFlag = CLEAR;  //DL 2019-07-15
                    s_DATA[3][Bank[3]].DATA[DataCount[3]].END_CONDITION_STATE = (Uint16) OPERATION_END_CONDITION_UPDATE_END;  //DL 2019-07-14
                    break;

                case OPERATION_PAUSE_RESTART:
                    s_Operation[3].COMMAND = SaveCOMMAND[3];
                    SaveCOMMAND[3] = COM_NONE;
                    FlagPause[3] = CLEAR;
                    EndConditionState[3] = OPERATION_END_CONDITION_NONE;
                    CELL_SCAN_DATA[3].CPU2_Pause = SCHEDULE_DATA[3].CPU1_Pause;
                    s_DATA[3][Bank[3]].DATA[DataCount[3]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_PAUSE);  //DL 2019-07-14
                    break;
            }
        }

        if (s_Operation[3].VARIABLE_ALARM_Flag) {

            s_Operation[3].OPERATIN = OP_DEFAULT;
            s_Operation[3].STEP_INDEX = INDEX_STEP_END;
            EndConditionState[3] = OPERATION_END_CONDITION_NONE_OP;
            s_DATA[3][Bank[3]].DATA[DataCount[3]].END_CONDITION_STATE = (Uint16) (OPERATION_END_CONDITION_ALARM);  //DL 2019-07-11 (SW engineer request)
        }

        //Simply changed to the condition
        if (FlagPause[3] == CLEAR) {
            if (s_Operation[3].VARIABLE_StartFlag == SET) {
                UpdateFlagStepStart[3] = SET;

                if (UpdateFlagStepEnd[3] == SET) {
                    COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3, (Uint16) EndConditionState[3]);
                    EndConditionState[3] = OPERATION_END_CONDITION_UPDATE_END;
                    UpdateFlagStepEnd[3] = CLEAR;
                }
            } else {
                s_Operation[3].STEP_INDEX = OPERATION_END_CONDITION_NONE;
            }
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 3);

        COMMUNICATION_ALARMWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 3);

        s_DATA[3][Bank[3]].Header = HEADER_DATA;
        s_DATA[3][Bank[3]].DataCount = COMMUNICATION_INTEGER_LENGTH_DATA * 2;
        s_DATA[3][Bank[3]].ChannelIndex = (1 << (3 + CONTROL_CHANNEL_START_INDEX ));

        CELL_SCAN_DATA[3].CPU2_DATA = s_DATA[3][Bank[3]];

        CELL_SCAN_DATA[3].CPU2_DataBank = Bank[3];
        CELL_SCAN_DATA[3].CPU2_StepIndex = s_Operation[3].STEP_INDEX;

        CELL_SCAN_DATA[3].CPU2_Data = Bank[3];

        GSRAM_DATA_BANK_INDEX_CPU1[3]++;
        GSRAM_DATA_BANK_INDEX_CPU1[3] %= COMMUNICATION_TX_DEVICE_BANK;
        ui_UpdateDATACPU1[3]++;

        if (s_Operation[3].STEP_INDEX == INDEX_STEP_END) {
            OPERATION_Stop(s_Operation, 3);
            s_Operation[3].C_IRELAY = CONTROL_CurrentRELAY(3, CONTROL_OFF);
        }
    }

    //Moved to the main loop
    //OPERATION_Run(s_Operation, 3);

    s_Operation[3].VARIABLE_TimeSpanResult++;

    //Simply changed to the condition
    if (s_Operation[3].VARIABLE_StartFlag == SET) {
        if (FlagPause[3] == SET) {
            if (ui_UpdateSTEPCPU1[3] != ui_UpdateSTEPCPU2[3]) {
                s_Operation[3].VARIABLE_UpdateData++;
                ui_UpdateSTEPCPU1[3] = ui_UpdateSTEPCPU2[3];
            }
        } else {
            if (UpdateFlagStepStart[3] == SET) {
                if (ui_UpdateSTEPCPU1[3] != ui_UpdateSTEPCPU2[3]) {

                    s_Operation[3].VARIABLE_UpdateData++;
                    if (s_Operation[3].STEP_INDEX == SCHEDULE_DATA[3].CPU1_STEP[0].STEP_INDEX) {

                        COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3, 0);
                        UpdateFlagStepStart[3] = CLEAR;
                        UpdateFlagStepEnd[3] = SET;
                    }
                    ui_UpdateSTEPCPU1[3] = ui_UpdateSTEPCPU2[3];
                }
            }
        }
    }

#endif
    //For loop time measurement
    //GpioDataRegs.GPDSET.bit.GPIO99 = 1;
}

//
// End of file
//

