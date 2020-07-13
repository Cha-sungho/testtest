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
//INTERFACE_CPU1
//
#ifdef INTERFACE_CPU1

extern volatile S_SCHEDULE_DATA s_CPU_1_GSRAM[];
extern volatile S_CELL_SCAN_DATA s_CPU_2_GSRAM[];
volatile S_COMMUNICATION_DRAM *s_SDRAMp[COMMUNICATION_CHANNEL];
extern volatile Uint16 ui_UpdateflagCPU1;
extern volatile Uint16 ui_UpdateflagCPU2;

#pragma CODE_SECTION(COMMUNICATION_Ethernet_STEPRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_JUMPRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_COMMANDRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_STARTRead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_PAUSERead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_DATAWrite, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_ENDWrite, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Ethernet_ALARMWrite, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_DRAM_STEPWrite, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_DRAM_DATARead, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Write, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_Read, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");
__attribute__((far)) volatile S_COMMUNICATION_DRAM s_SDRAM[COMMUNICATION_CHANNEL];

void COMMUNICATION_INIT(void)
{
	Uint16 ui_i = 0;
	Uint32 ul_Temp = 0;

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{
		ul_Temp = (Uint32)&(s_SDRAM[ui_i]);
		ul_Temp |= EMIF_CS0_BASE_ADDRESS;
		s_SDRAMp[ui_i] = (S_COMMUNICATION_DRAM *)(ul_Temp); //�ܺ� ���� ������ ������ �޸� ��������
	}							  //�ȵǴ� ������ �־� Ȯ�� �غ� ��� �ּ� 0x80000000��
}								  //������ �־� ������ ���� �־� �־� �����.

Uint16 COMMUNICATION_Ethernet_STEPRead(volatile S_COMMUNICATION_DRAM **sp_IF, Uint16 *buf, Uint32 len)
{
	Uint16 ui_i = 0;
	volatile static Uint16 ui_Channel = 0;
	volatile static Uint16 ui_StartFlag = 0;
	volatile static Uint32 ul_DataCount = 0;
	volatile static Uint32 ul_SumCount	= 0;
	volatile static Uint32 PointerCount[COMMUNICATION_CHANNEL];

//	memcpy_fast_far(sp_IF[0], buf, len / 2);//sp_IF ���� ������ �Դϴ�.
// �޸� �׽�Ʈ ����Դϴ�. ���� ���޹޾Ƽ� �ִ� ���� ������ �Դϴ�.
//	memcpy_fast_far(&((*sp_IF[0]).STEP) + PointerCount[ui_i], buf, len / 2);

//	return 0; //�޸� ī�� ������ ������ ������.

	if(!ui_StartFlag)
	{	ui_StartFlag = HEADER_DATA;

		ul_SumCount  = ((buf[1] * COMMUNICATION_INTEGER_LENGTH_RX) + COMMUNICATION_INTEGER_LENGTH_HEADER) * 2; //���� DATA �� ���� ���� ����
		ui_Channel 	 = buf[2]; //��� ���� ����
		//���� ǥ�� ���� �ʱ�ȭ
		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}
	ul_DataCount += len; //8BIT ������ Ethernet���� ���ŵ� BigEndian �ڷ���� ����

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����

		if(ui_Channel & (0x1 << ui_i))
		{
			memcpy_fast_far(&((*sp_IF)[ui_i].STEP.Header) + PointerCount[ui_i], buf, len / 2);
			PointerCount[ui_i] += (len / 2);
//			(*sp_IF)[ui_i].ReadFlagStep = SET;
		}
	}

	if(ul_DataCount >= ul_SumCount)//Data �� ������ ������ Counting�� ������ ���� ���ų�
	{							   //������(���� ����)�۾� �����ϰ� �ش� Flag �ʱ�ȭ ��.
		//���� ǥ�� ���� �ʱ�ȭ
		ui_StartFlag = CLEAR;

		ul_SumCount  = CLEAR;
		ui_Channel   = CLEAR;

		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}

	return(ui_StartFlag); //SUCCESS Or FAIL
						  //���ϰ����� ���� �� ���� �Ϸ�� 0�� ó�����϶��� HEADER_DATA�� ����.
}

