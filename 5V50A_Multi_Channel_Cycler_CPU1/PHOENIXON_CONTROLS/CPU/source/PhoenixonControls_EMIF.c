/*
 * PhoenixonControls_EMIF.c
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(DSP_EmifRead, ".TI.ramfunc");
#pragma CODE_SECTION(DSP_EmifWrite, ".TI.ramfunc");

extern void setup_emif1_pinmux_async_16bit(Uint16 cpu_sel);
extern void setup_emif1_pinmux_sdram_16bit(Uint16 cpu_sel);

void DSP_EmifInit(void)
{
	   EMIF_AsyncTimingParams tparam;
	   //
	    // Configure to run EMIF1 on full Rate. (EMIF1CLK = CPU1SYSCLK)
	    //
	    SysCtl_setEMIF1ClockDivider(SYSCTL_EMIF1CLK_DIV_1);
//	    SysCtl_setEMIF1ClockDivider(SYSCTL_EMIF1CLK_DIV_2);
	    //
	    // Grab EMIF1 For CPU1.
	    //
	    EMIF_selectMaster(EMIF1CONFIG_BASE, EMIF_MASTER_CPU1_G);

	    //
	    // Disable Access Protection. (CPU_FETCH/CPU_WR/DMA_WR)
	    //
	    EMIF_setAccessProtection(EMIF1CONFIG_BASE, 0x0);

	    //
	    // Commit the configuration related to protection. Till this bit remains
	    // set, contents of EMIF1ACCPROT0 register can't be changed.
	    //
	    EMIF_commitAccessConfig(EMIF1CONFIG_BASE);

	    //
	    // Lock the configuration so that EMIF1COMMIT register can't be changed
	    // any more.
	    //
	    EMIF_lockAccessConfig(EMIF1CONFIG_BASE);

//	    //
//	    // Configure GPIO pins for EMIF1.
//	    //
//	    setupEMIF1PinmuxAsync16Bit();

	    //
	    // Configures Normal Asynchronous Mode of Operation.
	    //
	    EMIF_setAsyncMode(EMIF1_BASE, EMIF_ASYNC_CS2_OFFSET,
	                      EMIF_ASYNC_NORMAL_MODE);

	    //
	    // Disables Extended Wait Mode.
	    //
	    EMIF_disableAsyncExtendedWait(EMIF1_BASE, EMIF_ASYNC_CS2_OFFSET);

	    //
	    // Configure EMIF1 Data Bus Width.
	    //
	    EMIF_setAsyncDataBusWidth(EMIF1_BASE, EMIF_ASYNC_CS2_OFFSET,
	                              EMIF_ASYNC_DATA_WIDTH_16);

	    //
	    // Configure the access timing for CS2 space.
	    //
//	    tparam.rSetup = 0;
//	    tparam.rStrobe = 3;
//	    tparam.rHold = 0;
//	    tparam.turnArnd = 0;
//	    tparam.wSetup = 0;
//	    tparam.wStrobe = 1;
//	    tparam.wHold = 0;

//	    tparam.rSetup 	= 4;		//Max 16
//	    tparam.rStrobe 	= 6;	    //Max 64
//	    tparam.rHold 	= 4;    	//Max 8
//	    tparam.turnArnd = 4;    	//Max 4
//	    tparam.wSetup 	= 6;      	//Max 16
//	    tparam.wStrobe 	= 10;    	//Max 64
//	    tparam.wHold 	= 4;       	//Max 8

	    tparam.rSetup 	= 15;		//Max 16
	    tparam.rStrobe 	= 6;	    //Max 64
	    tparam.rHold 	= 4;    	//Max 8
	    tparam.turnArnd = 4;    	//Max 4
	    tparam.wSetup 	= 15;      	//Max 16
	    tparam.wStrobe 	= 10;    	//Max 64
	    tparam.wHold 	= 4;       	//Max 8

	    EMIF_setAsyncTimingParams(EMIF1_BASE, EMIF_ASYNC_CS2_OFFSET, &tparam);

}

void setupEMIF1PinmuxAsync16Bit()
{
    Uint16 i;
    Uint16 cpu_sel = 0;

    for (i=28; i<=52; i++)
    {
        if ((i != 42) && (i != 43))
        {
            GPIO_SetupPinMux(i,cpu_sel,2);
        }
    }
    for (i=63; i<=87; i++)
    {
        if (i != 84)
        {
            GPIO_SetupPinMux(i,cpu_sel,2);
        }
    }

    GPIO_SetupPinMux(88,cpu_sel,3);
    GPIO_SetupPinMux(89,cpu_sel,3);
    GPIO_SetupPinMux(90,cpu_sel,3);
    GPIO_SetupPinMux(91,cpu_sel,3);
    GPIO_SetupPinMux(92,cpu_sel,3);
    GPIO_SetupPinMux(93,cpu_sel,3);
    GPIO_SetupPinMux(94,cpu_sel,2);

    //
    //setup async mode and enable pull-ups for Data pins
    //
    for (i=69; i<=85; i++)
    {
        if (i != 84)
        {
            GPIO_SetupPinOptions(i,0,0x31); // GPIO_ASYNC||GPIO_PULLUP
        }
    }
}

//
// setup_emif1_pinmux_async_32bit - Setup pinmux for 32bit async
//
void setupEMIF1PinmuxSync16Bit()
{
    Uint16 i;
    Uint16 cpu_sel = 0;
    for (i=28; i<=87; i++)
    {
        if ((i != 42) && (i != 43) && (i != 84))
        {
            GPIO_SetupPinMux(i,cpu_sel,2);
        }
    }

    GPIO_SetupPinMux(88,cpu_sel,3);
    GPIO_SetupPinMux(89,cpu_sel,3);
    GPIO_SetupPinMux(90,cpu_sel,3);
    GPIO_SetupPinMux(91,cpu_sel,3);
    GPIO_SetupPinMux(92,cpu_sel,3);
    GPIO_SetupPinMux(93,cpu_sel,3);
    GPIO_SetupPinMux(94,cpu_sel,2);

    //
    //setup async mode for Data pins
    //
    for (i=53; i<=85; i++)
    {
        if (i != 84)
        {
            GPIO_SetupPinOptions(i,0,0x31);
        }
    }
}





Uint16 DSP_EmifRead(Uint32 ul_Address, Uint16* ui_Data)
{

	if(ul_Address < EMIF_CS2_BASE_ADDRESS ||
	   ul_Address > EMIF_CS2_END_ADDRESS)
	{
		return(FAIL); //SUCCESS Or FAIL
	}

	*ui_Data = *((Uint32 *)(ul_Address));

	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 DSP_EmifWrite(Uint32 ul_Address, Uint16 ui_Data)
{

	if(ul_Address < EMIF_CS2_BASE_ADDRESS ||
	   ul_Address > EMIF_CS2_END_ADDRESS)
	{
		return(FAIL); //SUCCESS Or FAIL
	}

	(*(Uint16 *)(ul_Address)) = ui_Data;
	return(SUCCESS);
}

//
// End of file
//

