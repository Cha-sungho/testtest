//###########################################################################
//
// FILE:    F2837xD_McBSP.c
//
// TITLE:   F2837xD Device McBSP Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2837xD Support Library v3.04.00.00 $
// $Release Date: Sun Mar 25 13:26:04 CDT 2018 $
// $Copyright:
// Copyright (C) 2013-2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

//
// Included Files
//
#include "../../headers/include/F2837xD_device.h"
#include "../include/F2837xD_Examples.h"

//
// MCBSP_INIT_DELAY determines the amount of CPU cycles in the 2 sample rate
// generator (SRG) cycles required for the Mcbsp initialization routine.
// MCBSP_CLKG_DELAY determines the amount of CPU cycles in the 2 clock
// generator (CLKG) cycles required for the Mcbsp initialization routine.
//

//
// Defines
//
#define CPU_SPD              200E6
#define MCBSP_SRG_FREQ       CPU_SPD/4   // SRG input is LSPCLK (SYSCLKOUT/4)
                                         // for examples
#define CLKGDV_VAL           1

// # of CPU cycles in 2 SRG cycles-init delay
#define MCBSP_INIT_DELAY     2*(CPU_SPD/MCBSP_SRG_FREQ)

// # of CPU cycles in 2 CLKG cycles-init delay
#define MCBSP_CLKG_DELAY     2*(CPU_SPD/(MCBSP_SRG_FREQ/(1+CLKGDV_VAL)))

//
// Function Prototypes
//
void delay_loop(void);      // Delay function used for SRG initialization
void clkg_delay_loop(void); // Delay function used for CLKG initialization

//
// InitMcbsp - This function initializes the McBSP to a known state.
//
void InitMcbspa(void)
{
    //
    // Reset the McBSP
    // Disable all interrupts
    // Frame sync generator reset
    // Sample rate generator reset
    // Transmitter reset
    // Receiver reset
    //
    McbspaRegs.SPCR2.bit.FRST = 0;
    McbspaRegs.SPCR2.bit.GRST = 0;
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;

    //
    // Enable loop back mode
    // This does not require external hardware
    //
    McbspaRegs.SPCR2.all = 0x0000;
    McbspaRegs.SPCR1.all = 0x8000;

    //
    // RX data delay is 1 bit
    // TX data delay is 1 bit
    //
    McbspaRegs.RCR2.bit.RDATDLY = 1;
    McbspaRegs.XCR2.bit.XDATDLY = 1;

    //
    // No clock sync for CLKG
    // Frame-synchronization period
    //
    McbspaRegs.SRGR2.bit.GSYNC = 0;
    McbspaRegs.SRGR2.bit.FPER = 320;

    //
    // Frame-synchronization pulses from
    // the sample rate generator
    //
    McbspaRegs.SRGR2.bit.FSGM = 1;

    //
    // Sample rate generator input clock is LSPCLK
    //
    McbspaRegs.SRGR2.bit.CLKSM = 1;
    McbspaRegs.PCR.bit.SCLKME = 0;

    //
    // Divide-down value for CLKG
    // Frame-synchronization pulse width
    //
    McbspaRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;
    clkg_delay_loop();
    McbspaRegs.SRGR1.bit.FWID = 1;

    //
    // CLKX is driven by the sample rate generator
    // Transmit frame synchronization generated by internal
    // sample rate generator
    //
    McbspaRegs.PCR.bit.CLKXM = 1;
    McbspaRegs.PCR.bit.FSXM = 1;

    //
    // Enable Sample rate generator and
    // wait at least 2 CLKG clock cycles
    //
    McbspaRegs.SPCR2.bit.GRST = 1;
    clkg_delay_loop();

    //
    // Release from reset
    // RX, TX and frame sync generator
    //
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
    McbspaRegs.SPCR2.bit.FRST = 1;
}

//
// InitMcbspaInt - Enable TX and RX interrupts
//
void InitMcbspaInt(void)
{
    // Reset TX and RX
    // Enable interrupts for TX and RX
    // Release TX and RX
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;
    McbspaRegs.MFFINT.bit.XINT = 1;
    McbspaRegs.MFFINT.bit.RINT = 1;
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;
}

//
// InitMcbspa8bit - McBSP uses an 8-bit word for both TX and RX
//
void InitMcbspa8bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 0;
    McbspaRegs.XCR1.bit.XWDLEN1 = 0;
}

//
// InitMcbspa12bit - McBSP uses an 12-bit word for both TX and RX
//
void InitMcbspa12bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 1;
    McbspaRegs.XCR1.bit.XWDLEN1 = 1;
}

//
// InitMcbspa16bit - McBSP uses an 16-bit word for both TX and RX
//
void InitMcbspa16bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 2;
    McbspaRegs.XCR1.bit.XWDLEN1 = 2;
}

//
// InitMcbspa20bit - McBSP uses an 20-bit word for both TX and RX
//
void InitMcbspa20bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 3;
    McbspaRegs.XCR1.bit.XWDLEN1 = 3;
}