Uint16 COMMUNICATION_Ethernet_JUMPRead(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, Uint16 *buf, Uint32 len)
{
	Uint16 ui_i = 0;
	volatile static Uint16 ui_Channel = 0;
	volatile static Uint16 ui_StartFlag = 0;
	volatile static Uint32 ul_DataCount = 0;
	volatile static Uint32 ul_SumCount	= 0;
	volatile static Uint32 PointerCount[COMMUNICATION_CHANNEL];

//	memcpy_fast_far(sp_IF[0], buf, len / 2);//sp_IF ���� ������ �Դϴ�.
// �޸� �׽�Ʈ ����Դϴ�. ���� ���޹޾Ƽ� �ִ� ���� ������ �Դϴ�.
//	memcpy_fast_far(&((*sp_IF[0]).STEP) + PointerCount[ui_i], buf, len / 2);

	if(!ui_StartFlag)
	{	ui_StartFlag = HEADER_JUMP;

		ul_SumCount  = buf[1]; //���� DATA �� ���� ���� ����
		ui_Channel 	 = buf[2]; //��� ���� ����
		//���� ǥ�� ���� �ʱ�ȭ
		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}
	ul_DataCount += len; //8BIT ������ Ethernet���� ���ŵ� BigEndian �ڷ���� ����

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i))// && (sp_SCHEDULE_DATA[ui_i]).CPU1_Jump == (sp_CELL_SCAN_DATA[ui_i]).CPU2_Jump)
		{
			memcpy_fast_far(&(sp_SCHEDULE_DATA[ui_i]).CPU1_JUMP.Header + PointerCount[ui_i], buf, len / 2);
			PointerCount[ui_i] += (len / 2);
			(sp_SCHEDULE_DATA[ui_i]).CPU1_Jump++;// = ~(sp_CELL_SCAN_DATA[ui_i]).CPU2_Jump;
		}
	}

	if(ul_DataCount >= ul_SumCount)//Data �� ������ ������ Counting�� ������ ���� ���ų�
	{							   //������(���� ����)�۾� �����ϰ� �ش� Flag �ʱ�ȭ ��.
		//���� ǥ�� ���� �ʱ�ȭ
		ui_StartFlag = CLEAR;

		ul_SumCount  = CLEAR;
		ui_Channel   = CLEAR;

		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}

	return(ui_StartFlag); //SUCCESS Or FAIL
						  //���ϰ����� ���� �� ���� �Ϸ�� 0�� ó�����϶��� HEADER_DATA�� ����.
}

Uint16 COMMUNICATION_Ethernet_COMMANDRead(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, Uint16 *buf, Uint32 len)
{
	Uint16 ui_i = 0;
	volatile static Uint16 ui_Channel = 0;
	volatile static Uint16 ui_StartFlag = 0;
	volatile static Uint32 ul_DataCount = 0;
	volatile static Uint32 ul_SumCount	= 0;
	volatile static Uint32 PointerCount[COMMUNICATION_CHANNEL];

//	memcpy_fast_far(sp_IF[0], buf, len / 2);//sp_IF ���� ������ �Դϴ�.
// �޸� �׽�Ʈ ����Դϴ�. ���� ���޹޾Ƽ� �ִ� ���� ������ �Դϴ�.
//	memcpy_fast_far(&((*sp_IF[0]).STEP) + PointerCount[ui_i], buf, len / 2);

	if(!ui_StartFlag)
	{	ui_StartFlag = HEADER_COMMAND;

		ul_SumCount  = buf[1]; //���� DATA �� ���� ���� ����
		ui_Channel 	 = buf[2]; //��� ���� ����
		//���� ǥ�� ���� �ʱ�ȭ
		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}
	ul_DataCount += len; //8BIT ������ Ethernet���� ���ŵ� BigEndian �ڷ���� ����

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i))// && (sp_SCHEDULE_DATA[ui_i]).CPU1_Command == (sp_CELL_SCAN_DATA[ui_i]).CPU2_Command)
		{
			memcpy_fast_far(&(sp_SCHEDULE_DATA[ui_i]).CPU1_COMMAND.Header + PointerCount[ui_i], buf, len / 2);
			PointerCount[ui_i] += (len / 2);
			(sp_SCHEDULE_DATA[ui_i]).CPU1_Command++;// = ~(sp_CELL_SCAN_DATA[ui_i]).CPU2_Command;
		}
	}

	if(ul_DataCount >= ul_SumCount)//Data �� ������ ������ Counting�� ������ ���� ���ų�
	{							   //������(���� ����)�۾� �����ϰ� �ش� Flag �ʱ�ȭ ��.
		//���� ǥ�� ���� �ʱ�ȭ
		ui_StartFlag = CLEAR;

		ul_SumCount  = CLEAR;
		ui_Channel   = CLEAR;

		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}

	return(ui_StartFlag); //SUCCESS Or FAIL
						  //���ϰ����� ���� �� ���� �Ϸ�� 0�� ó�����϶��� HEADER_DATA�� ����.
}

