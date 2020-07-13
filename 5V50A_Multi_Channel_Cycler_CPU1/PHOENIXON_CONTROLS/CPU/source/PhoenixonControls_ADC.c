

#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

#pragma CODE_SECTION(DSP_ADCRead, ".TI.ramfunc");

void DSP_ADCInit(void)
{
    //
    // Set ADCCLK divider to /4
    //
    ADC_setPrescaler(ADCA_BASE, ADC_CLK_DIV_8_0);
    ADC_setPrescaler(ADCB_BASE, ADC_CLK_DIV_8_0);
    ADC_setPrescaler(ADCC_BASE, ADC_CLK_DIV_8_0);
    ADC_setPrescaler(ADCD_BASE, ADC_CLK_DIV_8_0);
    //
    // Set resolution and signal mode (see #defines above) and load
    // corresponding trims.
    //
    ADC_setMode(ADCA_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    ADC_setMode(ADCB_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    ADC_setMode(ADCC_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    ADC_setMode(ADCD_BASE, ADC_RESOLUTION_12BIT, ADC_MODE_SINGLE_ENDED);
    //
    // Set pulse positions to late
    //
    ADC_setInterruptPulseMode(ADCA_BASE, ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(ADCB_BASE, ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(ADCC_BASE, ADC_PULSE_END_OF_CONV);
    ADC_setInterruptPulseMode(ADCD_BASE, ADC_PULSE_END_OF_CONV);
    //
    // Power up the ADCs and then delay for 1 ms
    //
    ADC_enableConverter(ADCA_BASE);
    ADC_enableConverter(ADCB_BASE);
    ADC_enableConverter(ADCC_BASE);
    ADC_enableConverter(ADCD_BASE);

    DEVICE_DELAY_US(1000);

    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 511);//adc window max 512
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN1, 511);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 511);
    ADC_setupSOC(ADCA_BASE, ADC_SOC_NUMBER3, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, 511);

    ADC_setInterruptSource(ADCA_BASE, ADC_INT_NUMBER3, ADC_SOC_NUMBER3);
    ADC_enableInterrupt(ADCA_BASE, ADC_INT_NUMBER3);
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER3);


    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 511);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN1, 511);
    ADC_setupSOC(ADCB_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 511);

    ADC_setInterruptSource(ADCB_BASE, ADC_INT_NUMBER2, ADC_SOC_NUMBER2);
    ADC_enableInterrupt(ADCB_BASE, ADC_INT_NUMBER2);
    ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER2);


    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 511);
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN3, 511);
    ADC_setupSOC(ADCC_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN4, 511);

    ADC_setInterruptSource(ADCC_BASE, ADC_INT_NUMBER1, ADC_SOC_NUMBER2);
    ADC_enableInterrupt(ADCC_BASE, ADC_INT_NUMBER1);
    ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);

    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER0, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN0, 511);
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER1, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN1, 511);
    ADC_setupSOC(ADCD_BASE, ADC_SOC_NUMBER2, ADC_TRIGGER_SW_ONLY, ADC_CH_ADCIN2, 511);

    ADC_setInterruptSource(ADCD_BASE, ADC_INT_NUMBER4, ADC_SOC_NUMBER2);
    ADC_enableInterrupt(ADCD_BASE, ADC_INT_NUMBER4);
    ADC_clearInterruptStatus(ADCD_BASE, ADC_INT_NUMBER4);
}

Uint16 DSP_ADCRead(float32 *Data)
{
    Uint16  ui_i = 0;
    Uint16 ui_ResultData[13] = {0,};
    int32 l_TimeOut = 200000;
    //
    // Convert, wait for completion, and store results
    //
    ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER0);
    ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER1);
    ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER2);
    ADC_forceSOC(ADCA_BASE, ADC_SOC_NUMBER3);

    ADC_forceSOC(ADCB_BASE, ADC_SOC_NUMBER0);
    ADC_forceSOC(ADCB_BASE, ADC_SOC_NUMBER1);
    ADC_forceSOC(ADCB_BASE, ADC_SOC_NUMBER2);

    ADC_forceSOC(ADCC_BASE, ADC_SOC_NUMBER0);
    ADC_forceSOC(ADCC_BASE, ADC_SOC_NUMBER1);
    ADC_forceSOC(ADCC_BASE, ADC_SOC_NUMBER2);

    ADC_forceSOC(ADCD_BASE, ADC_SOC_NUMBER0);
    ADC_forceSOC(ADCD_BASE, ADC_SOC_NUMBER1);
    ADC_forceSOC(ADCD_BASE, ADC_SOC_NUMBER2);

    l_TimeOut = 200000;
    while(ADC_getInterruptStatus(ADCA_BASE, ADC_INT_NUMBER3) == false)
    {
        if(l_TimeOut-- <= 0)break;
    }
    ADC_clearInterruptStatus(ADCA_BASE, ADC_INT_NUMBER3);
    l_TimeOut = 200000;

    while(ADC_getInterruptStatus(ADCB_BASE, ADC_INT_NUMBER2) == false)
    {
        if(l_TimeOut-- <= 0)break;
    }
    ADC_clearInterruptStatus(ADCB_BASE, ADC_INT_NUMBER2);
    l_TimeOut = 200000;

    while(ADC_getInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1) == false)
    {
        if(l_TimeOut-- <= 0)break;
    }
    ADC_clearInterruptStatus(ADCC_BASE, ADC_INT_NUMBER1);
    l_TimeOut = 200000;

    while(ADC_getInterruptStatus(ADCD_BASE, ADC_INT_NUMBER4) == false)
    {
        if(l_TimeOut-- <= 0)break;
    }
    ADC_clearInterruptStatus(ADCD_BASE, ADC_INT_NUMBER4);

    //
    // Store results
    //
    ui_ResultData[0] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER0);        // LBIAS_ADC
    ui_ResultData[1] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER1);        // HBIAS_ADC
    ui_ResultData[2] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER2);        // +15V_PWR_ADC
    ui_ResultData[3] = ADC_readResult(ADCARESULT_BASE, ADC_SOC_NUMBER3);        // -15VA

    ui_ResultData[4] = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER0);        // 5V_PWR_ADC
    ui_ResultData[5] = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER1);        // TEMP_ADC
    ui_ResultData[6] = ADC_readResult(ADCBRESULT_BASE, ADC_SOC_NUMBER2);        // CURRP_1CH_ADC

    ui_ResultData[7] = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER0);        // CURRP_2CH_ADC
    ui_ResultData[8] = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER1);        // CURRP_3CH_ADC
    ui_ResultData[9] = ADC_readResult(ADCCRESULT_BASE, ADC_SOC_NUMBER2);        // CURRP_4CH_ADC

    ui_ResultData[10] = ADC_readResult(ADCDRESULT_BASE, ADC_SOC_NUMBER0);        // CURRP_2CH_ADC
    ui_ResultData[11] = ADC_readResult(ADCDRESULT_BASE, ADC_SOC_NUMBER1);        // CURRP_3CH_ADC
    ui_ResultData[12] = ADC_readResult(ADCDRESULT_BASE, ADC_SOC_NUMBER2);        // CURRP_4CH_ADC

    for(ui_i = 0; ui_i < 13; ui_i++)Data[ui_i] = ui_ResultData[ui_i] * DSP_ADC_GAIN;
    //
    // Software breakpoint. At this point, conversion results are stored in
    // adcAResult0, adcAResult1, adcBResult0, and adcBResult1.
    //
    // Hit run again to get updated conversions.
    //
    return(SUCCESS); //SUCCESS Or FAIL
}

//
// End of file
//
