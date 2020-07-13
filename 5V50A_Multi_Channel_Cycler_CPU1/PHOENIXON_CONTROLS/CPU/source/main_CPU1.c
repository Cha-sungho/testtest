/*
 * main.c
 *
 *  Created on: 2018. 5. 16.
 *      Author: BGKim
 */

#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"
#include "../../../common/include/F2837xD_Ipc_drivers.h"

//
// FPU header file to access memcpy_fast_far().
//
//#include "../include/PhoenixonControls_FPU.h"

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

volatile Uint16 SCIATxBuff[DSP_SCIA_TX_MAX_BUFFER_LENGTH]   = {0,};
volatile Uint16 SCIARxBuff[DSP_SCIA_RX_MAX_BUFFER_LENGTH]   = {0,};

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
//#pragma CODE_SECTION(PHOENIXON_CONTROLS_Funtion, ".TI.ramfunc");
//#pragma CODE_SECTION(printf, "ramfuncs");

S_OPERATION_FILE s_Operation[CONTROL_CHANNEL_PER_BOARD];

float g_capacity_1h = 0.0;
extern volatile Uint16 DataCount[CONTROL_CHANNEL_PER_BOARD];
volatile Uint16 DataCount_old[CONTROL_CHANNEL_PER_BOARD] = { 0, };

Uint16 plus_conversion_flag = 0;
Uint16 minus_conversion_flag = 0;
Uint16 conversion_flag = 0;

void main(void)
{
    //
    // Initialize device clock and peripherals
    //
    Uint16 i = 0;
    g_capacity_1h = (float) (1.0 / 3600.0);

    Device_init();
    DSP_GpioInit();
    DSP_EmifInit();
//    DSP_SpiInit();
    DSP_ADCInit();
    DSP_TimerInit();
    Init_SPI2();

    AD5547_Init();
//    AD7606_Init();
    AD7608_Init();
    AD5664R_Init();


//    CALIBRATION_Init();
//    CONTROL_Init();


    for( i = 0; i < CONTROL_CHANNEL_PER_BOARD; i++)     // 각 채널 데이터 초기화
    {
        OPERATION_Init(s_Operation, i);
    }


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

    //
    // Enable Global Interrupt (INTM) and realtime interrupt (DBGM)
    //
#ifdef _STANDALONE
    IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
#endif
    EINT;
    ERTM;

#if (VI_ADC_TUNE == 1)
    InitVIADCTuneProperties();
    RunVIADCTune();
#endif

    //
    // Starts CPU-Timer 0, CPU-Timer 1, and CPU-Timer 2.
    //
    CPUTimer_startTimer(CPUTIMER0_BASE);
    CPUTimer_startTimer(CPUTIMER1_BASE);
    CPUTimer_startTimer(CPUTIMER2_BASE);

    //
    // Loop Forever
    //
    i = 0;
    for(;;)
     {
        //For loop time measurement
        //GpioDataRegs.GPDCLEAR.bit.GPIO99 = 1;
#if 1
        for (i = 0; i < CONTROL_CHANNEL_PER_BOARD; i++) {
#if 1
            if (SYNC_SIGNAL_CPU1 != SYNC_SIGNAL_CPU2) {
#if 1
                if (DataCount[0] != DataCount_old[0]) {
                    if (DataCount[0] > DataCount_old[0]) {
                        CPUTimer_setPeriod(CPUTIMER0_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 1000));
                    } else {
                        CPUTimer_setPeriod(CPUTIMER0_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 999));
                    }
                }

                if (DataCount[2] != DataCount_old[2]) {
                    if (DataCount[2] > DataCount_old[2]) {
                        CPUTimer_setPeriod(CPUTIMER1_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 1000));
                    } else {
                        CPUTimer_setPeriod(CPUTIMER1_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 999));
                    }
                }

                if (DataCount[3] != DataCount_old[3]) {
                    if (DataCount[3] > DataCount_old[3]) {
                        CPUTimer_setPeriod(CPUTIMER2_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 1000));
                    } else {
                        CPUTimer_setPeriod(CPUTIMER2_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 999));
                    }
                }

                DataCount_old[0] = DataCount[0];
                DataCount_old[2] = DataCount[2];
                DataCount_old[3] = DataCount[3];
                SYNC_SIGNAL_CPU1 = SYNC_SIGNAL_CPU2;