Uint16 COMMUNICATION_Ethernet_STARTRead(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, Uint16 *buf, Uint32 len)
{
	Uint16 ui_i = 0;
	volatile static Uint16 ui_Channel = 0;
	volatile static Uint16 ui_StartFlag = 0;
	volatile static Uint32 ul_DataCount = 0;
	volatile static Uint32 ul_SumCount	= 0;
	volatile static Uint32 PointerCount[COMMUNICATION_CHANNEL];

//	memcpy_fast_far(sp_IF[0], buf, len / 2);//sp_IF ���� ������ �Դϴ�.
// �޸� �׽�Ʈ ����Դϴ�. ���� ���޹޾Ƽ� �ִ� ���� ������ �Դϴ�.
//	memcpy_fast_far(&((*sp_IF[0]).STEP) + PointerCount[ui_i], buf, len / 2);

	if(!ui_StartFlag)
	{	ui_StartFlag = HEADER_START;

		ul_SumCount  = buf[1]; //���� DATA �� ���� ���� ����
		ui_Channel 	 = buf[2]; //��� ���� ����
		//���� ǥ�� ���� �ʱ�ȭ
		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}
	ul_DataCount += len; //8BIT ������ Ethernet���� ���ŵ� BigEndian �ڷ���� ����

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i))// && (sp_SCHEDULE_DATA[ui_i]).CPU1_Start == (sp_CELL_SCAN_DATA[ui_i]).CPU2_Start)
		{
			memcpy_fast_far(&(sp_SCHEDULE_DATA[ui_i]).CPU1_START.Header + PointerCount[ui_i], buf, len / 2);
			PointerCount[ui_i] += (len / 2);
			(sp_SCHEDULE_DATA[ui_i]).CPU1_Start++;// = ~(sp_CELL_SCAN_DATA[ui_i]).CPU2_Start;
		}
	}

	if(ul_DataCount >= ul_SumCount)//Data �� ������ ������ Counting�� ������ ���� ���ų�
	{							   //������(���� ����)�۾� �����ϰ� �ش� Flag �ʱ�ȭ ��.
		//���� ǥ�� ���� �ʱ�ȭ
		ui_StartFlag = CLEAR;

		ul_SumCount  = CLEAR;
		ui_Channel   = CLEAR;

		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}

	return(ui_StartFlag); //SUCCESS Or FAIL
						  //���ϰ����� ���� �� ���� �Ϸ�� 0�� ó�����϶��� HEADER_DATA�� ����.
}

