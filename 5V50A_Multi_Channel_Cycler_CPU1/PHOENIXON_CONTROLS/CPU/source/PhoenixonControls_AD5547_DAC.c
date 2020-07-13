/*
 *  PhoenixonControls
 *  HW2ÆÀ
 *  MiniCell Ãæ¹æÀü±â
 *
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(AD5547_OutputSelect, ".TI.ramfunc");
//#pragma CODE_SECTION(AD5547_Write, ".TI.ramfunc");

void AD5547_Init(void)
{
	AD5547_Reset();
}

Uint16 AD5547_Reset(void)
{
    DSP_GPIO86_DAC_NRESET_SET
    asm(" RPT #10 || NOP");//60ns, Delay : 60ns * (10 + 2) = 60ns Max 200
    DSP_GPIO86_DAC_NRESET_CLEAR

    AD5547_OutputSelect(AD5547_NON_OUTPUT);

    return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 AD5547_OutputSelect(E_AD5547_OUTPUT_SELECT e_Port)
{
	static E_AD5547_OUTPUT_SELECT e_Port_Old = AD5547_NON_OUTPUT;

	if(e_Port_Old != e_Port)
	switch(e_Port)
	{
    case AD5547_A:
        DSP_GPIO89_DAC_A0_CLEAR
        DSP_GPIO88_DAC_A1_CLEAR
        break;
    case AD5547_NON_OUTPUT:
        DSP_GPIO89_DAC_A0_SET
        DSP_GPIO88_DAC_A1_CLEAR
        break;
    case AD5547_ALL_OUTPUT:
        DSP_GPIO89_DAC_A0_CLEAR
        DSP_GPIO88_DAC_A1_SET
        break;
    case AD5547_B:
        DSP_GPIO89_DAC_A0_SET
        DSP_GPIO88_DAC_A1_SET
		break;
	default : break;
	}
	e_Port_Old = e_Port;

	return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 AD5547_Write_VREF(Uint16 channel, Uint16 ui_Data)
{
	switch(channel)
	{
	//-----------VREF 1~4---------------------------------------------
	case 0:
		AD5547_OutputSelect(AD5547_A);
		AD5547_VREF_1_2_WIRTE_DATA(ui_Data);
		AD5547_VREF_1_2_LDAC;
		break;
	case 1:
		AD5547_OutputSelect(AD5547_B);
		AD5547_VREF_1_2_WIRTE_DATA(ui_Data);
		AD5547_VREF_1_2_LDAC;
		break;
	case 2:
		AD5547_OutputSelect(AD5547_A);
		AD5547_VREF_3_4_WIRTE_DATA(ui_Data);
		AD5547_VREF_3_4_LDAC;
		break;
	case 3:
		AD5547_OutputSelect(AD5547_B);
		AD5547_VREF_3_4_WIRTE_DATA(ui_Data);
		AD5547_VREF_3_4_LDAC;
		break;

	default : break;
	}

	DELAY_US(1);//AD5547   Output Voltage Settling Time Max 0.5us

	return(SUCCESS); //SUCCESS Or FAIL
}


Uint16 AD5547_Write_VINT(Uint16 channel, Uint16 ui_Data)
{
    switch(channel)
    {

    //-----------VINT 1~4---------------------------------------------
    case 0:
        //g_flow_Check.A10_AD5547_Write_VINT_Check = 1;
        AD5547_OutputSelect(AD5547_A);
        AD5547_VINT_1_2_WIRTE_DATA(ui_Data);
        AD5547_VINT_1_2_LDAC;
        break;
    case 1:
        AD5547_OutputSelect(AD5547_B);
        AD5547_VINT_1_2_WIRTE_DATA(ui_Data);
        AD5547_VINT_1_2_LDAC;
        break;
    case 2:
        AD5547_OutputSelect(AD5547_A);
        AD5547_VINT_3_4_WIRTE_DATA(ui_Data);
        AD5547_VINT_3_4_LDAC;
        break;
    case 3:
        AD5547_OutputSelect(AD5547_B);
        AD5547_VINT_3_4_WIRTE_DATA(ui_Data);
        AD5547_VINT_3_4_LDAC;
        break;

    default : break;
    }

    DELAY_US(1);//AD5547   Output Voltage Settling Time Max 0.5us

    return(SUCCESS); //SUCCESS Or FAIL
}


Uint16 AD5547_Write_IREF(Uint16 channel, Uint16 ui_Data)
{
    switch(channel)
    {

    //-----------IREF 1~4---------------------------------------------
    case 0:
        AD5547_OutputSelect(AD5547_A);
        AD5547_IREF_1_2_WIRTE_DATA(ui_Data);
        AD5547_IREF_1_2_LDAC;
        break;
    case 1:
        AD5547_OutputSelect(AD5547_B);
        AD5547_IREF_1_2_WIRTE_DATA(ui_Data);
        AD5547_IREF_1_2_LDAC;
        break;
    case 2:
        AD5547_OutputSelect(AD5547_A);
        AD5547_IREF_3_4_WIRTE_DATA(ui_Data);
        AD5547_IREF_3_4_LDAC;
        break;
    case 3:
        AD5547_OutputSelect(AD5547_B);
        AD5547_IREF_3_4_WIRTE_DATA(ui_Data);
        AD5547_IREF_3_4_LDAC;
        break;

    default : break;
    }

    DELAY_US(1);//AD5547   Output Voltage Settling Time Max 0.5us

    return(SUCCESS); //SUCCESS Or FAIL
}

//
// End of file
//

