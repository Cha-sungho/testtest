/*
 * PhoenixonControls_AD5547.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_AD5547_DAC_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_AD5547_DAC_H_

#define AD5547_REFERENCE_VOLTAGE            (float)(5.0)

#define AD5547_IFINE_REFERENCE_VOLTAGE      (float)(2.5)

#define AD5547_RESOLUTION                   (float)(65536.0)
#define AD5547_ZERO_VOLTAGE_BIPOLAR         (float)(32768.0)
#define AD5547_BIPOLAR_GAIN                 (float)(AD5547_REFERENCE_VOLTAGE / (AD5547_RESOLUTION / 2.0))   // (5/2^16 -> -5V ~ +5V)

#define AD5547_IFINE_BIPOLAR_GAIN           (float)(AD5547_IFINE_REFERENCE_VOLTAGE / (AD5547_RESOLUTION / 2.0))   // (5/2^16 -> -2.5V ~ +2.5V)

//----------------------------------------------------------------------------------------------
#define AD5547_DAC_LDAC_VREF1_ADDRESS       (Uint32)(0x10000A)
#define AD5547_DAC_LDAC_VREF2_ADDRESS       (Uint32)(0x10000C)
#define AD5547_DAC_LDAC_IREF1_ADDRESS       (Uint32)(0x100010)
#define AD5547_DAC_LDAC_IREF2_ADDRESS       (Uint32)(0x100012)
#define AD5547_DAC_LDAC_INTEG1_ADDRESS      (Uint32)(0x100014)
#define AD5547_DAC_LDAC_INTEG2_ADDRESS      (Uint32)(0x100016)

#define AD5547_DAC_nWR_VREF1_ADDRESS        (Uint32)(0x10000B)
#define AD5547_DAC_nWR_VREF2_ADDRESS        (Uint32)(0x10000D)
#define AD5547_DAC_nWR_IREF1_ADDRESS        (Uint32)(0x100011)
#define AD5547_DAC_nWR_IREF2_ADDRESS        (Uint32)(0x100013)
#define AD5547_DAC_nWR_INTEG1_ADDRESS       (Uint32)(0x100015)
#define AD5547_DAC_nWR_INTEG2_ADDRESS       (Uint32)(0x100017)


#define AD5547_VREF_1_2_WIRTE_DATA(X)           {DSP_EmifWrite(AD5547_DAC_nWR_VREF1_ADDRESS, X);}
#define AD5547_VREF_3_4_WIRTE_DATA(X)           {DSP_EmifWrite(AD5547_DAC_nWR_VREF2_ADDRESS, X);}

#define AD5547_VINT_1_2_WIRTE_DATA(X)           {DSP_EmifWrite(AD5547_DAC_nWR_INTEG1_ADDRESS, X);}
#define AD5547_VINT_3_4_WIRTE_DATA(X)           {DSP_EmifWrite(AD5547_DAC_nWR_INTEG2_ADDRESS, X);}

#define AD5547_IREF_1_2_WIRTE_DATA(X)           {DSP_EmifWrite(AD5547_DAC_nWR_IREF1_ADDRESS, X);}
#define AD5547_IREF_3_4_WIRTE_DATA(X)           {DSP_EmifWrite(AD5547_DAC_nWR_IREF2_ADDRESS, X);}


#define AD5547_VREF_1_2_LDAC                    {DSP_EmifWrite(AD5547_DAC_LDAC_VREF1_ADDRESS, 0xFFFF);}
#define AD5547_VREF_3_4_LDAC                    {DSP_EmifWrite(AD5547_DAC_LDAC_VREF2_ADDRESS, 0xFFFF);}

#define AD5547_VINT_1_2_LDAC                    {DSP_EmifWrite(AD5547_DAC_LDAC_INTEG1_ADDRESS, 0xFFFF);}
#define AD5547_VINT_3_4_LDAC                    {DSP_EmifWrite(AD5547_DAC_LDAC_INTEG2_ADDRESS, 0xFFFF);}

#define AD5547_IREF_1_2_LDAC                    {DSP_EmifWrite(AD5547_DAC_LDAC_IREF1_ADDRESS, 0xFFFF);}
#define AD5547_IREF_3_4_LDAC                    {DSP_EmifWrite(AD5547_DAC_LDAC_IREF2_ADDRESS, 0xFFFF);}


typedef enum
{
    AD5547_A,
    AD5547_NON_OUTPUT,
    AD5547_ALL_OUTPUT,
    AD5547_B
}E_AD5547_OUTPUT_SELECT;


void AD5547_Init(void);
Uint16 AD5547_Reset(void);
Uint16 AD5547_OutputSelect(E_AD5547_OUTPUT_SELECT e_Port);
Uint16 AD5547_Write_VREF(Uint16 channel, Uint16 ui_Data);
Uint16 AD5547_Write_IREF(Uint16 channel, Uint16 ui_Data);
Uint16 AD5547_Write_VINT(Uint16 channel, Uint16 ui_Data);

#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_AD5547_DAC_H_ */
