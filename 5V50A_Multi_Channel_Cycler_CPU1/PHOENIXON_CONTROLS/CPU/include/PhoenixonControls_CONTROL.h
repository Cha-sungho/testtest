/*
 * PhoenixonControls_CONTROL.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_CONTROL_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_CONTROL_H_

#define CONTROL_SHUNT                       (float)(0.001)
#define CONTROL_RESISTANCE_VALUE            (float)(511.0)  //  50A

#define CONTROL_CURRETN_AD8226_GAIN         (float)(((float)(49400.0) / CONTROL_RESISTANCE_VALUE) + (float)(1.0))

#define CONTROL_VOLTAGE_GAIN                (float)(1.0)
#define CONTROL_INTEGRATION_VOLTAGE_GAIN    (float)(1.0)
#define CONTROL_CURRENT_GAIN                (float)(CONTROL_CURRETN_AD8226_GAIN * CONTROL_SHUNT)
#define CONTROL_CURRENT_GAIN_FINE           (float)(CONTROL_CURRENT_GAIN * (float)(50.0))   // 1Mohm

typedef enum
{
    CONTROL_OFF,
    CONTROL_ON,
    CONTROL_COMMAND_INIT,
} E_CONTROL_COMMAND;

typedef enum
{
    CONTROL_REST,
    CONTROL_CHARGE,
    CONTROL_DISCHARGE,
    CONTROL_OPERATION_INIT,
} E_CONTROL_OPERATION_MODE_SELECT;

typedef enum
{
    CONTROL_GND,    //S1
    CONTROL_47K,    //S2,
    CONTROL_36K,    //S3,
    CONTROL_24K,    //S4,
    CONTROL_13K,    //S5
    CONTROL_3R9K,   //S6
    CONTROL_1K,     //S7
    CONTROL_OPEN,   //S8
} E_CONTROL_SLOP_SELECT;

typedef struct
{
    E_CONTROL_COMMAND e_Command;
    E_CONTROL_COMMAND e_OldCommand;
} S_CONTROL_VRELAY;

typedef struct
{
    E_CONTROL_COMMAND e_Command;
    E_CONTROL_COMMAND e_OldCommand;
} S_CONTROL_RELAY;

typedef struct
{
    float32 f_Value;
    float32 f_OldValue;
} S_CONTROL_VDAC;

typedef struct
{
    float32 f_Value;
    float32 f_OldValue;
} S_CONTROL_DAC;

typedef struct
{
    E_CONTROL_OPERATION_MODE_SELECT e_Mode;
    E_CONTROL_OPERATION_MODE_SELECT e_OldMode;
} S_CONTROL_MUX;

typedef struct
{
    E_CONTROL_COMMAND e_Command;
    E_CONTROL_COMMAND e_OldCommand;
} S_CONTROL_INTEGRATION_RESET_MUX;

typedef struct
{
    E_CONTROL_SLOP_SELECT e_Slop;
    E_CONTROL_SLOP_SELECT e_OldSlop;
} S_CONTROL_INTEGRATION_SOLP_MUX;


//----------------------------------------------------------------------------------


void CONTROL_Init(void);
S_CONTROL_RELAY                 CONTROL_CurrentRELAY        (Uint16 channel, E_CONTROL_COMMAND e_Command);
S_CONTROL_RELAY                 CONTROL_CurrentRELAY_Operation        (Uint16 channel, E_CONTROL_COMMAND e_Command);
S_CONTROL_MUX                   CONTROL_OperationMUX        (Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT e_Mode);
S_CONTROL_INTEGRATION_RESET_MUX CONTROL_IntegrationResetMUX (Uint16 channel, E_CONTROL_COMMAND e_Command);
S_CONTROL_INTEGRATION_SOLP_MUX  CONTROL_IntegrationSlopMUX  (Uint16 channel, E_CONTROL_SLOP_SELECT e_Slop);

S_CONTROL_VDAC                  CONTROL_GateOffsetDAC       (Uint16 channel, float32 f_VolatageValue);
S_CONTROL_MUX                   CONTROL_GateOutputMUX       (Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT e_Mode);
S_CONTROL_MUX                   CONTROL_GateOutputAMP       (Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT e_Mode);

S_CONTROL_VDAC                  CONTROL_VoltageReferenceDAC (Uint16 channel, float32 f_VolatageValue);
S_CONTROL_DAC                   CONTROL_CurrentReferenceDAC (Uint16 channel, float32 f_CurrentValue);
S_CONTROL_DAC                   CONTROL_FineCurrentReferenceDAC(Uint16 channel, float32 f_CurrentValue);

#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_CONTROL_H_ */
