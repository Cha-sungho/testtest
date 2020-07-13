/*
 * PhoenixonControls_COMMUNICATION.c
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"
#include "../include/PhoenixonControls_FPU.h"

//
//DEVICE_CPU2
//
#ifdef DEVICE_CPU2

extern volatile S_SCHEDULE_DATA _SCHEDULE_DATA[];
extern volatile S_CELL_SCAN_DATA _CELL_SCAN_DATA[];
extern volatile Uint16 ui_UpdateflagCPU1;
extern volatile Uint16 ui_UpdateflagCPU2;

extern E_DSP_SCIA_FLAG e_SCIA_RxState;
extern E_DSP_SCIA_FLAG e_SCIA_TxState;



#pragma CODE_SECTION(COMMUNICATION_RS422A_TxPacket, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_RS422A_RxUnPacket, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");

Uint16 COMMUNICATION_RS422A_TxPacket(volatile Uint16 *TxBuff, volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, volatile Uint16 Channel, volatile Uint16 Bank)
{
	Uint16 StartFlag[DSP_SCIA_HEADER_LENGTH] ={0xAAAA, 0xAAAA, 0xAAAA, 0x0000, 0x0000};
	Uint16 Counter = 0;
	Uint16 CRC16 = 0;
	Uint16 Dumy[DSP_SCIA_TX_DUMY_LENGTH] = {0,};

	for(Counter = 0; Counter < DSP_SCIA_TX_DUMY_LENGTH; Counter++)Dumy[Counter] = DSP_SCIA_TX_DUMY;
	Counter = 0;
	StartFlag[DSP_SCIA_START_FLAG_LENGTH] =
			((Channel << 8) & 0xFF00) | (Channel & 0x00FF);

	StartFlag[DSP_SCIA_START_FLAG_LENGTH + 1] =
			((Channel << 8) & 0xFF00) | (Channel & 0x00FF);


	memcpy_fast_far(TxBuff, StartFlag, DSP_SCIA_HEADER_LENGTH);
	Counter += DSP_SCIA_HEADER_LENGTH;

	memcpy_fast_far(TxBuff + Counter, &(sp_GSRAM_CPU2[Bank]), DSP_SCIA_TX_DATA_LENGTH);
	Counter += DSP_SCIA_TX_DATA_LENGTH;

	memcpy_fast_far(TxBuff + Counter, Dumy, DSP_SCIA_TX_DUMY_LENGTH);
	Counter += DSP_SCIA_TX_DUMY_LENGTH;

	//Bank = //링버퍼로 뱅크 카운트 조절.
	CRC16 = CRC16_Conversion(TxBuff, DSP_SCIA_TX_MAX_DATA_LENGTH - DSP_SCIA_CRC16_LENGTH);

	memcpy_fast_far(TxBuff + Counter, &(CRC16), DSP_SCIA_CRC16_LENGTH);
	Counter += DSP_SCIA_CRC16_LENGTH;

	DSP_SCIA_Write(TxBuff, Counter);

	return(SUCCESS); //SUCCESS Or FAIL
}


Uint16 COMMUNICATION_RS422A_RxUnPacket(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile Uint16 *RxBuff, volatile Uint16 Channel)
{
	Uint32 Bank = 0;
	memcpy_fast_far(&(sp_GSRAM_CPU1[Bank]), RxBuff, sizeof(S_SCHEDULE_DATA));
	return(SUCCESS); //SUCCESS Or FAIL
}
#endif //End #ifdef DEVICE_CPU2

//
//DEVICE_CPU1
//
#ifdef DEVICE_CPU1
extern volatile S_SCHEDULE_DATA _SCHEDULE_DATA[CONTROL_CHANNEL_PER_BOARD];
extern volatile S_CELL_SCAN_DATA _CELL_SCAN_DATA[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 ui_UpdateflagCPU1[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 ui_UpdateflagCPU2[CONTROL_CHANNEL_PER_BOARD];
extern volatile Uint32 SYNC_SIGNAL_CPU1;
extern volatile Uint32 SYNC_SIGNAL_CPU2;

volatile S_COMMUNICATION_TX_DATA s_DATA[CONTROL_CHANNEL_PER_BOARD][COMMUNICATION_TX_DEVICE_BANK];
volatile S_COMMUNICATION_RX_JUMP s_JUMP;
volatile S_COMMUNICATION_RX_COMMAD s_COMMAND;
volatile S_COMMUNICATION_RX_START s_START;
volatile S_COMMUNICATION_RX_PAUSE s_PAUSE;

volatile S_COMMUNICATION_TX_END   s_SENSING_END;
volatile S_COMMUNICATION_TX_ALARM s_SENSING_ALARM;

#pragma CODE_SECTION(COMMUNICATION_STEPRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_JUMPRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_COMMANDRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_STARTRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_PAUSERead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_DATAWrite, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_ENDWrite, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_ALARMWrite, ".TI.ramfunc");

Uint16 COMMUNICATION_STEPRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel, Uint16 StepIndexStage)
{
    Uint32 TempUint32 = 0;

    if (StepIndexStage != OPERATION_END_CONDITION_NONE_OP && StepIndexStage != OPERATION_END_CONDITION_UPDATE_END) {
        sp_SCHEDULE_DATA[Channel].CPU1_StepHeader;
        sp_SCHEDULE_DATA[Channel].CPU1_StepDataCount;  //스탭 카운트 갯수..
        sp_SCHEDULE_DATA[Channel].CPU1_StepChannelIndex;
        sp_SCHEDULE_DATA[Channel].CPU1_StepIndex;

        sp_OP[Channel].COMMAND                  = (E_OPERATION_COM_SELECT) (sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].COMMAND);
        sp_OP[Channel].MODE                     = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].MODE;
        sp_OP[Channel].OPERATIN                 = (E_OPERATION_OP_SELECT) (sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].OPERATIN);
        sp_OP[Channel].SAMPLING_TIME            = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].SAMPLING_TIME;
        sp_OP[Channel].SEND_TIME                = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].SEND_TIME;

        sp_OP[Channel].STEP_INDEX_OLD             = sp_OP[Channel].STEP_INDEX;

        sp_OP[Channel].STEP_INDEX               = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].STEP_INDEX;
        sp_OP[Channel].CYCLE_LOOP_COUNT         = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].CYCLE_LOOP_COUNT;
        sp_OP[Channel].GOTO_INDEX               = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].GOTO_INDEX;
        sp_OP[Channel].RETURN_INDEX             = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].RETURN_INDEX;

        TempUint32                              = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].VOLTAGE_REFx16 << 16) & (Uint32) 0xFFFF0000);
        TempUint32                              |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].VOLTAGE_REFx00 & (Uint32) 0x0000FFFF);
        sp_OP[Channel].VARIABLE_VoltageRef      = (float) (TempUint32) * (float) (0.001);

        sp_OP[Channel].END_VOLTAGE_INDEX        = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_VOLTAGE_INDEX;

        TempUint32                              = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_VOLTAGE_CONDITIONx16 << 16) & (Uint32) 0xFFFF0000);
        TempUint32                              |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_VOLTAGE_CONDITIONx00 & (Uint32) 0x0000FFFF);
        sp_OP[Channel].END_VOLTAGE_CONDITION    = (float) (TempUint32) * (float) (0.001);

        TempUint32                              = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].CURRENT_REF_16 << 16) & (Uint32) 0xFFFF0000);
        TempUint32                              |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].CURRENT_REFx00 & (Uint32) 0x0000FFFF);
        sp_OP[Channel].VARIABLE_CurrentRef      = (float) (TempUint32) * (float) (0.001);

        sp_OP[Channel].END_TIME_CONDITION       = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_TIME_CONDITIONx16 << 16) & (Uint32) 0xFFFF0000);
        sp_OP[Channel].END_TIME_CONDITION       |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_TIME_CONDITIONx00 & (Uint32) 0x0000FFFF);

		//DL 2019-07-09
        //Difference cycle
        sp_OP[Channel].CYCLE_INDEX = sp_OP[Channel].PATTERN_PRIOD;
        sp_OP[Channel].PATTERN_PRIOD = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].PATTERN_PRIOD;
        if (sp_OP[Channel].CYCLE_INDEX != sp_OP[Channel].PATTERN_PRIOD) {
            sp_OP[Channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;
        }
        sp_OP[Channel].END_TIME_INDEX           = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_TIME_INDEX;

        TempUint32                              = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_CURRENT_CONDITIONx16 << 16) & (Uint32) 0xFFFF0000);
        TempUint32                              |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_CURRENT_CONDITIONx00 & 0x0000FFFF);
        sp_OP[Channel].END_CURRENT_CONDITION    = (float) (TempUint32) * (float) (0.001);

        sp_OP[Channel].END_CURRENT_INDEX        = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_CURRENT_INDEX;

        TempUint32                              = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_CAPACITY_CONDITIONx16 << 16) & (Uint32) 0xFFFF0000);
        TempUint32                              |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_CAPACITY_CONDITIONx00 & (Uint32) 0x0000FFFF);
        sp_OP[Channel].END_CAPACITY_CONDITION   = (float) (TempUint32) * (float) (0.001);

        sp_OP[Channel].END_CAPACITY_INDEX       = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_CAPACITY_INDEX;

        TempUint32                              = (((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_WATT_CONDITIONx16 << 16) & (Uint32) 0xFFFF0000);
        TempUint32                              |= ((Uint32) sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_WATT_CONDITIONx00 & (Uint32) 0x0000FFFF);
        //sp_OP[Channel].END_WATT_CONDITION       = (float) (TempUint32);
        //kW -> W
        sp_OP[Channel].END_WATT_CONDITION       = (float) (TempUint32) * (float) (0.001);

        sp_OP[Channel].END_WATT_INDEX           = sp_SCHEDULE_DATA[Channel].CPU1_STEP[StepIndexStage].END_WATT_INDEX;

        sp_CELL_SCAN_DATA[Channel].CPU2_Step    = sp_SCHEDULE_DATA[Channel].CPU1_Step;
    }
    return (SUCCESS);  //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_JUMPRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel )
{
//	if(sp_CELL_SCAN_DATA[0].CPU2_Jump != sp_SCHEDULE_DATA[0].CPU1_Jump)
//	{
//		sp_CELL_SCAN_DATA[0].CPU2_StepIndex = sp_SCHEDULE_DATA[0].CPU1_JUMP.STEP_INDEX; //짭푸할 인댁스를 불러와서 시작해주세요.
//											//노멀 값은 0이오 0 주소에 노멀 값을 넣어 주시오.
//		sp_CELL_SCAN_DATA[0].CPU2_Jump = sp_SCHEDULE_DATA[0].CPU1_Jump;
//	}
	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_COMMANDRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel)
{
	if(sp_CELL_SCAN_DATA[Channel].CPU2_Command != sp_SCHEDULE_DATA[Channel].CPU1_Command)
	{
		sp_CELL_SCAN_DATA[Channel].CPU2_StepIndex = 0; //노멀값을 넣고 커맨드를 실행하시오. 커맨드는 별거 없을 것 같다. 릴레이 온오프 정도. 불러와서 시작해주세요.
		sp_OP[Channel].DIRECT_COMMAND = (E_OPERATION_COM_SELECT)(sp_SCHEDULE_DATA[Channel].CPU1_COMMAND.COMMAD);
		//		(*spSTEP).COMMAND = (*sp_SCHEDULE_DATA).CPU1_COMMAND.COMMAD;	//노멀 값은 0이오 0 주소에 노멀 값을 넣어 주시오.
		sp_CELL_SCAN_DATA[Channel].CPU2_Command = sp_SCHEDULE_DATA[Channel].CPU1_Command;
	}

	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_STARTRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel)
{
	if(sp_CELL_SCAN_DATA[Channel].CPU2_Start != sp_SCHEDULE_DATA[Channel].CPU1_Start)
	{
		if (OPERATION_CellCheck(s_Operation, Channel) == FAIL)
        {
            sp_OP[Channel].C_IRELAY = CONTROL_CurrentRELAY(Channel, CONTROL_OFF);
            sp_OP[Channel].VARIABLE_StartFlag = CLEAR;
            sp_OP[Channel].COMMAND = COM_NONE;
            sp_CELL_SCAN_DATA[Channel].CPU2_StepIndex = 0;
        }
        else
        {
            sp_OP[Channel].C_IRELAY = CONTROL_CurrentRELAY(Channel, CONTROL_ON);
            sp_OP[Channel].VARIABLE_StartFlag = SET;
            sp_OP[Channel].VARIABLE_TimeSpanResult = CLEAR;
            sp_OP[Channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;
        }

		sp_CELL_SCAN_DATA[Channel].CPU2_Start = sp_SCHEDULE_DATA[Channel].CPU1_Start;//노멀 값은 0이오 0 주소에 노멀 값을 넣어 주시오.

		return(SUCCESS); //SUCCESS Or FAIL
	}																//DATA 및 카운터 등등 초기화 해주길 바라오.


	return(FAIL); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_PAUSERead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel)
{
	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_DATAWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_COMMUNICATION_TX *spDATA, S_OPERATION_FILE *sp_OP, volatile Uint16 Channel )
{
	Uint32 Temp32 = 0;
	Uint64 Temp64 = 0;

	(*spDATA).SAMPLING_TIME = sp_OP[Channel].SAMPLING_TIME;
	(*spDATA).STEP_INDEX = sp_OP[Channel].STEP_INDEX;
	(*spDATA).COMMAND = sp_OP[Channel].COMMAND;
	(*spDATA).END_CONDITION_STATE= (Uint32)(sp_OP[Channel].VARIABLE_EndConditionResult);
	(*spDATA).OPERATIN = sp_OP[Channel].VARIABLE_OPERATIN;
	(*spDATA).MODE = sp_OP[Channel].MODE;

#if 1
	Temp32 = (Uint32)(ABS_F(sp_OP[Channel].VARIABLE_VoltageResult) * 10000.0);
	//Temp32 = (Uint32)round(ABS_F((*sp_OP).VARIABLE_VoltageResult) * 1000.0);    //Added round() on 2019-07-09
#else
	Temp32 = (Uint32)(ABS_F(sp_OP[Channel].VARIABLE_VoltageResult) * 1000000.0);
#endif
	(*spDATA).VOLTAGEx16 = (Uint16)((Temp32 >> 16) & 0x0000FFFF);
	(*spDATA).VOLTAGEx00 = (Uint16)((Temp32      ) & 0x0000FFFF);

	Temp32 = (Uint32)(ABS_F(sp_OP[Channel].VARIABLE_CurrentResult) * 10000.0);
	//Temp32 = (Uint32)round(ABS_F((*sp_OP).VARIABLE_CurrentResult) * 1000.0);    //Added round() on 2019-07-09
	(*spDATA).CURRENTx16 = (Uint16)((Temp32 >> 16) & 0x0000FFFF);
	(*spDATA).CURRENTx00 = (Uint16)((Temp32      ) & 0x0000FFFF);

	Temp64 = (Uint64)(sp_OP[Channel].VARIABLE_TimeSpanResult);
	(*spDATA).TIME_SPANx64= (Uint16)((Temp64 >> 48) & 0x000000000000FFFF);
	(*spDATA).TIME_SPANx32= (Uint16)((Temp64 >> 32) & 0x000000000000FFFF);
	(*spDATA).TIME_SPANx16= (Uint16)((Temp64 >> 16) & 0x000000000000FFFF);
	(*spDATA).TIME_SPANx00= (Uint16)((Temp64      ) & 0x000000000000FFFF);

	Temp32 = (Uint32)(sp_OP[Channel].VARIABLE_EndConditionValue);
	(*spDATA).END_CONDITION_VALUEx16= (Uint16)((Temp32 >> 16) & 0x0000FFFF);
	(*spDATA).END_CONDITION_VALUEx00= (Uint16)((Temp32      ) & 0x0000FFFF);

	(*spDATA).LOOP_COUNT = sp_OP[Channel].VARIABLE_CYCLE_LOOP_COUNT;

	Temp32 = (Uint32)(sp_OP[Channel].VARIABLE_CapacityResult * 10000.0); // 소숫점 4자리
    (*spDATA).CAPACITYx16= (Uint16)((Temp32 >> 16) & 0x0000FFFF);
    (*spDATA).CAPACITYx00= (Uint16)((Temp32      ) & 0x0000FFFF);

	sp_OP[Channel].VARIABLE_EndConditionResult = CLEAR;
	sp_OP[Channel].VARIABLE_EndConditionValue = CLEAR;
	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_ENDWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, S_OPERATION_FILE* sp_OP, volatile Uint16 Channel )
{
	if(sp_OP[Channel].STEP_INDEX == INDEX_STEP_END)

	{
		//OPERATION_Stop(sp_OP, Channel);
		sp_OP[Channel].COMMAND = COM_NONE;
        sp_OP[Channel].VARIABLE_COMMAND_OLD = COM_NONE;
        sp_OP[Channel].OPERATIN = OP_DEFAULT;
		sp_OP[Channel].VARIABLE_StartFlag = CLEAR;
		//sp_OP[Channel].C_IRELAY = CONTROL_CurrentRELAY(Channel, CONTROL_OFF);
		sp_CELL_SCAN_DATA[Channel].CPU2_END.Header = HEADER_END;
		sp_CELL_SCAN_DATA[Channel].CPU2_END.ChannelIndex = (1 << (Channel+CONTROL_CHANNEL_START_INDEX));
		sp_CELL_SCAN_DATA[Channel].CPU2_END.DataCount = COMMUNICATION_INTEGER_LENGTH_END * 2;
		sp_CELL_SCAN_DATA[Channel].CPU2_StepIndex = 0;
		sp_CELL_SCAN_DATA[Channel].CPU2_End++;
	    sp_OP[Channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;
	}

	sp_OP[Channel].VARIABLE_EndConditionResult = CLEAR;
	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_ALARMWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, S_OPERATION_FILE* sp_OP, volatile Uint16 Channel)
{
	if(sp_OP[Channel].VARIABLE_ALARM_Flag == SET)
	{
        OPERATION_Stop(sp_OP, Channel);
        sp_OP[Channel].COMMAND = COM_NONE;
        sp_OP[Channel].VARIABLE_COMMAND_OLD = COM_NONE;
        sp_OP[Channel].VARIABLE_StartFlag = CLEAR;
        sp_OP[Channel].C_IRELAY = CONTROL_CurrentRELAY(Channel, CONTROL_OFF);

		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.Header = HEADER_ALARM;
		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.ChannelIndex = (1 << (Channel+CONTROL_CHANNEL_START_INDEX));
		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.DataCount = COMMUNICATION_INTEGER_LENGTH_ALARM * 2;
		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.WARING.all 	=  sp_OP[Channel].VARIABLE_ALARM_WARING.all;
		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.GRID.all 	=  sp_OP[Channel].VARIABLE_ALARM_GRID.all;
		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.DC.all 		=  sp_OP[Channel].VARIABLE_ALARM_DC.all;
		sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.FAULT.all 	=  sp_OP[Channel].VARIABLE_ALARM_FAULT.all;

		sp_CELL_SCAN_DATA[Channel].CPU2_StepIndex = 0;
	}

	if(sp_OP[Channel].VARIABLE_ALARM_UpDateFlag == CLEAR && sp_OP[Channel].VARIABLE_ALARM_Flag == SET)
        sp_OP[Channel].VARIABLE_ALARM_RequestFlag = ++(sp_CELL_SCAN_DATA[Channel].CPU2_Alarm);

    if(sp_CELL_SCAN_DATA[Channel].CPU2_Alarm != sp_OP[Channel].VARIABLE_ALARM_RequestFlag)
	{
	    sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.Header = HEADER_ALARM;
	    sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.ChannelIndex = (1 << (Channel+CONTROL_CHANNEL_START_INDEX));
        sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.DataCount = COMMUNICATION_INTEGER_LENGTH_ALARM * 2;
        sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.WARING.all   =  sp_OP[Channel].VARIABLE_ALARM_WARING.all;
        sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.GRID.all     =  sp_OP[Channel].VARIABLE_ALARM_GRID.all;
        sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.DC.all       =  sp_OP[Channel].VARIABLE_ALARM_DC.all;
        sp_CELL_SCAN_DATA[Channel].CPU2_ALARM.FAULT.all    =  sp_OP[Channel].VARIABLE_ALARM_FAULT.all;
        sp_CELL_SCAN_DATA[Channel].CPU2_Alarm = sp_OP[Channel].VARIABLE_ALARM_RequestFlag;
        sp_OP[Channel].VARIABLE_CYCLE_LOOP_COUNT = CLEAR;
	}

	sp_OP[Channel].VARIABLE_ALARM_UpDateFlag = sp_OP[Channel].VARIABLE_ALARM_Flag;

	return(SUCCESS); //SUCCESS Or FAIL
}
#endif //End #ifdef DEVICE_CPU2

//
// End of file
//


