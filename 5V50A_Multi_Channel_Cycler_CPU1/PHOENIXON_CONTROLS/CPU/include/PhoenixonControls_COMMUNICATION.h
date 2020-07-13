/*
 * PhoenixonControls_COMMUNICATION.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */


#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_COMMUNICATION_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_COMMUNICATION_H_

																								  //Gateway 		        192,168,  0,  1
//Ethernet																						  //Subnet Mask		        255,255,255,0
#define COMMUNICATION_ETHERNET_CHANNEL_NUMBER		(1)											  //Setting Value Max       1 ~ 244
#define COMMUNICATION_ETHERNET_MAC					(1 + COMMUNICATION_ETHERNET_CHANNEL_NUMBER)   //MAC  Setting Value Max  1 ~ 244
#define COMMUNICATION_ETHERNET_IP					(1 + COMMUNICATION_ETHERNET_CHANNEL_NUMBER)   //IP   Setting Value Max  192,168,  0,  2 ~ 192,168,  0,255
#define COMMUNICATION_ETHERNET_PORT					(12345)										  //Port Setting Value Max  0 ~ 65535

#define COMMUNICATION_RX_STEP_LENGTH				(65536)
#define COMMUNICATION_TX_DATA_LENGTH 				(100)

//Device
#define COMMUNICATION_CELL_CYCLER_CHANNEL           (1)         //Cycler_Channel Number 1 ~ 16 Channel

#define COMMUNICATION_CHANNEL                       (4)         //MAX 4 Channel
#define COMMUNICATION_CHANNEL_NUMBER                (Uint16)((COMMUNICATION_CELL_CYCLER_CHANNEL - 1) % 4)           //device id 0~3 total 4 Channel


#define CONTROL_BOARD                               (1) //Cycler_Board Number 1 ~ 10 : 40CH ( 4 channel per Board )


#define CONTROL_CHANNEL_PER_BOARD                   (4) //4 channel per Board
#define CONTROL_BOARD_PER_INTERFACE                 (1) //1 board per interface board
#define CONTROL_CHANNEL_PER_INTERFACE               (CONTROL_CHANNEL_PER_BOARD * CONTROL_BOARD_PER_INTERFACE)
#define CONTROL_CHANNEL_START_INDEX                 (Uint16)(((CONTROL_CHANNEL_PER_BOARD * (CONTROL_BOARD - 1))) % CONTROL_CHANNEL_PER_INTERFACE)
#define CONTROL_CHANNEL_END_INDEX                   (Uint16)(CONTROL_CHANNEL_START_INDEX + 3)

#define COMMUNICATION_TX_DEVICE_LENGTH				(100)
#define COMMUNICATION_TX_DEVICE_BANK				(4)

#define COMMUNICATION_MAX_LENGTH_TX_DATA			(100)
#define COMMUNICATION_INTEGER_LENGTH_HEADER			(sizeof(S_COMMUNICATION_HEADER_FILE))
#define COMMUNICATION_INTEGER_LENGTH_STEP_PACKET 	(8)//(1) + (7)

#define COMMUNICATION_INTEGER_LENGTH_RX				(sizeof(S_COMMUNICATION_RX))
#define COMMUNICATION_INTEGER_LENGTH_TX				(sizeof(S_COMMUNICATION_TX))


#define COMMUNICATION_INTEGER_LENGTH_STEP			(COMMUNICATION_INTEGER_LENGTH_RX * COMMUNICATION_INTEGER_LENGTH_STEP_PACKET)
#define COMMUNICATION_INTEGER_LENGTH_DATA			(sizeof(S_COMMUNICATION_TX_DATA))
#define COMMUNICATION_INTEGER_LENGTH_JUMP			(sizeof(S_COMMUNICATION_RX_JUMP))
#define COMMUNICATION_INTEGER_LENGTH_COMMAND		(sizeof(S_COMMUNICATION_RX_COMMAD))
#define COMMUNICATION_INTEGER_LENGTH_START			(sizeof(S_COMMUNICATION_RX_START))
#define COMMUNICATION_INTEGER_LENGTH_END			(sizeof(S_COMMUNICATION_TX_END))
#define COMMUNICATION_INTEGER_LENGTH_PAUSE			(sizeof(S_COMMUNICATION_RX_PAUSE))
#define COMMUNICATION_INTEGER_LENGTH_ALARM			(sizeof(S_COMMUNICATION_TX_ALARM))

