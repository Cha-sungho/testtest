/*
 * PhoenixonControls_OPERATION.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_OPERATION_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_OPERATION_H_

#define OPERATION_LIMIT_MAX_VOLTAGE					(float)(5.0)
#define OPERATION_LIMIT_MIN_VOLTAGE					(float)(0.0)

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

typedef struct
{
	E_OPERATION_CHARGE_MODE_SELECT Mode;
	float Voltage;
	float Current;
}S_OPERATION_CHARGE_DATA;

typedef struct
{
	Uint16 ui_UpdateData; //ipc ���

	float32 f_Voltage;
	float32 f_Current;
	float32 f_Power;

	E_OPERATION_CELL_STATE e_Cell;
	E_OPERATION_MODE_SELECT e_Operation;

	S_CONTROL_VRELAY 	C_VRELAY;
	S_CONTROL_RELAY 	C_IRELAY;
	S_CONTROL_DIO 		C_DIORELAY;
	S_CONTROL_MUX C_OperationMUX;
	S_CONTROL_INTEGRATION_RESET_MUX C_IntResetMUX;
	S_CONTROL_INTEGRATION_SOLP_MUX C_IntSlopMUX;
	S_CONTROL_DAC C_IntSlopPMDAC;
	S_CONTROL_VDAC C_GateOffsetDAC;
	S_CONTROL_MUX C_GateOutputMUX;
	S_CONTROL_MUX C_GateOutputAMP;
	S_CONTROL_VDAC C_VRefDAC;
	S_CONTROL_DAC C_IRefDAC;
	S_CONTROL_DAC C_FINEIRefDAC;

	S_SENSING_ADC_RESULT_DATA s_ADCData;
	S_SENSING_ADC_RESULT_DATA s_OldADCData;

//	Uint16 ui_UpdateData; //ipc ���
//
//	float32 f_Voltage;
//	float32 f_Current;
//	float32 f_Power;
//	E_OPERATION_CELL_STATE e_Cell;
//	E_OPERATION_CHARGE_CHANNEL_SELECT e_Channel;
//	E_OPERATION_CHARGE_MODE_SELECT e_Mode;
//	E_OPERATION_MODE_SELECT e_Operation;
//
//	E_CONTROL_CHANNEL_SELECT e_MUXVRelayCH;
//	E_CONTROL_CHANNEL_SELECT e_MUXIRelayCH;
//	E_CONTROL_DIO_CHANNEL_SELECT e_MUXDIORelayCH;
//	E_CONTROL_CHANNEL_SELECT e_MUXOperationCH;
//	E_CONTROL_CHANNEL_SELECT e_MUXIntResetCH;
//	E_CONTROL_CHANNEL_SELECT e_MUXSlopCH;
//	E_CONTROL_CHANNEL_SELECT e_DACSlopPMCH;
//	E_CONTROL_CHANNEL_SELECT e_DACGateOffsetCH;
//
//	E_CONTROL_SLOP_SELECT e_IntSlop[3];
//	float32 f_IntSlopPM[3];
//	float32 f_VRefDAC;
//	float32 f_IRefDAC[3];
//	float32 f_FINEIRefDAC[3];
//	float32 f_GateOffset;
//	S_SENSING_ADC_RESULT_DATA s_ADCData;
}S_OPERATION_FILE;

//typedef struct
//{
//	S_OPERATION_DATA New;
//	S_OPERATION_DATA Old;
//}S_OPERATION_FILE;



S_OPERATION_FILE OPERATION_Init(void);
S_OPERATION_FILE OPERATION_STATE(void);
S_OPERATION_FILE OPERATION_Run(void);
S_OPERATION_FILE OPERATION_CellCheck(void);
S_OPERATION_FILE OPERATION_Start(void);
S_OPERATION_FILE OPERATION_Stop(void);
S_OPERATION_FILE OPERATION_Alram(void);
S_OPERATION_FILE OPERATION_Emergency(void);
S_OPERATION_FILE OPERATION_Reset(void);
S_OPERATION_FILE OPERATION_Charge(void);
S_OPERATION_FILE OPERATION_Charge(void);
S_OPERATION_FILE OPERATION_Discharge(void);
S_OPERATION_FILE OPERATION_CC(void);
S_OPERATION_FILE OPERATION_CV(void);
S_OPERATION_FILE OPERATION_CP(void);
S_OPERATION_FILE OPERATION_Nomal(void);
S_OPERATION_FILE OPERATION_Pattern(void);
S_OPERATION_FILE OPERATION_Parallel(void);
S_OPERATION_FILE OPERATION_Sensing(void);
S_OPERATION_FILE OPERATION_ChannelControl(void);
S_OPERATION_FILE OPERATION_IntegrationSlopControl(void);
S_OPERATION_FILE OPERATION_GateOffsetControl(void);
S_OPERATION_FILE OPEARTION_CurrentFineControl(void);
#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_OPERATION_H_ */