//
// InitMcbspa24bit - McBSP uses an 24-bit word for both TX and RX
//
void InitMcbspa24bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 4;
    McbspaRegs.XCR1.bit.XWDLEN1 = 4;
}

//
// InitMcbspa32bit - McBSP uses an 32-bit word for both TX and RX
//
void InitMcbspa32bit(void)
{
    McbspaRegs.RCR1.bit.RWDLEN1 = 5;
    McbspaRegs.XCR1.bit.XWDLEN1 = 5;
}

//
// InitMcbspaGpio - Assign GPIO pins to the McBSP peripheral
//                 (Note: This function must be called from CPU1.)
//
void InitMcbspaGpio(void)
{
#ifdef CPU1
    EALLOW;

    //
    // This specifies which of the possible GPIO pins will be
    // McBSPA functional pins. Comment out unwanted connections.
    // Set qualification for selected input pins to asynchronous only
    // This will select asynchronous (no qualification) for the selected pins.
    //

    //
    // MDXA
    // GPIO20
    // GPIO84
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 2;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 3;

    //
    // MDRA
    // GPIO21 with asynchronous qualification
    // GPIO85 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 2;
    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 3;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 3;
    //GpioCtrlRegs.GPCQSEL2.bit.GPIO85 = 3;

    //
    // MCLKXA
    // GPIO22 with asynchronous qualification
    // GPIO86 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 2;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO22 = 3;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO86 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 3;
    //GpioCtrlRegs.GPCQSEL2.bit.GPIO86 = 3;

    //
    // MCLKRA
    // Select one of the following
    // GPIO7 with asynchronous qualification
    // GPIO58 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;
    //GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3;

    //
    // MFSXA
    // GPIO23 with asynchronous qualification
    // GPIO87 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX2.bit.GPIO23 = 2;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO23 = 3;
    //GpioCtrlRegs.GPCGMUX2.bit.GPIO87 = 3;
    //GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 3;
    //GpioCtrlRegs.GPCQSEL2.bit.GPIO87 = 3;

    //
    // MFSRA
    // Select one of the following
    // GPIO5 with asynchronous qualification
    // GPIO59 with asynchronous qualification
    //
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 2;
    GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 3;
    //GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 1;
    //GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3;

    EDIS;
#endif
}

//
// InitMcbspb - McBSPB initialization routine for examples
//
void InitMcbspb(void)
{
    //
    // Reset the McBSP
    // Disable all interrupts
    // Frame sync generator reset
    // Sample rate generator reset
    // Transmitter reset
    // Receiver reset
    //
    McbspbRegs.SPCR2.bit.FRST = 0;
    McbspbRegs.SPCR2.bit.GRST = 0;
    McbspbRegs.SPCR2.bit.XRST = 0;
    McbspbRegs.SPCR1.bit.RRST = 0;

    //
    // Enable loop back mode
    // This does not require external hardware
    //
    McbspbRegs.SPCR2.all = 0x0000;
    McbspbRegs.SPCR1.all = 0x8000;

    //
    // RX data delay is 1 bit
    // TX data delay is 1 bit
    //
    McbspbRegs.RCR2.bit.RDATDLY = 1;
    McbspbRegs.XCR2.bit.XDATDLY = 1;

    //
    // No clock sync for CLKG
    // Frame-synchronization period
    //
    McbspbRegs.SRGR2.bit.GSYNC = 0;
    McbspbRegs.SRGR2.bit.FPER = 320;

    //
    // Frame-synchronization pulses from
    // the sample rate generator
    //
    McbspbRegs.SRGR2.bit.FSGM = 1;

    //
    // Sample rate generator input clock is LSPCLK
    //
    McbspbRegs.SRGR2.bit.CLKSM = 1;
    McbspbRegs.PCR.bit.SCLKME = 0;

    //
    // Divide-down value for CLKG
    // Frame-synchronization pulse width
    //
    McbspbRegs.SRGR1.bit.CLKGDV = CLKGDV_VAL;
    clkg_delay_loop();
    McbspbRegs.SRGR1.bit.FWID = 1;

    //
    // CLKX is driven by the sample rate generator
    // Transmit frame synchronization generated by internal
    // sample rate generator
    //
    McbspbRegs.PCR.bit.CLKXM = 1;
    McbspbRegs.PCR.bit.FSXM = 1;

    //
    // Enable Sample rate generator and
    // wait at least 2 CLKG clock cycles
    //
    McbspbRegs.SPCR2.bit.GRST = 1;
    clkg_delay_loop();

    //
    // Release from reset
    // RX, TX and frame sync generator
    //
    McbspbRegs.SPCR2.bit.XRST = 1;
    McbspbRegs.SPCR1.bit.RRST = 1;
    McbspbRegs.SPCR2.bit.FRST = 1;
}