#define COMMUNICATION_RS422_MAX_LENGTH_RX_DATA		DSP_SCIA_MAX_RX_BUFFER
#define COMMUNICATION_RS422_MAX_LENGTH_TX_DATA		DSP_SCIA_MAX_TX_BUFFER

extern S_OPERATION_FILE s_Operation[4];

//
//*Header
//
typedef enum
{
	HEADER_DATA    = 0x4044,	//ASCII   @D = 0x4044
	HEADER_JUMP    = 0x404A,	//Code    @J = 0x404A
	HEADER_COMMAND = 0x4043,	//		  @C = 0x4043
	HEADER_START   = 0x4053,	//        @S = 0x4053
	HEADER_END     = 0x4045,	//        @E = 0x4045
	HEADER_PAUSE   = 0x4050,	//		  @P = 0x4050
	HEADER_ALARM   = 0x4041,	//		  @A = 0x4041
	HEADER_INIT
}E_COMMUNICATION_HEADER;

//
//*Channel Index
//
typedef enum
{
	CHANNEL_NONE 			= 0x0,
	CHANNEL_00    			= 0x1,
	CHANNEL_01    			= 0x2,
	CHANNEL_02	  			= 0x4,
	CHANNEL_03    			= 0x8,
}E_COMMUNICATION_CHANNEL;

//
//*I/F Command
//
typedef enum
{
	COMMAND_NONE				= 0x0,
	COMMAND_REST				= 0x81,
	COMMAND_CHARGE				= 0x82,
	COMMAND_DISCHARGE			= 0x83,
	COMMAND_ALRAM_CLEAR			= 0x84,
	COMMAND_INPUT_RELAY_OFF		= 0x85,
	COMMAND_INPUT_RELAY_ON		= 0x86,
	COMMAND_OUTPUT_RELAY_OFF	= 0x87,
	COMMAND_OUTPUT_RELAY_ON		= 0x88,
	COMMAND_ALL_RELAY_OFF		= 0x89,
	COMMAND_ALRAM_REQUEST		= 0x90,
	COMMAND_MODE_PARALLEL		= 0x91,
	COMMAND_MODE_NORMAL			= 0x92,

}E_COMMUNICATION_COMMAND;

//
//*Step Mode
//
typedef enum
{
	MODE_NONE					= 0x0,
	MODE_NORMAL					= 0x1,
	MODE_PATTERN				= 0x2,
	MODE_PARALLEL_NORMAL		= 0x3,
	MODE_PARALLEL_PATTERN		= 0x4,
}E_COMMUNICATION_MODE;

//
//*Operation Mode
//
typedef enum
{
	OPERATION_DEFAULT			= 0x0,
	OPERATION_CHARGE_CC			= 0x1,
	OPERATION_CHARGE_CV			= 0x2,
	OPERATION_CHARGE_CC_CP		= 0x3,
	OPERATION_CHARGE_CV_CP		= 0x4,
	OPERATION_DISCHARGE_CC		= 0x5,
	OPERATION_DISCHARGE_CV		= 0x6,
	OPERATION_DISCHARGE_CC_CP	= 0x7,
	OPERATION_DISCHARGE_CV_CP	= 0x8,
	OPERATION_CHARGE_CR         = 0x9,
	OPERATION_DISCHARGE_CR      = 0xA,
}E_COMMUNICATION_OPERATION;

typedef enum
{
	OPERATION_SEND_DEFAULT			= 0x0,
	OPERATION_SEND_INPUT_ON			= 0x1,
	OPERATION_SEND_OUTPUT_ON		= 0x2,
	OPERATION_SEND_CHARGE_CC		= 0x3,
	OPERATION_SEND_CHARGE_CV		= 0x4,
	OPERATION_SEND_DISCHARGE_CC		= 0x5,
	OPERATION_SEND_DISCHARGE_CV		= 0x6,
	OPERATION_SEND_CHARGE_CC_CP		= 0x7,
	OPERATION_SEND_DISCHARGE_CC_CP	= 0x8,
	OPERATION_SEND_CHARGE_CV_CP		= 0x9,
	OPERATION_SEND_DISCHARGE_CV_CP	= 0xA,
	OPERATION_SEND_CHARGE_CR        = 0xB,
    OPERATION_SEND_DISCHARGE_CR     = 0xC,
}E_COMMUNICATION_OPERATION_SEND;



