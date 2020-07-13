/*
 * PhoenixonControls_AD5664R_DAC.h
 *
 *  Created on: 2020. 3. 30.
 *      Author: CHA
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_AD5664R_DAC_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_AD5664R_DAC_H_


#define AD5664R_INTERNAL_REFERENCE_VOLTAGE          (float)(1.25)
#define AD5664R_EXTERNAL_REFERENCE_VOLTAGE          (float)(2.5)
#define AD5644R_RESOLUTION                          (float)(16384.0)
#define AD5664R_RESOLUTION                          (float)(65536.0)


#define AD5664R_COMMAND_WRITE_INPUT_N               0
#define AD5664R_COMMAND_UPDATE_DAC_N                1
#define AD5664R_COMMAND_WRITE_INPUT_N_UPDATE_ALL    2
#define AD5664R_COMMAND_WRITE_INPUT_N_UPDATE_N      3
#define AD5664R_COMMAND_POWERDOWN_DAC               4
#define AD5664R_COMMAND_RESET                       5
#define AD5664R_COMMAND_LDAC_SETUP                  6
#define AD5664R_COMMAND_INTERNAL_REF_SETUP          7

#define AD5664R_ADDRESS_CH1                         0
#define AD5664R_ADDRESS_CH2                         1
#define AD5664R_ADDRESS_CH3                         2
#define AD5664R_ADDRESS_CH4                         3
#define AD5664R_ADDRESS_CH_ALL                      4

#define AD5664R_LDAC_PWRDN_NONE                     0
#define AD5664R_LDAC_PWRDN_1K                       1
#define AD5664R_LDAC_PWRDN_100K                     2
#define AD5664R_LDAC_PWRDN_3_STATE                  3

#define AD5664R_RESET                               0   // DAC register, Input shift register
#define AD5664R_RESET_ALL_REGISTER                  4   // All register

#define AD5664R_INTERNAL_REFERENCE_OFF              0
#define AD5664R_INTERNAL_REFERENCE_ON               1

#define SPI2_BAUDRATE                           ((200E6 / 1) / 1600E3) - 1  // 1.6MHz

#define AD5664R_WRITE(X)             {DSP_SpiWrite(X);}

typedef enum
{
    AD5664R_CH1,
    AD5664R_CH2,
    AD5664R_CH3,
    AD5664R_CH4,
    RESERVE1,
    RESERVE2,
    RESERVE3,
    AD5664R_ALL,
} E_AD5664R_CHANNEL_SELECT;

typedef struct
{
    float Target_Voltage;
    E_AD5664R_CHANNEL_SELECT Target_Channel;


} S_AD5664R_TEST;

extern S_AD5664R_TEST cha_test;

void AD5664R_Init(void);
void AD5664R_Reset(void);
//void AD5644R_Write(E_AD5664R_CHANNEL_SELECT Channel, Uint16 Command, Uint16 Data);
//void AD5644R_Write(Uint16 Channel, Uint16 Command, Uint16 Data);
//void AD5664R_Internal_Reference_Setup(void);
//void I_Fine_DAC(E_AD5664R_CHANNEL_SELECT channel, float f_VolatageValue);

void AD5664R_Write(E_AD5664R_CHANNEL_SELECT Channel, Uint16 Command, Uint16 Data);

void Init_SPI2(void);
void SPI_Transmit(Uint16 data);


#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_AD5664R_DAC_H_ */
