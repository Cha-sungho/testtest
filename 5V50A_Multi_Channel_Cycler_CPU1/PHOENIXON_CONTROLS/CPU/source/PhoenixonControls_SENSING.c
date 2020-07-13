/*
 * PhoenixonControls_SENSING.c
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(SENSING_Emergency, ".TI.ramfunc");
#pragma CODE_SECTION(SENSING_ADC, ".TI.ramfunc");
#pragma CODE_SECTION(SENSING_ADCResultData, ".TI.ramfunc");


E_SENSING_EMERGENCY SENSING_Emergency(void)
{
    E_SENSING_EMERGENCY e_SensingEmergency = SENSING_EMERGENCY_OFF;

    e_SensingEmergency = (E_SENSING_EMERGENCY)DSP_GPIO50_EMERGENCY_IN_DATA;

    return (e_SensingEmergency);
}

Uint16 SENSING_ADC(S_SENSING_ADC_RESULT_ARRAY_DATA *s_ResultData, E_SENSING_ADC_SOURCE_SELECT e_Source)
{

    if(e_Source == SENSING_AD7608)
        AD7608_Read(AD7608, AD7608_05_VOLTAGE, AD7608_64, &(*s_ResultData).ExternalADC[0]);
    if(e_Source == SENSING_DSP_ADC)
        DSP_ADCRead(&(*s_ResultData).ADC[0]);
    if(e_Source == SENSING_ALL)
    {
        AD7608_Read(AD7608, AD7608_05_VOLTAGE, AD7608_64, &(*s_ResultData).ExternalADC[0]);
        DSP_ADCRead(&(*s_ResultData).ADC[0]);
    }
    return(SUCCESS); //SUCCESS Or FAIL
}

Uint16 SENSING_ADCResultData(S_SENSING_ADC_RESULT_DATA *s_ResultData, E_SENSING_ADC_SOURCE_SELECT e_Source, Uint16 channel)
{
    S_SENSING_ADC_RESULT_ARRAY_DATA s_Data;

    SENSING_ADC(&s_Data, e_Source);

    (*s_ResultData).SENSING_SMPS_Positive_7R5V_Power    = s_Data.ADC[SENSING_SMPS_POSITIVE_8R5V_POWER] * SENSING_POSITIVE_07V_POWER_GAIN;
    (*s_ResultData).SENSING_SMPS_Negative_0V_Power      = s_Data.ADC[SENSING_SMPS_NEGATIVE_0V_POWER] * SENSING_NEGATIVE_02V_POWER_GAIN;
    (*s_ResultData).SENSING_M_Positive_15V_Power        = s_Data.ADC[SENSING_M_POSITIVE_15V_POWER] * SENSING_POSITIVE_15V_POWER_GAIN;
    (*s_ResultData).SENSING_M_Negative_15V_Power        = s_Data.ADC[SENSING_M_NEGATIVE_15V_POWER] * SENSING_NEGATIVE_15V_POWER_GAIN;
    (*s_ResultData).SENSING_Temperature                 = s_Data.ADC[SENSING_TEMP] * SENSING_TEMPERATURE_GAIN;
    (*s_ResultData).SENSING_M_Positive_5V_Power         = s_Data.ADC[SENSING_M_POSITIVE_5V_POWER] * SENSING_POSITIVE_05V_POWER_GAIN;
    (*s_ResultData).SENSING_Analog_3R3V_Power           = s_Data.ADC[SENSING_ANALOG_3R3V_POWER] * SENSING_POSITIVE_3R3V_POWER_GAIN;
    (*s_ResultData).SENSING_Digital_3R3V_Power          = s_Data.ADC[SENSING_DIGITAL_3R3V_POWER] * SENSING_POSITIVE_3R3V_POWER_GAIN;
    (*s_ResultData).SENSING_Analog_5V_Power             = s_Data.ADC[SENSING_ANALOG_5V_POWER] * SENSING_POSITIVE_05V_POWER_GAIN;

    switch (channel)
    {
        case 0:
            (*s_ResultData).SENSING_Bat_Voltage     = s_Data.ExternalADC[SENSING_BAT_VOLT_CH1] * SENSING_BATTERY_VOLTAGE_GAIN;
            (*s_ResultData).SENSING_Current         = s_Data.ExternalADC[SENSING_CURRENT_CH1] * SENSING_CURRENT_GAIN;
            (*s_ResultData).SENSING_Ground_Power    = s_Data.ADC[SENSING_GROUND_POWER_CH1] * SENSING_GROUND_POWER_GAIN;
            break;
        case 1:
            (*s_ResultData).SENSING_Bat_Voltage     = s_Data.ExternalADC[SENSING_BAT_VOLT_CH2] * SENSING_BATTERY_VOLTAGE_GAIN;
            (*s_ResultData).SENSING_Current         = s_Data.ExternalADC[SENSING_CURRENT_CH2] * SENSING_CURRENT_GAIN;
            (*s_ResultData).SENSING_Ground_Power    = s_Data.ADC[SENSING_GROUND_POWER_CH2] * SENSING_GROUND_POWER_GAIN;
            break;
        case 2:
            (*s_ResultData).SENSING_Bat_Voltage     = s_Data.ExternalADC[SENSING_BAT_VOLT_CH3] * SENSING_BATTERY_VOLTAGE_GAIN;
            (*s_ResultData).SENSING_Current         = s_Data.ExternalADC[SENSING_CURRENT_CH3] * SENSING_CURRENT_GAIN;
            (*s_ResultData).SENSING_Ground_Power    = s_Data.ADC[SENSING_GROUND_POWER_CH3] * SENSING_GROUND_POWER_GAIN;
            break;
        case 3:
            (*s_ResultData).SENSING_Bat_Voltage     = s_Data.ExternalADC[SENSING_BAT_VOLT_CH4] * SENSING_BATTERY_VOLTAGE_GAIN;
            (*s_ResultData).SENSING_Current         = s_Data.ExternalADC[SENSING_CURRENT_CH4] * SENSING_CURRENT_GAIN;
            (*s_ResultData).SENSING_Ground_Power    = s_Data.ADC[SENSING_GROUND_POWER_CH4] * SENSING_GROUND_POWER_GAIN;
            break;
    }
    return (SUCCESS);  //SUCCESS Or FAIL
}

//
// End of file
//