Uint16 COMMUNICATION_Ethernet_PAUSERead(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, Uint16 *buf, Uint32 len)
{
	Uint16 ui_i = 0;
	volatile static Uint16 ui_Channel = 0;
	volatile static Uint16 ui_StartFlag = 0;
	volatile static Uint32 ul_DataCount = 0;
	volatile static Uint32 ul_SumCount	= 0;
	volatile static Uint32 PointerCount[COMMUNICATION_CHANNEL];

	if(!ui_StartFlag)
	{	ui_StartFlag = HEADER_PAUSE;

		ul_SumCount  = buf[1]; //���� DATA �� ���� ���� ����
		ui_Channel 	 = buf[2]; //��� ���� ����
		//���� ǥ�� ���� �ʱ�ȭ
		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}
	ul_DataCount += len; //8BIT ������ Ethernet���� ���ŵ� BigEndian �ڷ���� ����

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i))// && (sp_SCHEDULE_DATA[ui_i]).CPU1_Pause == (sp_CELL_SCAN_DATA[ui_i]).CPU2_Pause)
		{
			memcpy_fast_far(&(sp_SCHEDULE_DATA[ui_i]).CPU1_PAUSE.Header + PointerCount[ui_i], buf, len / 2);
			PointerCount[ui_i] += (len / 2);
			(sp_SCHEDULE_DATA[ui_i]).CPU1_Pause++;// = ~(sp_CELL_SCAN_DATA[ui_i]).CPU2_Pause;
		}
	}

	if(ul_DataCount >= ul_SumCount)//Data �� ������ ������ Counting�� ������ ���� ���ų�
	{							   //������(���� ����)�۾� �����ϰ� �ش� Flag �ʱ�ȭ ��.
		//���� ǥ�� ���� �ʱ�ȭ
		ui_StartFlag = CLEAR;

		ul_SumCount  = CLEAR;
		ui_Channel   = CLEAR;

		ul_DataCount = CLEAR;
		for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)PointerCount[ui_i] = CLEAR;
	}

	return(ui_StartFlag); //SUCCESS Or FAIL
						  //���ϰ����� ���� �� ���� �Ϸ�� 0�� ó�����϶��� HEADER_DATA�� ����.
}

Uint32 COMMUNICATION_Ethernet_DATAWrite(volatile S_COMMUNICATION_DRAM **sp_IF, Uint16 *buf)
{
//	Uint16 ui_Channel = 0;									//������ ����ؼ� �׻� ������ ��.
	Uint16 ui_i = 0;										//ī���ʹ� �󸶳� ���� ����غ��� �� �� ����.
	Uint32 BufferCount = 0;									//���ͷ�Ʈ�� �־ ��� �ؾ� �� �� ����.
	Uint32 Index = 0;
	Uint32 Count  = 0;

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		Count = RINGBUFFER_GetSize((S_RINGBUFFER_FILE *)&((*sp_IF)[ui_i].RING_DATA), COMMUNICATION_TX_DATA_LENGTH);//����Ÿ �������� ī��
		if(Count)// && (*sp_IF)[ui_i].WriteFlagData == SET)//����Ÿ�� �ִ��� Ȯ��
		{
			Index = RINGBUFFER_Get((S_RINGBUFFER_FILE *)&((*sp_IF)[ui_i].RING_DATA), COMMUNICATION_TX_DATA_LENGTH);    //����Ÿ �������� ī��
			(*sp_IF)[ui_i].DATA[Index].Header = HEADER_DATA;
			(*sp_IF)[ui_i].DATA[Index].DataCount = (COMMUNICATION_INTEGER_LENGTH_DATA * 2);
			(*sp_IF)[ui_i].DATA[Index].ChannelIndex = 1 << ui_i;

			memcpy_fast_far(buf + BufferCount, &((*sp_IF)[ui_i].DATA[Index].Header), (COMMUNICATION_INTEGER_LENGTH_DATA));
			BufferCount += COMMUNICATION_INTEGER_LENGTH_DATA;
			//(*sp_IF)[ui_i].WriteFlagData = CLEAR; // ���ͳ� �۽� �÷���
		}
	}
	return(BufferCount * 2); //SUCCESS Or FAIL
}