#else
                if (DataCount[0] > DataCount_old[0]) {
                    g_timer_fast_count[0]++;
                    CPUTimer_setPeriod(CPUTIMER0_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 1000));
                    CPUTimer_setPeriod(CPUTIMER1_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 1000));
                    CPUTimer_setPeriod(CPUTIMER2_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 1000));
                } else if (DataCount[0] < DataCount_old[0]) {
                    g_timer_slow_count[0]++;
                    CPUTimer_setPeriod(CPUTIMER0_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 999));
                    CPUTimer_setPeriod(CPUTIMER1_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 999));
                    CPUTimer_setPeriod(CPUTIMER2_BASE, (Uint32) (DEVICE_SYSCLK_FREQ / 1000000 * 999));
                } else {

                }


                DataCount_old[0] = DataCount[0];
                SYNC_SIGNAL_CPU1 = SYNC_SIGNAL_CPU2;
#endif
            }
#endif
            if ((ui_UpdateSTEPCPU1[i] != ui_UpdateSTEPCPU2[i]) && (s_Operation[i].VARIABLE_StartFlag == CLEAR))  //ui_UpdateflagCPU2++ 증가형임.
            {
                s_Operation[i].VARIABLE_UpdateData++;
                COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, i, 0);

                ui_UpdateSTEPCPU1[i] = ui_UpdateSTEPCPU2[i];
            }

            COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, i);

#if (SENSING_TYPE == SENSING_MAIN)
            OPERATION_Sensing(s_Operation, i);
#elif (SENSING_TYPE == SENSING_SEPERATE)
            OPERATION_Sensing(s_Operation, i);
#endif
            //OPERATION_ModeCheck(s_Operation, i);
            OPERATION_Alram(s_Operation, i);
            OPERATION_Run(s_Operation, i);
        }
#else
        //Main loop cycle about 1~1.7ms
        //channel 1
        if ((ui_UpdateSTEPCPU1[0] != ui_UpdateSTEPCPU2[0]) && (s_Operation[0].VARIABLE_StartFlag == CLEAR))//ui_UpdateflagCPU2++ 증가형임.
        {
            s_Operation[0].VARIABLE_UpdateData++;
            COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 0, 0);

            ui_UpdateSTEPCPU1[0] = ui_UpdateSTEPCPU2[0];
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 0);

        OPERATION_Sensing(s_Operation, 0);
        //OPERATION_ModeCheck(s_Operation, 0);
        OPERATION_Alram(s_Operation, 0);
        OPERATION_Run(s_Operation, 0);

        //channel 2
        if ((ui_UpdateSTEPCPU1[1] != ui_UpdateSTEPCPU2[1]) && (s_Operation[1].VARIABLE_StartFlag == CLEAR))//ui_UpdateflagCPU2++ 증가형임.
        {
            s_Operation[1].VARIABLE_UpdateData++;
            COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 1, 0);

            ui_UpdateSTEPCPU1[1] = ui_UpdateSTEPCPU2[1];
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 1);

        OPERATION_Sensing(s_Operation, 1);
        //OPERATION_ModeCheck(s_Operation, 1);
        OPERATION_Alram(s_Operation, 1);
        OPERATION_Run(s_Operation, 1);

        //channel 3
        if ((ui_UpdateSTEPCPU1[2] != ui_UpdateSTEPCPU2[2]) && (s_Operation[2].VARIABLE_StartFlag == CLEAR))//ui_UpdateflagCPU2++ 증가형임.
        {
            s_Operation[2].VARIABLE_UpdateData++;
            COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 2, 0);

            ui_UpdateSTEPCPU1[2] = ui_UpdateSTEPCPU2[2];
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 2);

        OPERATION_Sensing(s_Operation, 2);
        //OPERATION_ModeCheck(s_Operation, 2);
        OPERATION_Alram(s_Operation, 2);
        OPERATION_Run(s_Operation, 2);

        //channel 4
        if ((ui_UpdateSTEPCPU1[3] != ui_UpdateSTEPCPU2[3]) && (s_Operation[3].VARIABLE_StartFlag == CLEAR))//ui_UpdateflagCPU2++ 증가형임.
        {
            s_Operation[3].VARIABLE_UpdateData++;
            COMMUNICATION_STEPRead(s_Operation, SCHEDULE_DATA, CELL_SCAN_DATA, 3, 0);

            ui_UpdateSTEPCPU1[3] = ui_UpdateSTEPCPU2[3];
        }

        COMMUNICATION_ENDWrite(SCHEDULE_DATA, CELL_SCAN_DATA, s_Operation, 3);

        OPERATION_Sensing(s_Operation, 3);
        //OPERATION_ModeCheck(s_Operation, 3);
        OPERATION_Alram(s_Operation, 3);
        OPERATION_Run(s_Operation, 3);
#endif

        //For loop time measurement
        //GpioDataRegs.GPDSET.bit.GPIO99 = 1;
        //DELAY_US(100);
    }
 }

void PHOENIXON_CONTROLS_Funtion(Uint16 channel)
{

}

//
// End of file
//