//
//*Row Data Collection Time Period
//
typedef enum
{
	SAMPLING_1MS				= 1, //Default
	SAMPLING_10MS				= 2,
	SAMPLING_100MS				= 3,
	SAMPLING_1000MS				= 4,

}E_COMMUNICATION_SAMPLING_TIME;

//
//*Step End Reason
//
typedef enum
{
	STATE_TIME					= 1,
	STATE_AMPERE				= 2,
	STATE_CAPACITY				= 3,
	STATE_WATT					= 4,
	STATE_VOLTAGE				= 5,
	STATE_INIT
}E_COMMUNICATION_END_STATE;

//
//*Pause Command
//
typedef enum
{
	PAUSE_STOP				= 0,
	PAUSE_PAUSE				= 1,
	PAUSE_COMPLETE			= 3, //Default
}E_COMMUNICATION_PAUSE_COMMAND;

typedef enum
{
	INDEX_END_TIME			= 1,
	INDEX_END_CURRENT		= 2,
	INDEX_END_CAPACITY		= 3,
	INDEX_END_WATT			= 4,
	INDEX_END_VOLTAGE		= 5,
	INDEX_GOTO				= 6,
	INDEX_RETURN			= 7,
	INDEX_STEP_END          = 0xFFFF,
}E_COMMUNICATION_STEP_INDEX;



//
// Big endian
// CYCLER -> IF( Data )
//

typedef struct
{
	Uint16 COMMAND;

	Uint16 MODE;
	Uint16 OPERATIN;

	Uint16 SAMPLING_TIME;
	Uint16 SEND_TIME;

	Uint16 STEP_INDEX;

	Uint16 CYCLE_LOOP_COUNT;

	Uint16 GOTO_INDEX;

	Uint16 RETURN_INDEX;

	Uint16 VOLTAGE_REFx16;
	Uint16 VOLTAGE_REFx00;

	Uint16 END_VOLTAGE_CONDITIONx16;
	Uint16 END_VOLTAGE_CONDITIONx00;

	Uint16 END_VOLTAGE_INDEX;

	Uint16 CURRENT_REF_16;
	Uint16 CURRENT_REFx00;

	Uint16 END_TIME_CONDITIONx16;
	Uint16 END_TIME_CONDITIONx00;

	Uint16 PATTERN_PRIOD;

	Uint16 END_TIME_INDEX;

	Uint16 END_CURRENT_CONDITIONx16;
	Uint16 END_CURRENT_CONDITIONx00;

	Uint16 END_CURRENT_INDEX;

	Uint16 END_CAPACITY_CONDITIONx16;
	Uint16 END_CAPACITY_CONDITIONx00;

	Uint16 END_CAPACITY_INDEX;

	Uint16 END_WATT_CONDITIONx16;
	Uint16 END_WATT_CONDITIONx00;

	Uint16 END_WATT_INDEX;

}S_COMMUNICATION_RX;
//
// Big endian
// CYCLER -> IF( Data )
//
typedef struct
{
	Uint16 SAMPLING_TIME;

	Uint16 STEP_INDEX;

	Uint16 COMMAND;
	Uint16 END_CONDITION_STATE;

	Uint16 OPERATIN;
	Uint16 MODE;

	Uint16 VOLTAGEx16;
	Uint16 VOLTAGEx00;

	Uint16 CURRENTx16;
	Uint16 CURRENTx00;

	Uint16 TIME_SPANx64;
	Uint16 TIME_SPANx32;
	Uint16 TIME_SPANx16;
	Uint16 TIME_SPANx00;

	Uint16 END_CONDITION_VALUEx16;
	Uint16 END_CONDITION_VALUEx00;

	Uint16 LOOP_COUNT;
	Uint16 CAPACITYx16;
    Uint16 CAPACITYx00;
}S_COMMUNICATION_TX;

//
// Big endian
// CYCLER -> IF( Jump )
//
typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

	Uint16 STEP_INDEX;
}S_COMMUNICATION_RX_JUMP;

//
// Big endian
// CYCLER -> IF( Command )
//
typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

	Uint16 COMMAD;
}S_COMMUNICATION_RX_COMMAD;

//
// Big endian
// CYCLER -> IF( Start )
//

typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

	Uint16 START_TIMEx64;
	Uint16 START_TIMEx32;
	Uint16 START_TIMEx16;
	Uint16 START_TIMEx00;
}S_COMMUNICATION_RX_START;

//
// Big endian
// IF -> CYCLER( End )
//
typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

}S_COMMUNICATION_TX_END;

//
// Big endian
// CYCLER -> IF( Pause )
//
typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

	Uint16 StepIndex;
	Uint16 COMMAND;
}S_COMMUNICATION_RX_PAUSE;

//
//Big endian
// IF -> CYCLER( Alarm )
//
typedef struct
{
	Uint16 EMOSwitchOn			:1; //Bit00
	Uint16 BatteryNone			:1; //Bit01
	Uint16 PowerFault			:1; //Bit02
	Uint16 CurrentFault			:1; //Bit03
	Uint16 VoltageFault			:1; //Bit04
	Uint16 CVFault				:1; //Bit05
	Uint16 Reserve06			:1; //Bit06
	Uint16 Reserve08			:1; //Bit08
	Uint16 Reserve07			:1; //Bit07
	Uint16 Reserve09			:1; //Bit09
	Uint16 Reserve10			:1; //Bit10
	Uint16 Reserve11			:1; //Bit11
	Uint16 Reserve12			:1; //Bit12
	Uint16 Reserve13			:1; //Bit13
	Uint16 Reserve14			:1; //Bit14
//	Uint16 Reserve15			:1; //Bit15
}S_COMMUNICATION_RX_ALARM_WARING_BIT;

typedef union
{
	Uint16 all;
	S_COMMUNICATION_RX_ALARM_WARING_BIT bit;
}U_COMMUNICATION_RX_ALARM_WARING;

typedef struct
{
	Uint16 VoltageRelayOn		:1; //Bit00
	Uint16 RelayOn				:1; //Bit01
	Uint16 OverHeatFault		:1; //Bit02
	Uint16 BatteryFault			:1; //Bit03
	Uint16 LineFault			:1; //Bit04
	Uint16 CCFault				:1; //Bit05
	Uint16 Reserve06			:1; //Bit06
	Uint16 Reserve07			:1; //Bit07
	Uint16 Reserve08			:1; //Bit08
	Uint16 Reserve09			:1; //Bit09
	Uint16 Reserve10			:1; //Bit10
	Uint16 Reserve11			:1; //Bit11
	Uint16 Reserve12			:1; //Bit12
	Uint16 Reserve13			:1; //Bit13
	Uint16 Reserve14			:1; //Bit14
//	Uint16 Reserve15			:1; //Bit15
}S_COMMUNICATION_RX_ALARM_GRID_BIT;

typedef union
{
	Uint16 all;
	S_COMMUNICATION_RX_ALARM_GRID_BIT bit;
}U_COMMUNICATION_RX_ALARM_GRID;

typedef struct
{
	Uint16 RelayOn				:1; //Bit00
	Uint16 CommunicationFault	:1; //Bit01
	Uint16 MaximumVoltage		:1; //Bit02
	Uint16 Reserve03			:1; //Bit03
	Uint16 MinumumVoltage		:1; //Bit04
	Uint16 MaximumTime			:1; //Bit05
	Uint16 Reserve06			:1; //Bit06
	Uint16 Reserve07			:1; //Bit07
	Uint16 Reserve08			:1; //Bit08
	Uint16 Reserve09			:1; //Bit09
	Uint16 Reserve10			:1; //Bit10
	Uint16 Reserve11			:1; //Bit11
	Uint16 Reserve12			:1; //Bit12
	Uint16 Reserve13			:1; //Bit13
	Uint16 Reserve14			:1; //Bit14
//	Uint16 Reserve15			:1; //Bit15
}S_COMMUNICATION_RX_ALARM_DC_BIT;

typedef union
{
	Uint16 all;
	S_COMMUNICATION_RX_ALARM_DC_BIT bit;
}U_COMMUNICATION_RX_ALARM_DC;