Uint32 COMMUNICATION_Ethernet_ENDWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile  S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, Uint16 *buf)
{
	Uint16 ui_Channel = 0;									//������ ����ؼ� �׻� ������ ��.
	Uint16 ui_i = 0;										//ī���ʹ� �󸶳� ���� ����غ��� �� �� ����.
	Uint32 BufferCount = 0;									//���ͷ�Ʈ�� �־ ��� �ؾ� �� �� ����.

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{
		ui_Channel |= (sp_CELL_SCAN_DATA[ui_i]).CPU2_END.ChannelIndex;//�÷��׺� ������ �����Ѵ�. ******
	}

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i) && (sp_SCHEDULE_DATA[ui_i]).CPU1_End != (sp_CELL_SCAN_DATA[ui_i]).CPU2_End)// (if))//����Ÿ�� �ִ��� Ȯ���ؾ���. �ϴ� ����
		{
			memcpy_fast_far(buf + BufferCount, &((sp_CELL_SCAN_DATA[ui_i]).CPU2_END.Header), COMMUNICATION_INTEGER_LENGTH_END);
			BufferCount += COMMUNICATION_INTEGER_LENGTH_END;
			(sp_SCHEDULE_DATA[ui_i]).CPU1_End = (sp_CELL_SCAN_DATA[ui_i]).CPU2_End; // ���ͳ� �۽� �÷���
		}
	}
	return(BufferCount * 2); //SUCCESS Or FAIL �̴��� (8bit) ���� ��
}

Uint32 COMMUNICATION_Ethernet_ALARMWrite(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, Uint16 *buf)
{
	Uint16 ui_Channel = 0;									//������ ����ؼ� �׻� ������ ��.
	Uint16 ui_i = 0;										//ī���ʹ� �󸶳� ���� ����غ��� �� �� ����.
	Uint32 BufferCount = 0;									//���ͷ�Ʈ�� �־ ��� �ؾ� �� �� ����.

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{
		ui_Channel |= (sp_CELL_SCAN_DATA[ui_i]).CPU2_ALARM.ChannelIndex;//�÷��׺� ������ �����Ѵ�. ******
	}

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i) && (sp_SCHEDULE_DATA[ui_i]).CPU1_Alarm != (sp_CELL_SCAN_DATA[ui_i]).CPU2_Alarm)// (if))//����Ÿ�� �ִ��� Ȯ���ؾ���. �ϴ� ����
		{
			memcpy_fast_far(buf + BufferCount, &((sp_CELL_SCAN_DATA[ui_i]).CPU2_ALARM.Header), COMMUNICATION_INTEGER_LENGTH_ALARM);
			BufferCount += COMMUNICATION_INTEGER_LENGTH_ALARM;
			(sp_SCHEDULE_DATA[ui_i]).CPU1_Alarm = (sp_CELL_SCAN_DATA[ui_i]).CPU2_Alarm;
		}
	}
	return(BufferCount * 2); //SUCCESS Or FAIL
}

