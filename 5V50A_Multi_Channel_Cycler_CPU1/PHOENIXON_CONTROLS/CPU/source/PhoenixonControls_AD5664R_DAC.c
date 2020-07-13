/*
 * PhoenixonControls_AD5664R_DAC.c
 *
 *  Created on: 2020. 3. 30.
 *      Author: CHA
 */

#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"


void AD5664R_Init(void)
{
    AD5664R_Reset();
}

void AD5664R_Reset(void)
{
    AD5664R_Write(AD5664R_ALL, AD5664R_COMMAND_RESET, AD5664R_RESET_ALL_REGISTER);
    DELAY_US(1);
}

void AD5664R_Internal_Reference_Setup(void)
{
    AD5664R_Write(AD5664R_ALL, AD5664R_COMMAND_INTERNAL_REF_SETUP, AD5664R_INTERNAL_REFERENCE_ON);
    DELAY_US(1);
}

//void I_Fine_DAC(E_AD5664R_CHANNEL_SELECT channel, float f_VolatageValue)
//{
//    float f_Temp = 0.0;
//    float f_Value = 0.0;
//    Uint16 ui_Temp = 0;
//
//    f_Value = f_VolatageValue;
//
////    if (f_OldValue != f_Value)
////    {
////        f_Temp = f_Value * AD5644R_RESOLUTION / (2 * AD5644R_INTERNAL_REFERENCE_VOLTAGE );
//        if(f_Value <= 0.0) f_Value = 0.0;
//        f_Temp = f_Value * AD5644R_RESOLUTION / (AD5644R_EXTERNAL_REFERENCE_VOLTAGE );
//        ui_Temp = (Uint16) (f_Temp);
//
//        switch (channel)
//        {
//        case AD5644R_CH1:
//            AD5644R_Write(AD5644R_CH1, AD5644R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
//            break;
//        case AD5644R_CH2:
//            AD5644R_Write(AD5644R_CH2, AD5644R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
//            break;
//        case AD5644R_CH3:
//            AD5644R_Write(AD5644R_CH3, AD5644R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
//            break;
//        case AD5644R_CH4:
//            AD5644R_Write(AD5644R_CH4, AD5644R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
//            break;
//        case AD5644R_ALL:
//            AD5644R_Write(AD5644R_ALL, AD5644R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
//            break;
//        }
//
////    }
//}

//void AD5644R_Write(E_AD5664R_CHANNEL_SELECT Channel, Uint16 Command, Uint16 Data)
//{
//    Uint16 ui_HighTemp = 0;
//    Uint16 ui_MidTemp = 0;
//    Uint16 ui_LowTemp = 0;
//
//    ui_HighTemp = ((Command & 0x7) << 3) + (Channel & 0x7);
//    ui_MidTemp = (Data & 0x3FFF) >> 6;
//    ui_LowTemp = (Data & 0x3FFF);
//
//    DSP_GPIO20_IFINE_DAC_N_SYNC_SET
//    DELAY_US(1);                // 13ns min SYNC to SCLK falling edge setup time
//    SPI_Transmit(ui_HighTemp);
//    SPI_Transmit(ui_MidTemp);
//    SPI_Transmit(ui_LowTemp);
////    DELAY_US(1);                // 0ns min SCLK falling edge to SYNC rising edge
//    DSP_GPIO20_IFINE_DAC_N_SYNC_CLEAR
//    DELAY_US(1);                // 15ns min Minimum SYNC high time
//
//}

void AD5664R_Write(E_AD5664R_CHANNEL_SELECT Channel, Uint16 Command, Uint16 Data)
{
    Uint16 ui_HighTemp = 0;
    Uint16 ui_MidTemp = 0;
    Uint16 ui_LowTemp = 0;

    ui_HighTemp = ((Command & 0x7) << 3) + (Channel & 0x7);
    ui_MidTemp = (Data & 0xFFFF) >> 8;
    ui_LowTemp = (Data & 0xFFFF);

    DSP_GPIO20_IFINE_DAC_N_SYNC_SET
    DELAY_US(1);                // 13ns min SYNC to SCLK falling edge setup time
    SPI_Transmit(ui_HighTemp);
    SPI_Transmit(ui_MidTemp);
    SPI_Transmit(ui_LowTemp);
//    DELAY_US(1);                // 0ns min SCLK falling edge to SYNC rising edge
    DSP_GPIO20_IFINE_DAC_N_SYNC_CLEAR
    DELAY_US(1);                // 15ns min Minimum SYNC high time

}

void Init_SPI2(void)
{

    //
    // Initialize core SPI registers
    //

    // Set reset low before configuration changes
    // Clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Disable loop-back
    SpibRegs.SPICCR.bit.SPISWRESET = 0;
    SpibRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpibRegs.SPICCR.bit.SPICHAR = (8 - 1);     // Word length - 1
    SpibRegs.SPICCR.bit.SPILBK = 0;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are disabled
    SpibRegs.SPICTL.bit.MASTER_SLAVE = 1;
    SpibRegs.SPICTL.bit.TALK = 1;
    SpibRegs.SPICTL.bit.CLK_PHASE = 0;
    SpibRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate
    SpibRegs.SPIBRR.bit.SPI_BIT_RATE = SPI2_BAUDRATE;

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpibRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpibRegs.SPICCR.bit.SPISWRESET = 1;
}

void SPI_Transmit(Uint16 data)
{
    volatile Uint16 dummy;
    Uint16 sdata;

    sdata = (data << 8) & 0xFF00;
    SpibRegs.SPICTL.bit.TALK = 1;   // Enable Transmit path
    SpibRegs.SPITXBUF = sdata;      // Master transmits data
    while(SpibRegs.SPISTS.bit.INT_FLAG != 1) {} // Waits until data transmit
    dummy = SpibRegs.SPIRXBUF;      // Clears INT_FLAG
}

