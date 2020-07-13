/*
 *  PhoenixonControls
 *  HW2팀
 *  MiniCell 충방전기
 *
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(AD7606_Conversion, ".TI.ramfunc");
#pragma CODE_SECTION(AD7606_BusyCheck, ".TI.ramfunc");
#pragma CODE_SECTION(AD7606_OverSamplingSelect, ".TI.ramfunc");
#pragma CODE_SECTION(AD7606_VoltageRangeSelect, ".TI.ramfunc");
#pragma CODE_SECTION(AD7606_Read, ".TI.ramfunc");

void AD7606_Init(void)
{
	AD7606_Reset();
}


void AD7606_Reset(void)
{
    DSP_GPIO90_ADC_RESET_SET;
	asm(" RPT #10 || NOP");//60ns, Delay : 5ns * (10 + 2) = 60ns Max 200
	DSP_GPIO90_ADC_RESET_CLEAR;
}


Uint16 AD7606_Conversion(void)
{
    if(AD7606_BusyCheck())
    {
        AD7606_CONVERSION
        if(AD7606_BusyCheck()){
            return(SUCCESS); //SUCCESS Or FAIL
        }
    }
    return(FAIL); //SUCCESS Or FAIL
}


Uint16 AD7606_BusyCheck(void)
{
    Uint32 ul_TimeOut1 = 200000;
    asm(" RPT #10 || NOP");//60ns, Delay : 5ns * (10 + 2) = 60ns Max 200

    while(ul_TimeOut1--)
    {
        if(!DSP_GPIO33_ADC_BUSY_DATA)break;
    }
    if(ul_TimeOut1){
        return(SUCCESS); //SUCCESS Or FAIL
    }
    else{
        return(FAIL); //SUCCESS Or FAIL
    }
}


Uint16 AD7606_OverSamplingSelect(E_AD7606_OVER_SAMPLING_SELECT e_OverSampling)
{
    static E_AD7606_OVER_SAMPLING_SELECT e_OverSampling_Old = AD7606_OS_NON;

    if(e_OverSampling_Old != e_OverSampling)
    if(AD7606_BusyCheck())
    {
        if(e_OverSampling & 0x1){
            DSP_GPIO49_ADC_OS0_SET
        }
        else{
            DSP_GPIO49_ADC_OS0_CLEAR
        }

        if(e_OverSampling & 0x2){
            DSP_GPIO48_ADC_OS1_SET
        }
        else{
            DSP_GPIO48_ADC_OS1_CLEAR
        }

        if(e_OverSampling & 0x4){
            DSP_GPIO36_ADC_OS2_SET
        }
        else{
            DSP_GPIO36_ADC_OS2_CLEAR
        }
        e_OverSampling_Old = e_OverSampling;
    }
    else
    {
        return(FAIL); //SUCCESS Or FAIL
    }
    asm(" RPT #10 || NOP");//60ns, Delay : 60ns * (10 + 2) = 60ns Max 200
    return(SUCCESS); //SUCCESS Or FAIL
}


Uint16 AD7606_VoltageRangeSelect(E_AD7606_VOLTAGE_RANGE_SELECT e_Voltage)
{
    static E_AD7606_VOLTAGE_RANGE_SELECT e_Voltage_Old = AD7606_VR_NON;

    if(e_Voltage_Old != e_Voltage){
        if(AD7606_BusyCheck())
        {
            if(e_Voltage == AD7606_05_VOLTAGE){
                DSP_GPIO21_ADC_RANGE_CLEAR
            }
            else if(e_Voltage == AD7606_10_VOLTAGE){
                DSP_GPIO21_ADC_RANGE_SET
            }
            else{
                DSP_GPIO21_ADC_RANGE_CLEAR
            }
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


// VSENSE와 션트 전류 측정용 ADC
Uint16 AD7606_Read(E_AD7606_VOLTAGE_RANGE_SELECT e_Voltage, E_AD7606_OVER_SAMPLING_SELECT e_OverSampling, float *Data)
{
    Uint16 ui_i = 0;
    Uint16 ui_Byte = 0;
    int32 l_Temp = 0;

    AD7606_VoltageRangeSelect(e_Voltage);
    AD7606_OverSamplingSelect(e_OverSampling);

    if(AD7606_Conversion())
    {
        for(ui_i = 0; ui_i < 8; ui_i++)//Read Data
        {
            AD7606_READ_ADC(&ui_Byte)

            asm(" RPT #4 || NOP");

            if(ui_Byte & 0x8000){       // bipolar 16bit ADC에서 최대값(2^15)
                l_Temp = ( 0xFFFF0000 |  ui_Byte );     // 보수 변환(?), 보수 표현(?)
            }
            else    l_Temp = (0x0000 | ui_Byte);

            Data[ui_i] = (float)(l_Temp)
            *(float)(e_Voltage == AD7606_10_VOLTAGE ? AD7606_GAIN_10V : AD7606_GAIN_5V);
        }
    }
    else
    {
        return(FAIL); //SUCCESS Or FAIL
    }
    return(SUCCESS); //SUCCESS Or FAIL
}

//
// End of file
//