void COMMUNICATION_DRAM_STEPWrite(volatile S_COMMUNICATION_DRAM **sp_IF,
		volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA)
{
	//CPU2���� index���� �Ͽ� �÷��׸� �ָ� �ش� �Ǵ� ����Ÿ�� �Ѱܾ��Ѵ�.
	//Start ��ȣ�� ���� �׶� ���� ����Ÿ�� �ѱ��.
	//End ��ȣ�� ���� �׶� ���� ����Ÿ�� �ѱ��� �ʴ´�.

	Uint16 ui_Channel = 0;
	Uint16 ui_i = 0;
	Uint16 ui_Index = 0;

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{
		ui_Channel |= (*sp_IF)[ui_i].STEP.ChannelIndex;//�÷��׺� ������ �����Ѵ�. ******
	}

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����

		if((sp_CELL_SCAN_DATA[ui_i]).CPU2_StepIndex != (sp_SCHEDULE_DATA[ui_i]).CPU1_StepIndex)//���� Ŀ�ǵ� ��� ���� ��� ���� �Ҽ� �ְ� ���� ��!!!!!
		if(ui_Channel & (0x1 << ui_i))// && (sp_SCHEDULE_DATA[ui_i]).CPU1_Step == (sp_CELL_SCAN_DATA[ui_i]).CPU2_Step)// (if))//����Ÿ�� �ִ��� Ȯ���ؾ���. �ϴ� ����
		{
			(sp_SCHEDULE_DATA[ui_i]).CPU1_StepHeader = (*sp_IF)[ui_i].STEP.Header;
			(sp_SCHEDULE_DATA[ui_i]).CPU1_StepDataCount = (*sp_IF)[ui_i].STEP.DataCount;
			(sp_SCHEDULE_DATA[ui_i]).CPU1_StepChannelIndex = (*sp_IF)[ui_i].STEP.ChannelIndex;

			ui_Index = (sp_CELL_SCAN_DATA[ui_i]).CPU2_StepIndex;// �ҷ��� ���� �ּ�
			//First Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[0 ] = (*sp_IF)[ui_i].STEP.STEP[ui_Index];
			//Second Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1]  = (*sp_IF)[ui_i].STEP.STEP[((*sp_IF)[ui_i].STEP.STEP[ui_Index].GOTO_INDEX)];
			//Third Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[7 ] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1].GOTO_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[8 ] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1].RETURN_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[9 ] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1].END_VOLTAGE_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[10] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1].END_TIME_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[11] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1].END_CURRENT_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[12] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[1].END_CAPACITY_INDEX)];

			//Second Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2]  = (*sp_IF)[ui_i].STEP.STEP[((*sp_IF)[ui_i].STEP.STEP[ui_Index].RETURN_INDEX)];
			//Third Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[13] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2].GOTO_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[14] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2].RETURN_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[15] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2].END_VOLTAGE_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[16] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2].END_TIME_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[17] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2].END_CURRENT_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[18] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[2].END_CAPACITY_INDEX)];

			//Second Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3]  = (*sp_IF)[ui_i].STEP.STEP[((*sp_IF)[ui_i].STEP.STEP[ui_Index].END_VOLTAGE_INDEX)];
			//Third Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[19] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3].GOTO_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[20] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3].RETURN_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[21] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3].END_VOLTAGE_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[22] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3].END_TIME_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[23] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3].END_CURRENT_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[24] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[3].END_CAPACITY_INDEX)];

			//Second Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4]  = (*sp_IF)[ui_i].STEP.STEP[((*sp_IF)[ui_i].STEP.STEP[ui_Index].END_TIME_INDEX)];
			//Third Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[25] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4].GOTO_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[26] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4].RETURN_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[27] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4].END_VOLTAGE_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[28] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4].END_TIME_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[29] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4].END_CURRENT_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[30] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[4].END_CAPACITY_INDEX)];

			//Second Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5]  = (*sp_IF)[ui_i].STEP.STEP[((*sp_IF)[ui_i].STEP.STEP[ui_Index].END_CURRENT_INDEX)];
			//Third Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[31] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5].GOTO_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[32] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5].RETURN_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[33] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5].END_VOLTAGE_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[34] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5].END_TIME_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[35] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5].END_CURRENT_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[36] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[5].END_CAPACITY_INDEX)];

			//Second Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6]  = (*sp_IF)[ui_i].STEP.STEP[((*sp_IF)[ui_i].STEP.STEP[ui_Index].END_CAPACITY_INDEX)];
			//Third Step Level
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[37] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6].GOTO_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[38] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6].RETURN_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[39] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6].END_VOLTAGE_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[40] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6].END_TIME_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[41] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6].END_CURRENT_INDEX)];
			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[42] = (*sp_IF)[ui_i].STEP.STEP[((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[6].END_CAPACITY_INDEX)];

			(sp_SCHEDULE_DATA[ui_i]).CPU1_Step++;// = ~(sp_SCHEDULE_DATA[ui_i]).CPU1_Step;//������Ʈ ����Ÿ �Ѵ�.
//			(sp_SCHEDULE_DATA[ui_i]).CPU1_STEP = (*sp_IF)[ui_i].STEP.STEP[ui_Index];
//			memcpy_fast_far(&((sp_SCHEDULE_DATA[ui_i]).CPU1_STEP[0]), &((*sp_IF)[ui_i].STEP.STEP[ui_Index]), COMMUNICATION_INTEGER_LENGTH_STEP);
			(sp_SCHEDULE_DATA[ui_i]).CPU1_StepIndex = ui_Index;
		}
	}
}