typedef struct
{
	Uint16 MaximumWatt			:1; //Bit00
	Uint16 MaximumWattHour		:1; //Bit01
	Uint16 MinumumCVAmpere		:1; //Bit02
	Uint16 Reserve03			:1; //Bit03
	Uint16 Reserve04			:1; //Bit04
	Uint16 Reserve05			:1; //Bit05
	Uint16 Reserve06			:1; //Bit06
	Uint16 Reserve07			:1; //Bit07
	Uint16 Reserve08			:1; //Bit08
	Uint16 Reserve09			:1; //Bit09
	Uint16 Reserve10			:1; //Bit10
	Uint16 Reserve11			:1; //Bit11
	Uint16 Reserve12			:1; //Bit12
	Uint16 Reserve13			:1; //Bit13
	Uint16 Reserve14			:1; //Bit14
//	Uint16 Reserve15			:1; //Bit15
}S_COMMUNICATION_RX_ALARM_FAULT_BIT;
typedef union
{
	Uint16 all;
	S_COMMUNICATION_RX_ALARM_FAULT_BIT bit;
}U_COMMUNICATION_RX_ALARM_FAULT;

typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;
	Uint16 StepIndx;

	U_COMMUNICATION_RX_ALARM_WARING WARING;
	U_COMMUNICATION_RX_ALARM_GRID 	GRID;
	U_COMMUNICATION_RX_ALARM_DC 	DC;
	U_COMMUNICATION_RX_ALARM_FAULT 	FAULT;
}S_COMMUNICATION_TX_ALARM;

typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

	S_COMMUNICATION_RX STEP[COMMUNICATION_RX_STEP_LENGTH];
}S_COMMUNICATION_RX_STEP;

typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;

	S_COMMUNICATION_TX DATA[COMMUNICATION_MAX_LENGTH_TX_DATA];
}S_COMMUNICATION_TX_DATA;

typedef struct
{
//	Uint16 ReadFlagStep;
	S_RINGBUFFER_FILE RING_STEP;
	S_COMMUNICATION_RX_STEP STEP;

//	Uint16 WriteFlagData;
	S_RINGBUFFER_FILE RING_DATA;
	S_COMMUNICATION_TX_DATA DATA[COMMUNICATION_TX_DATA_LENGTH];

}S_COMMUNICATION_DRAM;

typedef struct
{
	Uint16 CPU1_Step;
	Uint16 CPU1_Data;
	Uint16 CPU1_Jump;
	Uint16 CPU1_Command;
	Uint16 CPU1_Start;
	Uint16 CPU1_End;
	Uint16 CPU1_Pause;
	Uint16 CPU1_Alarm;

	Uint16 CPU1_DataBank;

	Uint16 CPU1_StepHeader;
	Uint16 CPU1_StepDataCount;
	Uint16 CPU1_StepChannelIndex;
	Uint16 CPU1_StepIndex;

	S_COMMUNICATION_RX CPU1_STEP[COMMUNICATION_INTEGER_LENGTH_STEP_PACKET];
	S_COMMUNICATION_RX_JUMP CPU1_JUMP;
	S_COMMUNICATION_RX_COMMAD CPU1_COMMAND;
	S_COMMUNICATION_RX_START CPU1_START;
	S_COMMUNICATION_RX_PAUSE CPU1_PAUSE;

}S_SCHEDULE_DATA;

typedef struct
{
	Uint16 CPU2_Step;
	Uint16 CPU2_Data;
	Uint16 CPU2_Jump;
	Uint16 CPU2_Command;
	Uint16 CPU2_Start;
	Uint16 CPU2_End;
	Uint16 CPU2_Pause;
	Uint16 CPU2_Alarm;

	Uint16 CPU2_StepIndex;
	Uint16 CPU2_DataBank;

	S_COMMUNICATION_TX_DATA CPU2_DATA;
	S_COMMUNICATION_TX_END CPU2_END;
	S_COMMUNICATION_TX_ALARM CPU2_ALARM;
}S_CELL_SCAN_DATA;

//typedef struct
//{
//	Uint16 CPU2_Step;
//	Uint16 CPU2_Data;
//	Uint16 CPU2_Jump;
//	Uint16 CPU2_Command;
//	Uint16 CPU2_Start;
//	Uint16 CPU2_End;
//	Uint16 CPU2_Pause;
//	Uint16 CPU2_Alarm;
//
//	Uint16 CPU2_StepIndex;
//	Uint16 CPU2_DataBank;
//
//	S_COMMUNICATION_TX_DATA CPU2_DATA[COMMUNICATION_TX_DEVICE_BANK];
//	S_COMMUNICATION_TX_END CPU2_END;
//	S_COMMUNICATION_TX_ALARM CPU2_ALARM;
//}S_CELL_SCAN_DATA;

