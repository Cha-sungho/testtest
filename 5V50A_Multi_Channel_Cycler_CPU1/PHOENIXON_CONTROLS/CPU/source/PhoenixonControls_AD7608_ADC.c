/*
 * PhoenixonControls_AD7608_ADC.c
 *
 *  Created on: 2020. 3. 30.
 *      Author: HW2
 */


#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(AD7608_Conversion, ".TI.ramfunc");
//#pragma CODE_SECTION(AD7608_FastDataCheck, ".TI.ramfunc");
#pragma CODE_SECTION(AD7608_BusyCheck, ".TI.ramfunc");
#pragma CODE_SECTION(AD7608_OverSamplingSelect, ".TI.ramfunc");
#pragma CODE_SECTION(AD7608_VoltageRangeSelect, ".TI.ramfunc");
#pragma CODE_SECTION(AD7608_Read, ".TI.ramfunc");

void AD7608_Init(void)
{
    AD7608_Reset();
}

void AD7608_Reset(void)
{
    DSP_GPIO90_ADC_RESET_SET
    asm(" RPT #10 || NOP");//60ns, Delay : 5ns * (10 + 2) = 60ns Max 200
    DSP_GPIO90_ADC_RESET_CLEAR
}

Uint16 AD7608_BusyCheck(E_AD7608_ADC_SELECT e_Channel)
{
    Uint32 ul_TimeOut = 200000;

    asm(" RPT #10 || NOP");//60ns, Delay : 5ns * (10 + 2) = 60ns Max 200
    while(ul_TimeOut--)
    {
        if (e_Channel == AD7608)
        {
            if (!DSP_GPIO33_ADC_BUSY_DATA) break;
        }
        else if (e_Channel == AD7608_LOW_CURRENT)
        {
            if (!DSP_GPIO2_ADC_BUSY2_DATA) break;
        }
    }
    if(ul_TimeOut)return(SUCCESS); //SUCCESS Or FAIL
    else return(FAIL); //SUCCESS Or FAIL
}

Uint16 AD7608_Conversion(E_AD7608_ADC_SELECT e_Channel)
{
    if (e_Channel == AD7608)
    {
        if(AD7608_BusyCheck(AD7608))
        {
            AD7608A_CONVERSION
            if(AD7608_BusyCheck(AD7608))
            {
                return(SUCCESS); //SUCCESS Or FAIL
            }
        }
    }
    else if (e_Channel == AD7608_LOW_CURRENT)
    {
        if(AD7608_BusyCheck(AD7608_LOW_CURRENT))
        {
            AD7608B_CONVERSION
            if(AD7608_BusyCheck(AD7608))
            {
                return(SUCCESS); //SUCCESS Or FAIL
            }
        }
    }
    return(FAIL); //SUCCESS Or FAIL
}