Uint16 COMMUNICATION_DRAM_DATARead(volatile S_COMMUNICATION_DRAM **sp_IF,
		volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA)
{
	Uint16 ui_Channel = 0;
	Uint16 ui_i = 0;
	Uint32 ul_Index = 0;


	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{
		ui_Channel |= ((sp_CELL_SCAN_DATA[ui_i]).CPU2_Data != (sp_SCHEDULE_DATA[ui_i]).CPU1_Data) << ui_i;//�÷��׺� ������ �����Ѵ�. ******
	}

	for(ui_i = 0; ui_i < COMMUNICATION_CHANNEL; ui_i++)
	{	//4���� ä�ο��� �ش� �Ǵ� Channel�� Ȯ���ϰ� �ش� SETP DATA�� �ڷ� ����
		if(ui_Channel & (0x1 << ui_i))// (if))//����Ÿ�� �ִ��� Ȯ���ؾ���. �ϴ� ����
		{
			ul_Index = RINGBUFFER_Put((S_RINGBUFFER_FILE *)&(*sp_IF)[ui_i].RING_DATA, COMMUNICATION_TX_DATA_LENGTH);
			memcpy_fast_far(&((*sp_IF)[ui_i].DATA[ul_Index]), &((sp_CELL_SCAN_DATA[ui_i]).CPU2_DATA), COMMUNICATION_INTEGER_LENGTH_DATA);

			(sp_SCHEDULE_DATA[ui_i]).CPU1_Data = (sp_CELL_SCAN_DATA[ui_i]).CPU2_Data;
//			(*sp_IF)[ui_i].WriteFlagData = SET;  // �̴��� �۽� �÷��� ON
		}
	}
	return (ui_Channel);
}

void COMMUNICATION_Write(volatile S_COMMUNICATION_DRAM **sp_IF, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA)
{
    COMMUNICATION_DRAM_STEPWrite(sp_IF, sp_SCHEDULE_DATA, sp_CELL_SCAN_DATA);
}
void COMMUNICATION_Read(volatile S_COMMUNICATION_DRAM **sp_IF, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA,
		volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA)
{
    COMMUNICATION_DRAM_DATARead(sp_IF, sp_SCHEDULE_DATA, sp_CELL_SCAN_DATA);
}
#endif //End #ifdef INTERFACE_CPU1

//
//INTERFACE_CPU2
//
#ifdef INTERFACE_CPU2

extern volatile S_SCHEDULE_DATA s_CPU_1_GSRAM[];
extern volatile S_CELL_SCAN_DATA s_CPU_2_GSRAM[];
extern volatile Uint16 ui_UpdateflagCPU1;
extern volatile Uint16 ui_UpdateflagCPU2;

extern E_DSP_SCIA_FLAG e_SCIA_RxState[COMMUNICATION_CHANNEL];
extern E_DSP_SCIA_FLAG e_SCIA_TxState;

#pragma CODE_SECTION(COMMUNICATION_RS422A_TxPacket, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_RS422A_RxUnPacket, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");

