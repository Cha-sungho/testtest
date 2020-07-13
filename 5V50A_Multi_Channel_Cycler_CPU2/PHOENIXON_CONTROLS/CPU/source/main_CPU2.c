/*
 * main.c
 *
 *  Created on: 2018. 5. 16.
 *      Author: BGKim
 */

#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

//
// PhoenixonControls_COMMUNICATION.c
//
volatile S_SCHEDULE_DATA SCHEDULE_DATA[CONTROL_CHANNEL_PER_BOARD];
volatile S_CELL_SCAN_DATA CELL_SCAN_DATA[CONTROL_CHANNEL_PER_BOARD];

volatile Uint32 GSRAM_DATA_BANK_INDEX_CPU1[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile Uint32 GSRAM_DATA_BANK_INDEX_CPU2[CONTROL_CHANNEL_PER_BOARD] = { 0, };

volatile Uint32 SYNC_SIGNAL_CPU1 = 0;
volatile Uint32 SYNC_SIGNAL_CPU2 = 0;

volatile Uint32 ui_UpdateflagCPU1[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile Uint32 ui_UpdateflagCPU2[CONTROL_CHANNEL_PER_BOARD] = { 0, };

volatile Uint32 ui_UpdateSTEPCPU1[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile Uint32 ui_UpdateSTEPCPU2[CONTROL_CHANNEL_PER_BOARD] = { 0, };

volatile Uint32 ui_UpdateDATACPU1[CONTROL_CHANNEL_PER_BOARD] = { 0, };
volatile Uint32 ui_UpdateDATACPU2[CONTROL_CHANNEL_PER_BOARD] = { 0, };

volatile Uint16 SCIATxBuff[DSP_SCIA_TX_MAX_BUFFER_LENGTH] 	= {0,};
volatile Uint16 SCIARxBuff[DSP_SCIA_RX_MAX_BUFFER_LENGTH]	= {0,};

#pragma DATA_SECTION(CELL_SCAN_DATA,"SHARE_GSRAM_CPU1");
#pragma DATA_SECTION(GSRAM_DATA_BANK_INDEX_CPU1,"SHARE_GSRAM_CPU1");
#pragma DATA_SECTION(SYNC_SIGNAL_CPU1,"SHARE_GSRAM_CPU1");
#pragma DATA_SECTION(ui_UpdateflagCPU1,"SHARE_GSRAM_CPU1");
#pragma DATA_SECTION(ui_UpdateSTEPCPU1,"SHARE_GSRAM_CPU1");
#pragma DATA_SECTION(ui_UpdateDATACPU1,"SHARE_GSRAM_CPU1");

#pragma DATA_SECTION(SCHEDULE_DATA,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(GSRAM_DATA_BANK_INDEX_CPU2,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(SYNC_SIGNAL_CPU2,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(ui_UpdateflagCPU2,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(ui_UpdateSTEPCPU2,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(ui_UpdateDATACPU2,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(SCIATxBuff,"SHARE_GSRAM_CPU2");
#pragma DATA_SECTION(SCIARxBuff,"SHARE_GSRAM_CPU2");

//End

//
// PhoenixonControls_SCI.c
//
extern volatile Uint16 SCIARxCount;
extern volatile Uint16 SCIATxCount;
extern volatile Uint16 SCIARxChannel;
extern volatile Uint16 SCIATxChannel;
extern volatile Uint16 SCIATxData[];
extern volatile Uint16 SCIARxData[];
extern volatile S_DSP_SCIA_RINGBUFFER_FILE SCIARxRing;
extern volatile S_DSP_SCIA_RINGBUFFER_FILE SCIATxRing;
extern volatile E_DSP_SCIA_FLAG e_SCIA_RxState;
extern volatile E_DSP_SCIA_FLAG e_SCIA_TxState;
//End

#ifdef _DEBUG
Uint16 DEBUG_S_SCHEDULE_DATA     = sizeof(S_SCHEDULE_DATA);
Uint16 DEBUG_S_CELL_SCAN_DATA     = sizeof(S_CELL_SCAN_DATA);
Uint16 DEBUG_DSP_SCIA_TX_DATA_LENGTH		= DSP_SCIA_TX_DATA_LENGTH;
Uint16 DEBUG_DSP_SCIA_RX_DATA_LENGTH		= DSP_SCIA_RX_DATA_LENGTH;
Uint16 DEBUG_DSP_SCIA_TX_TOTAL_LENGTH		= DSP_SCIA_TX_TOTAL_LENGTH;
Uint16 DEBUG_DSP_SCIA_RX_TOTAL_LENGTH		= DSP_SCIA_RX_TOTAL_LENGTH;
Uint16 DEBUG_DSP_SCIA_TX_DUMY_LENGTH		= DSP_SCIA_TX_DUMY_LENGTH;
Uint16 DEBUG_DSP_SCIA_RX_DUMY_LENGTH		= DSP_SCIA_RX_DUMY_LENGTH;
Uint16 DEBUG_DSP_SCIA_TX_MAX_DATA_LENGTH	= DSP_SCIA_TX_MAX_DATA_LENGTH;
Uint16 DEBUG_DSP_SCIA_RX_MAX_DATA_LENGTH 	= DSP_SCIA_RX_MAX_DATA_LENGTH;
Uint16 DEBUG_DSP_SCIA_TX_MAX_BUFFER_LENGTH 	= DSP_SCIA_TX_MAX_BUFFER_LENGTH;
Uint16 DEBUG_DSP_SCIA_RX_MAX_BUFFER_LENGTH	= DSP_SCIA_RX_MAX_BUFFER_LENGTH;
#endif //_DEBUG

Uint16 PHOENIXON_CONTROLS_Variable_Shared;

void main(void) {
//	Uint16 ui_i = 0;
//	Uint32 ul_TimeOut = 0;
	E_DSP_SCIA_FLAG e_Flag = SCIA_INIT;
    //
    // Initialize device clock and peripherals
    //

	Device_init();
    DSP_TimerInit();
    DSP_SCIA_Init();

    //
    // Initialize GPIO and configure the GPIO pin as a push-pull output
    //
    // This is configured by CPU1

    //
    // Initialize PIE and clear PIE registers. Disables CPU interrupts.
    //
    Interrupt_initModule();

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    //
    Interrupt_initVectorTable();

    //
    // ISRs
    //
    Interrupt_register(INT_TIMER0, &DSP_Timer0_ISR);
    Interrupt_register(INT_TIMER1, &DSP_Timer1_ISR);
    Interrupt_register(INT_TIMER2, &DSP_Timer2_ISR);
    Interrupt_register(INT_SCIA_RX, &DSP_SCIA_RX_ISR);
    Interrupt_register(INT_SCIA_TX, &DSP_SCIA_TX_ISR);
    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in configCPUTimer and initCPUTimers, the below settings must also
    // be updated.
    //
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    CPUTimer_enableInterrupt(CPUTIMER1_BASE);
    CPUTimer_enableInterrupt(CPUTIMER2_BASE);

    //
    // Enables CPU int1, int13, and int14 which are connected to CPU-Timer 0,
    // CPU-Timer 1, and CPU-Timer 2 respectively.
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    Interrupt_enable(INT_TIMER0);
    Interrupt_enable(INT_TIMER1);
    Interrupt_enable(INT_TIMER2);
    Interrupt_enable(INT_SCIA_RX);
    Interrupt_enable(INT_SCIA_TX);
    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
    EINT;
    ERTM;
    CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);

    //
    // Loop Forever
    //
    for(;;)
    {
    	e_Flag = DSP_SCIA_RxUpDate(SCIARxData, SCIARxBuff, &SCIARxCount, &SCIARxChannel);

#if 1
#if 1
    	SCIARxChannel -= CONTROL_CHANNEL_START_INDEX;
        switch (e_Flag)
        {
            case SCIA_SYNC:
                SYNC_SIGNAL_CPU2++;
                break;
            case SCIA_DATA:
                COMMUNICATION_RS422A_TxPacket(SCIATxBuff, CELL_SCAN_DATA, SCIARxChannel);
                //TODO SCIARxChannel이 어떻게 변하는지 확인 필요
                ui_UpdateDATACPU2[SCIARxChannel]++;
                GSRAM_DATA_BANK_INDEX_CPU2[SCIARxChannel] = GSRAM_DATA_BANK_INDEX_CPU1[SCIARxChannel];
                break;
            case SCIA_STEP:
                COMMUNICATION_RS422A_RxUnPacket(SCHEDULE_DATA, &(SCIARxData[DSP_SCIA_HEADER_LENGTH]), SCIARxChannel);
                ui_UpdateSTEPCPU2[SCIARxChannel]++;
                break;
        }
#endif
#else
    	if(SCIARxChannel == COMMUNICATION_CHANNEL_NUMBER)
		{
			switch(e_Flag)
			{
			case SCIA_SYNC:
				SYNC_SIGNAL_CPU2++;
				break;
			case SCIA_DATA:
				COMMUNICATION_RS422A_TxPacket(SCIATxBuff, CELL_SCAN_DATA, 0);
				//TODO SCIARxChannel이 어떻게 변하는지 확인 필요
				ui_UpdateDATACPU2[0]++;
				GSRAM_DATA_BANK_INDEX_CPU2[0] = GSRAM_DATA_BANK_INDEX_CPU1[0];
				break;
			case SCIA_STEP:
				COMMUNICATION_RS422A_RxUnPacket(SCHEDULE_DATA, &(SCIARxData[DSP_SCIA_HEADER_LENGTH]), 0);
				ui_UpdateSTEPCPU2[0]++;
				break;
			}
    	}
#endif

    	if((SCI_RXSTATUS_OVERRUN | SCI_RXSTATUS_PARITY | SCI_RXSTATUS_ERROR | SCI_RXSTATUS_FRAMING) & SCI_getRxStatus(SCIA_BASE))
    	{
    		SCI_performSoftwareReset(SCIA_BASE);
    	}

//    	if(SCI_RXSTATUS_ERROR == SCI_getRxStatus(SCIA_BASE))SCI_performSoftwareReset(SCIA_BASE);
    	PHOENIXON_CONTROLS_Funtion();
    }
}
void PHOENIXON_CONTROLS_Funtion(void)
{

}

//
// End of file
//