typedef struct
{
	Uint16 Header;
	Uint16 DataCount;
	Uint16 ChannelIndex;
}S_COMMUNICATION_HEADER_FILE;

//
//INTERFACE_CPU1
//
#ifdef INTERFACE_CPU1

void COMMUNICATION_INIT(void);
Uint16 COMMUNICATION_Ethernet_STEPRead(volatile S_COMMUNICATION_DRAM **sp_IF, Uint16 *buf, Uint32 len);

Uint16 COMMUNICATION_Ethernet_JUMPRead(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1,
									   volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, Uint16 *buf, Uint32 len);

Uint16 COMMUNICATION_Ethernet_COMMANDRead(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1,
										  volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, Uint16 *buf, Uint32 len);

Uint16 COMMUNICATION_Ethernet_STARTRead(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1,
									    volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, Uint16 *buf, Uint32 len);

Uint16 COMMUNICATION_Ethernet_PAUSERead(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1,
										volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, Uint16 *buf, Uint32 len);

Uint32 COMMUNICATION_Ethernet_DATAWrite(volatile S_COMMUNICATION_DRAM **sp_IF, Uint16 *buf);

Uint32 COMMUNICATION_Ethernet_ENDWrite(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1,
									   volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, Uint16 *buf);

Uint32 COMMUNICATION_Ethernet_ALARMWrite(volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1,
										 volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, Uint16 *buf);

void COMMUNICATION_DRAM_STEPWrite(volatile S_COMMUNICATION_DRAM **sp_IF,
		volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2);

Uint16 COMMUNICATION_DRAM_DATARead(volatile S_COMMUNICATION_DRAM **sp_IF,
		volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2);

void COMMUNICATION_Write(volatile S_COMMUNICATION_DRAM **sp_IF,
		volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2);

void COMMUNICATION_Read(volatile S_COMMUNICATION_DRAM **sp_IF,
		volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2);

#endif //End #ifdef INTERFACE_CPU1

//
//INTERFACE_CPU2
//
#ifdef INTERFACE_CPU2
Uint16 COMMUNICATION_RS422A_TxRunSync(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile Uint16 Channel);
Uint16 COMMUNICATION_RS422A_TxDataCall(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile Uint16 Channel);
Uint16 COMMUNICATION_RS422A_TxStepSend(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1, volatile Uint16 Channel);
Uint16 COMMUNICATION_RS422A_RxUnPacket(volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, volatile Uint16 *RxBuff, volatile Uint16 Channel);
Uint16 COMMUNICATION_WritePass(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1);
Uint16 COMMUNICATION_ReadPass(volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, volatile Uint16 *RxBuff);

#endif //End #ifdef INTERFACE_CPU2

//
//DEVICE_CPU2
//
#ifdef DEVICE_CPU2
Uint16 COMMUNICATION_RS422A_TxPacket(volatile Uint16 *TxBuff, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel);
Uint16 COMMUNICATION_RS422A_RxUnPacket(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile Uint16 *RxBuff, volatile Uint16 Channel);
Uint16 COMMUNICATION_WritePass(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_GSRAM_CPU1);
Uint16 COMMUNICATION_ReadPass(volatile S_CELL_SCAN_DATA *sp_GSRAM_CPU2, volatile Uint16 *RxBuff);
#endif //End #ifdef DEVICE_CPU2

//
//DEVICE_CPU1
//
#ifdef DEVICE_CPU1
Uint16 COMMUNICATION_STEPRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel, Uint16 StepIndexStage);
Uint16 COMMUNICATION_JUMPRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel );
Uint16 COMMUNICATION_COMMANDRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel );
Uint16 COMMUNICATION_STARTRead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel );
Uint16 COMMUNICATION_PAUSERead(S_OPERATION_FILE* sp_OP, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel );
Uint16 COMMUNICATION_DATAWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_COMMUNICATION_TX *spDATA, S_OPERATION_FILE *sp_OP, volatile Uint16 Channel);
Uint16 COMMUNICATION_ENDWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA,  S_OPERATION_FILE* sp_OP, volatile Uint16 Channel);
Uint16 COMMUNICATION_ALARMWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA,  S_OPERATION_FILE* sp_OP, volatile Uint16 Channel);
#endif //End #ifdef DEVICE_CPU2

#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_COMMUNICATION_H_ */