Uint16 AD7608_OverSamplingSelect(E_AD7608_OVER_SAMPLING_SELECT e_OverSampling)
{
    volatile static E_AD7608_OVER_SAMPLING_SELECT e_OverSampling_Old = AD7608_OS_NON;

    if(e_OverSampling_Old != e_OverSampling)
    {
        if(AD7608_BusyCheck(AD7608) && AD7608_BusyCheck(AD7608_LOW_CURRENT))
        {
            if(e_OverSampling & 0x1)DSP_GPIO49_ADC_OS0_SET
            else DSP_GPIO49_ADC_OS0_CLEAR

            if(e_OverSampling & 0x2)DSP_GPIO48_ADC_OS1_SET
            else DSP_GPIO48_ADC_OS1_CLEAR

            if(e_OverSampling & 0x4)DSP_GPIO36_ADC_OS2_SET
            else DSP_GPIO36_ADC_OS2_CLEAR

            e_OverSampling_Old = e_OverSampling;
        }
        else
        {
            return(FAIL); //SUCCESS Or FAIL
        }
    }
    asm(" RPT #10 || NOP");//60ns, Delay : 60ns * (10 + 2) = 60ns Max 200
    return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 AD7608_VoltageRangeSelect(E_AD7608_VOLTAGE_RANGE_SELECT e_Voltage)
{
    volatile static E_AD7608_VOLTAGE_RANGE_SELECT e_Voltage_Old = AD7608_VR_NON;

    if(e_Voltage_Old != e_Voltage)
    {
        if(AD7608_BusyCheck(AD7608) && AD7608_BusyCheck(AD7608_LOW_CURRENT))
        {
            if(e_Voltage == AD7608_05_VOLTAGE)DSP_GPIO21_ADC_RANGE_CLEAR
            else if(e_Voltage == AD7608_10_VOLTAGE)DSP_GPIO21_ADC_RANGE_SET
            else DSP_GPIO21_ADC_RANGE_CLEAR

            asm(" RPT #10 || NOP");//60ns, Delay : 60ns * (10 + 2) = 60ns Max 200
            e_Voltage_Old = e_Voltage;
        }
        else
        {
            return(FAIL); //SUCCESS Or FAIL
        }
    }
    return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 AD7608_Read(E_AD7608_ADC_SELECT e_Channel, E_AD7608_VOLTAGE_RANGE_SELECT e_Voltage, E_AD7608_OVER_SAMPLING_SELECT e_OverSampling, float32 *Data)
{
    Uint16 ui_i = 0;
    Uint16 ui_HighByte = 0;
    Uint16 ui_LowByte = 0;
    int32 l_Temp = 0;

    AD7608_VoltageRangeSelect(e_Voltage);
    AD7608_OverSamplingSelect(e_OverSampling);

    if(e_Channel == AD7608)
    {
        if(AD7608_Conversion(AD7608))
        {
            DSP_GPIO19_ADC_N_CS_SET
            for(ui_i = 0; ui_i < 8; ui_i++)//Read Data
            {
                AD7608_READ_ADC(&ui_HighByte)
                AD7608_READ_ADC(&ui_LowByte)
                l_Temp = (int32)(ui_HighByte);
                l_Temp <<= 2;

                ui_LowByte >>= 14;
                ui_LowByte &= 0x03;
                l_Temp |= ui_LowByte;

                if(l_Temp & 0x20000)l_Temp |= 0xFFFC0000;

                Data[ui_i] = (float32)(l_Temp) * (float32)(e_Voltage == AD7608_10_VOLTAGE ? AD7608_GAIN_10V : AD7608_GAIN_5V);
            }
            DSP_GPIO19_ADC_N_CS_CLEAR
        }
        else
        {
            return(FAIL); //SUCCESS Or FAIL
        }
    }
    else if (e_Channel == AD7608_LOW_CURRENT)
    {
        if(AD7608_Conversion(AD7608_LOW_CURRENT))  //DL 2019-07-01
        {
            DSP_GPIO1_ADC_N_CS2_SET
            for(ui_i = 0; ui_i < 8; ui_i++)//Read Data
            {
                AD7608_READ_ADC2(&ui_HighByte)
                AD7608_READ_ADC2(&ui_LowByte)
                l_Temp = (int32)(ui_HighByte);
                l_Temp <<= 2;

                ui_LowByte >>= 14;
                ui_LowByte &= 0x03;
                l_Temp |= ui_LowByte;

                if(l_Temp & 0x20000)l_Temp |= 0xFFFC0000;

                Data[ui_i] = (float32)(l_Temp) * (float32)(e_Voltage == AD7608_10_VOLTAGE ? AD7608_GAIN_10V : AD7608_GAIN_5V);
            }
            DSP_GPIO1_ADC_N_CS2_CLEAR
        }
        else
        {
            return(FAIL); //SUCCESS Or FAIL
        }
    }
    return(SUCCESS); //SUCCESS Or FAIL
}