Uint16 COMMUNICATION_RS422A_TxPacket(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile Uint16 Channel)
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

	memcpy_fast_far(TxBuff + Counter, &(sp_SCHEDULE_DATA[Channel]), DSP_SCIA_TX_DATA_LENGTH);
	Counter += DSP_SCIA_TX_DATA_LENGTH;


	memcpy_fast_far(TxBuff + Counter, Dumy, DSP_SCIA_TX_DUMY_LENGTH);
	Counter += DSP_SCIA_TX_DUMY_LENGTH;

	CRC16 = CRC16_Conversion(TxBuff, DSP_SCIA_TX_MAX_DATA_LENGTH - DSP_SCIA_CRC16_LENGTH);

	memcpy_fast_far(TxBuff + Counter, &CRC16, DSP_SCIA_CRC16_LENGTH);
	Counter += DSP_SCIA_CRC16_LENGTH;

	DSP_SCIA_Write(TxBuff, Counter);
	Counter  = 0;
	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_RS422A_RxUnPacket(volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 *RxBuff, volatile Uint16 Channel)
{
	memcpy_fast_far(&(sp_CELL_SCAN_DATA[Channel]), RxBuff, sizeof(S_CELL_SCAN_DATA));
	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 COMMUNICATION_WritePass(volatile Uint16 *TxBuff, volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA)
{
	return(SUCCESS); //SUCCESS Or FAIL
}
Uint16 COMMUNICATION_ReadPass(volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 *RxBuff)
{
	return(SUCCESS); //SUCCESS Or FAIL
}

#endif //End #ifdef INTERFACE_CPU2


//
//DEVICE_CPU2
//
#ifdef DEVICE_CPU2

extern volatile S_SCHEDULE_DATA SCHEDULE_DATA[4];
extern volatile S_CELL_SCAN_DATA CELL_SCAN_DATA[4];
extern volatile Uint32 ui_UpdateflagCPU1[4];
extern volatile Uint32 ui_UpdateflagCPU2[4];

extern E_DSP_SCIA_FLAG e_SCIA_RxState;
extern E_DSP_SCIA_FLAG e_SCIA_TxState;



#pragma CODE_SECTION(COMMUNICATION_RS422A_TxPacket, ".TI.ramfunc");
#pragma CODE_SECTION(COMMUNICATION_RS422A_RxUnPacket, ".TI.ramfunc");
//#pragma CODE_SECTION(, ".TI.ramfunc");

Uint16 COMMUNICATION_RS422A_TxPacket(volatile Uint16 *TxBuff, volatile S_CELL_SCAN_DATA *sp_CELL_SCAN_DATA, volatile Uint16 Channel)
{
	Uint16 StartFlag[DSP_SCIA_HEADER_LENGTH] ={0xAAAA, 0xAAAA, 0xAAAA, 0x0000, 0x0000};
	Uint16 Counter = 0;
	Uint16 CRC16 = 0;
	Uint16 Dumy[DSP_SCIA_TX_DUMY_LENGTH] = {0,};

	for(Counter = 0; Counter < DSP_SCIA_TX_DUMY_LENGTH; Counter++){
	    Dumy[Counter] = DSP_SCIA_TX_DUMY;
	}
	Counter = 0;
	StartFlag[DSP_SCIA_START_FLAG_LENGTH] =
			(((Channel+CONTROL_CHANNEL_START_INDEX) << 8) & 0xFF00) | ((Channel+CONTROL_CHANNEL_START_INDEX) & 0x00FF);
	        //((Channel << 8) & 0xFF00) | (Channel & 0x00FF);


	StartFlag[DSP_SCIA_START_FLAG_LENGTH + 1] =
			(((Channel+CONTROL_CHANNEL_START_INDEX) << 8) & 0xFF00) | (Channel+CONTROL_CHANNEL_START_INDEX & 0x00FF);
	        //((Channel << 8) & 0xFF00) | (Channel & 0x00FF);


	memcpy_fast_far(TxBuff, StartFlag, DSP_SCIA_HEADER_LENGTH);
	Counter += DSP_SCIA_HEADER_LENGTH;//5

	memcpy_fast_far(TxBuff + Counter, &(sp_CELL_SCAN_DATA[Channel]), DSP_SCIA_TX_DATA_LENGTH);
	Counter += DSP_SCIA_TX_DATA_LENGTH;//1729

	memcpy_fast_far(TxBuff + Counter, Dumy, DSP_SCIA_TX_DUMY_LENGTH);
	Counter += DSP_SCIA_TX_DUMY_LENGTH;//1747

	//Bank = //�����۷� ��ũ ī��Ʈ ����.
	CRC16 = CRC16_Conversion(TxBuff, DSP_SCIA_TX_MAX_DATA_LENGTH - DSP_SCIA_CRC16_LENGTH);

	memcpy_fast_far(TxBuff + Counter, &(CRC16), DSP_SCIA_CRC16_LENGTH);
	Counter += DSP_SCIA_CRC16_LENGTH;//1748

	DSP_SCIA_Write(TxBuff, Counter);

	return(SUCCESS); //SUCCESS Or FAIL
}


Uint16 COMMUNICATION_RS422A_RxUnPacket(volatile S_SCHEDULE_DATA *sp_SCHEDULE_DATA, volatile Uint16 *RxBuff, volatile Uint16 Channel)
{
	memcpy_fast_far(&(sp_SCHEDULE_DATA[Channel]), RxBuff, sizeof(S_SCHEDULE_DATA));
	return(SUCCESS); //SUCCESS Or FAIL
}
#endif //End #ifdef DEVICE_CPU2

//
//DEVICE_CPU1

//
// End of file
//