//
// InitMcbspbInt - Enable TX and RX interrupts
//
void InitMcbspbInt(void)
{
    //
    // Reset TX and RX
    // Enable interrupts for TX and RX
    // Release TX and RX
    //
    McbspbRegs.SPCR2.bit.XRST = 0;
    McbspbRegs.SPCR1.bit.RRST = 0;
    McbspbRegs.MFFINT.bit.XINT = 1;
    McbspbRegs.MFFINT.bit.RINT = 1;
    McbspbRegs.SPCR2.bit.XRST = 1;
    McbspbRegs.SPCR1.bit.RRST = 1;
}

//
// InitMcbspb8bit - McBSPB uses an 8-bit word for both TX and RX
//
void InitMcbspb8bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1 = 0;
    McbspbRegs.XCR1.bit.XWDLEN1 = 0;
}

//
// IniMcbspb12bit - McBSPB uses an 12-bit word for both TX and RX
//
void IniMcbspb12bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1 = 1;
    McbspbRegs.XCR1.bit.XWDLEN1 = 1;
}

//
// InitMcbspb16bit - McBSPB uses an 16-bit word for both TX and RX
//
void InitMcbspb16bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1 = 2;
    McbspbRegs.XCR1.bit.XWDLEN1 = 2;
}

//
// InitMcbspb20bit - McBSPB uses an 20-bit word for both TX and RX
//
void InitMcbspb20bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1 = 3;
    McbspbRegs.XCR1.bit.XWDLEN1 = 3;
}

//
// InitMcbspb24bit - McBSPB uses an 24-bit word for both TX and RX
//
void InitMcbspb24bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1 = 4;
    McbspbRegs.XCR1.bit.XWDLEN1 = 4;
}

//
// InitMcbspb32bit - McBSPB uses an 32-bit word for both TX and RX
//
void InitMcbspb32bit(void)
{
    McbspbRegs.RCR1.bit.RWDLEN1 = 5;
    McbspbRegs.XCR1.bit.XWDLEN1 = 5;
}

//
// InitMcbspbGpio - Assign GPIO pins to the McBSP peripheral
//                 (Note: This function must be called from CPU1.)
//
void InitMcbspbGpio(void)
{
#ifdef CPU1
    EALLOW;

    //
    // This specifies which of the possible GPIO pins will be
    // McBSPB functional pins. Comment out unwanted connections.
    // Set qualification for selected input pins to asynchronous only
    // This will select asynchronous (no qualification) for the selected pins.
    //

    //
    // Select one of the following for MDXB
    // GPIO24
    // GPIO84
    //
    //GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 3;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO84 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO84 = 2;

    //
    // MDRB
    // GPIO13 with asynchronous qualification
    // GPIO25 with asynchronous qualification
    // GPIO85 with asynchronous qualification
    //
    //GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 3;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO13 = 3;
    //GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 3;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 3;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO85 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO85 = 2;
    GpioCtrlRegs.GPCQSEL2.bit.GPIO85 = 3;

    //
    // MCLKXB
    // GPIO14 with asynchronous qualification
    // GPIO26 with asynchronous qualification
    // GPIO86 with asynchronous qualification
    //
    //GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 3;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO14 = 3;
    //GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 3;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 3;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO86 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO86 = 2;
    GpioCtrlRegs.GPCQSEL2.bit.GPIO86= 3;

    //
    // MCLKRB
    // Select one of the following
    // GPIO3 with asynchronous qualification
    // GPIO60 with asynchronous qualification
    //
    //GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 3;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO3 = 3;
    GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 1;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3;

    //
    // MFSXB
    // GPIO15 with asynchronous qualification
    // GPIO27 with asynchronous qualification
    // GPIO87 with asynchronous qualification
    //
    //GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 3;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 3;
    //GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 3;
    //GpioCtrlRegs.GPAQSEL2.bit.GPIO27 = 3;
    GpioCtrlRegs.GPCGMUX2.bit.GPIO87 = 1;
    GpioCtrlRegs.GPCMUX2.bit.GPIO87 = 2;
    GpioCtrlRegs.GPCQSEL2.bit.GPIO87= 3;

    //
    // MFSRB
    // Select one of the following
    // GPIO1 with asynchronous qualification
    // GPIO61 with asynchronous qualification
    //
    //GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 3;
    //GpioCtrlRegs.GPAQSEL1.bit.GPIO1 = 3;
    GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 1;
    GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3;

    EDIS;

#endif
}

//
// delay_loop - Delay function (at least 2 SRG cycles)
//              Required in McBSP initialization
//
void delay_loop(void)
{
    long i;
    for (i = 0; i < MCBSP_INIT_DELAY; i++) {}
}

//
// clkg_delay_loop - Delay function (at least 2 CLKG cycles)
//                   Required in McBSP init
//
void clkg_delay_loop(void)
{
    long i;
    for (i = 0; i < MCBSP_CLKG_DELAY; i++) {}
}

//
// End of file
//
