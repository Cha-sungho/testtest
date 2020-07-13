/*
 * PhoenixonControls_CONTROL.c
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"


//#pragma CODE_SECTION(CONTROL_VoltageRELAY, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_CurrentRELAY, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_CurrentRELAY_Operation, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_OperationMUX, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_IntegrationResetMUX, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_IntegrationSlopMUX, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_GateOutputMUX, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_GateOutputAMP, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_VoltageReferenceDAC, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_CurrentReferenceDAC, ".TI.ramfunc");
#pragma CODE_SECTION(CONTROL_GateOffsetDAC, ".TI.ramfunc");

Uint16 I_Fine_Value = 0;

void CONTROL_Init(void)
{

}

//S_CONTROL_VRELAY CONTROL_VoltageRELAY(Uint16 channel, E_CONTROL_COMMAND e_Command)
//{
//    volatile static S_CONTROL_VRELAY s_Data[4] = {
//            { CONTROL_OFF, CONTROL_COMMAND_INIT },
//            { CONTROL_OFF, CONTROL_COMMAND_INIT },
//            { CONTROL_OFF, CONTROL_COMMAND_INIT },
//            { CONTROL_OFF, CONTROL_COMMAND_INIT }
//    };
//
//    s_Data[channel].e_Command = e_Command;
//
//    if (s_Data[channel].e_OldCommand != s_Data[channel].e_Command) {
//        switch (channel)
//        {
//            case 0:
//                if (s_Data[channel].e_Command == CONTROL_ON) {
//                    DSP_GPIO3_CH1_VOLTAGE_RELAY_SET
//                } else {
//                    DSP_GPIO3_CH1_VOLTAGE_RELAY_CLEAR
//                }
//                break;
//            case 1:
//                if (s_Data[channel].e_Command == CONTROL_ON) {
//                    DSP_GPIO2_CH2_VOLTAGE_RELAY_SET
//                } else {
//                    DSP_GPIO2_CH2_VOLTAGE_RELAY_CLEAR
//                }
//                break;
//            case 2:
//                if (s_Data[channel].e_Command == CONTROL_ON) {
//                    DSP_GPIO1_CH3_VOLTAGE_RELAY_SET
//                } else {
//                    DSP_GPIO1_CH3_VOLTAGE_RELAY_CLEAR
//                }
//                break;
//            case 3:
//                if (s_Data[channel].e_Command == CONTROL_ON) {
//                    DSP_GPIO0_CH4_VOLTAGE_RELAY_SET
//                } else {
//                    DSP_GPIO0_CH4_VOLTAGE_RELAY_CLEAR
//                }
//                break;
//        }
//        s_Data[channel].e_OldCommand = s_Data[channel].e_Command;
//        DELAY_US(10000);
//    }
//
//    return (s_Data[channel]);  //SUCCESS Or FAIL
//}

S_CONTROL_RELAY CONTROL_CurrentRELAY(Uint16 channel, E_CONTROL_COMMAND e_Command)
{
    volatile static S_CONTROL_RELAY s_Data[4] = {
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT }
    };

    s_Data[channel].e_Command = e_Command;

    if (s_Data[channel].e_OldCommand != s_Data[channel].e_Command) {
        switch (channel)
        {
            case 0:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO66_DSP_CH1_RLY_ON_SET
                } else {
                    DSP_GPIO66_DSP_CH1_RLY_ON_CLEAR
                }
                break;
            case 1:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO65_DSP_CH2_RLY_ON_SET
                } else {
                    DSP_GPIO65_DSP_CH2_RLY_ON_CLEAR
                }
                break;
            case 2:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO64_DSP_CH3_RLY_ON_SET
                } else {
                    DSP_GPIO64_DSP_CH3_RLY_ON_CLEAR
                }
                break;
            case 3:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO63_DSP_CH4_RLY_ON_SET
                } else {
                    DSP_GPIO63_DSP_CH4_RLY_ON_CLEAR
                }
                break;
        }
        s_Data[channel].e_OldCommand = s_Data[channel].e_Command;
        DELAY_US(10000);
    }

    return (s_Data[channel]);
}

S_CONTROL_RELAY CONTROL_CurrentRELAY_Operation(Uint16 channel, E_CONTROL_COMMAND e_Command)
{
    volatile static S_CONTROL_RELAY s_Data[4] = {
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT }
    };

    s_Data[channel].e_Command = e_Command;

    if (s_Data[channel].e_OldCommand != s_Data[channel].e_Command) {
        switch (channel)
        {
            case 0:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO66_DSP_CH1_RLY_ON_SET
                } else {
                    DSP_GPIO66_DSP_CH1_RLY_ON_CLEAR
                }
                break;
            case 1:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO65_DSP_CH2_RLY_ON_SET
                } else {
                    DSP_GPIO65_DSP_CH2_RLY_ON_CLEAR
                }
                break;
            case 2:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO64_DSP_CH3_RLY_ON_SET
                } else {
                    DSP_GPIO64_DSP_CH3_RLY_ON_CLEAR
                }
                break;
            case 3:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO63_DSP_CH4_RLY_ON_SET
                } else {
                    DSP_GPIO63_DSP_CH4_RLY_ON_CLEAR
                }
                break;
        }
        s_Data[channel].e_OldCommand = s_Data[channel].e_Command;
    }

    return (s_Data[channel]);
}

S_CONTROL_MUX CONTROL_OperationMUX(Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT e_Mode)
{
    volatile static S_CONTROL_MUX s_Data[4] = {
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT }
    };

    s_Data[channel].e_Mode = e_Mode;

    if (s_Data[channel].e_OldMode != s_Data[channel].e_Mode) {
        switch (channel)
        {
            case 0:
                if (s_Data[channel].e_Mode == CONTROL_CHARGE) {
                    DSP_GPIO6_OPMODE_CH1_SEL1_CLEAR
                    DELAY_US(1);
                    DSP_GPIO5_OPMODE_CH1_SEL0_SET
                } else if (s_Data[channel].e_Mode == CONTROL_DISCHARGE) {
                    DSP_GPIO5_OPMODE_CH1_SEL0_CLEAR
                    DELAY_US(1);
                    DSP_GPIO6_OPMODE_CH1_SEL1_SET
                } else {
                    DSP_GPIO5_OPMODE_CH1_SEL0_CLEAR
                    DSP_GPIO6_OPMODE_CH1_SEL1_CLEAR
                }
                break;
            case 1:
                if (s_Data[channel].e_Mode == CONTROL_CHARGE) {
                    DSP_GPIO4_OPMODE_CH2_SEL1_CLEAR
                    DELAY_US(1);
                    DSP_GPIO7_OPMODE_CH2_SEL0_SET
                } else if (s_Data[channel].e_Mode == CONTROL_DISCHARGE) {
                    DSP_GPIO7_OPMODE_CH2_SEL0_CLEAR
                    DELAY_US(1);
                    DSP_GPIO4_OPMODE_CH2_SEL1_SET
                } else {
                    DSP_GPIO7_OPMODE_CH2_SEL0_CLEAR
                    DSP_GPIO4_OPMODE_CH2_SEL1_CLEAR
                }
                break;
            case 2:
                if (s_Data[channel].e_Mode == CONTROL_CHARGE) {
                    DSP_GPIO11_OPMODE_CH3_SEL1_CLEAR
                    DELAY_US(1);
                    DSP_GPIO10_OPMODE_CH3_SEL0_SET
                } else if (s_Data[channel].e_Mode == CONTROL_DISCHARGE) {
                    DSP_GPIO10_OPMODE_CH3_SEL0_CLEAR
                    DELAY_US(1);
                    DSP_GPIO11_OPMODE_CH3_SEL1_SET
                } else {
                    DSP_GPIO10_OPMODE_CH3_SEL0_CLEAR
                    DSP_GPIO11_OPMODE_CH3_SEL1_CLEAR
                }
                break;
            case 3:
                if (s_Data[channel].e_Mode == CONTROL_CHARGE) {
                    DSP_GPIO94_OPMODE_CH4_SEL1_CLEAR
                    DELAY_US(1);
                    DSP_GPIO12_OPMODE_CH4_SEL0_SET
                } else if (s_Data[channel].e_Mode == CONTROL_DISCHARGE) {
                    DSP_GPIO12_OPMODE_CH4_SEL0_CLEAR
                    DELAY_US(1);
                    DSP_GPIO94_OPMODE_CH4_SEL1_SET
                } else {
                    DSP_GPIO12_OPMODE_CH4_SEL0_CLEAR
                    DSP_GPIO94_OPMODE_CH4_SEL1_CLEAR
                }
                break;
        }
        s_Data[channel].e_OldMode = s_Data[channel].e_Mode;
        DELAY_US(1);  //DG333AGW Turn On Time Max 175ns
                      //         Turn Off Time Max 145ns
                      //         Break-Before-Make Time Delay 5ns
    }

    return (s_Data[channel]);
}

S_CONTROL_INTEGRATION_RESET_MUX CONTROL_IntegrationResetMUX(Uint16 channel, E_CONTROL_COMMAND e_Command)
{
    volatile static S_CONTROL_INTEGRATION_RESET_MUX s_Data[4] = {
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT },
            { CONTROL_OFF, CONTROL_COMMAND_INIT }
    };

    s_Data[channel].e_Command = e_Command;

    if (s_Data[channel].e_OldCommand != s_Data[channel].e_Command) {
        switch (channel)
        {
            case 0:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO16_INTG_CH1_RESET_SET
                } else {
                    DSP_GPIO16_INTG_CH1_RESET_CLEAR
                }
                break;
            case 1:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO17_INTG_CH2_RESET_SET
                } else {
                    DSP_GPIO17_INTG_CH2_RESET_CLEAR
                }
                break;
            case 2:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO18_INTG_CH3_RESET_SET
                } else {
                    DSP_GPIO18_INTG_CH3_RESET_CLEAR
                }
                break;
            case 3:
                if (s_Data[channel].e_Command == CONTROL_ON) {
                    DSP_GPIO15_INTG_CH4_RESET_SET
                } else {
                    DSP_GPIO15_INTG_CH4_RESET_CLEAR
                }
                break;
        }
        DELAY_US(1);  //DG333AGW Turn On Time Max 175ns
                      //Turn Off Time Max 145ns
                      // Break-Before-Make Time Delay 5ns
        s_Data[channel].e_OldCommand = s_Data[channel].e_Command;
    }

    return (s_Data[channel]);
}

S_CONTROL_INTEGRATION_SOLP_MUX CONTROL_IntegrationSlopMUX(Uint16 channel, E_CONTROL_SLOP_SELECT e_Slop)
{
    volatile static S_CONTROL_INTEGRATION_SOLP_MUX s_Data[4] = {
            { CONTROL_GND, CONTROL_OPEN },
            { CONTROL_GND, CONTROL_OPEN },
            { CONTROL_GND, CONTROL_OPEN },
            { CONTROL_GND, CONTROL_OPEN }
    };

    s_Data[channel].e_Slop = e_Slop;

    if (s_Data[channel].e_OldSlop != s_Data[channel].e_Slop) {
        switch (channel)
        {
            case 0:
                switch (s_Data[channel].e_Slop)
                {
                    case CONTROL_GND: //S1, 000
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR
                        break;
                    case CONTROL_47K: //S2, 001
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_SET
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR
                        break;
                    case CONTROL_36K: //S3, 010
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_SET
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR
                        break;
                    case CONTROL_24K: //S4, 011
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_SET
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_SET
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR
                        break;
                    case CONTROL_13K: //S5, 100
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_SET
                        break;
                    case CONTROL_3R9K: //S6, 101
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_SET
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_SET
                        break;
                    case CONTROL_1K: //S7, 110
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_SET
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_SET
                        break;
                    case CONTROL_OPEN: //S8, 111
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_SET
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_SET
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_SET
                        break;
                    default:
                        DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR
                        DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR
                        DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR
                        break;
                }
                break;
            case 1:
                switch (s_Data[channel].e_Slop)
                {
                    case CONTROL_GND: //S1, 000:
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR
                        break;
                    case CONTROL_47K: //S2, 001
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_SET
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR
                        break;
                    case CONTROL_36K: //S3, 010
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_SET
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR
                        break;
                    case CONTROL_24K: //S4, 011
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_SET
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_SET
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR
                        break;
                    case CONTROL_13K: //S5, 100
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_SET
                        break;
                    case CONTROL_3R9K: //S6, 101
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_SET
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_SET
                        break;
                    case CONTROL_1K: //S7, 110
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_SET
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_SET
                        break;
                    case CONTROL_OPEN: //S8, 111
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_SET
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_SET
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_SET
                        break;
                    default:
                        DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR
                        DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR
                        DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR
                        break;
                }
                break;
            case 2:
                switch (s_Data[channel].e_Slop)
                {
                    case CONTROL_GND: //S1, 000:
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR
                        break;
                    case CONTROL_47K: //S2, 001
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_SET
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR
                        break;
                    case CONTROL_36K: //S3, 010
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_SET
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR
                        break;
                    case CONTROL_24K: //S4, 011
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_SET
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_SET
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR
                        break;
                    case CONTROL_13K:  //S5, 100
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_SET
                        break;
                    case CONTROL_3R9K: //S6, 101
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_SET
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_SET
                        break;
                    case CONTROL_1K: //S7, 110
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_SET
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_SET
                        break;
                    case CONTROL_OPEN: //S8, 111
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_SET
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_SET
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_SET
                        break;
                    default:
                        DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR
                        DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR
                        DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR
                        break;
                }
                break;
            case 3:
                switch (s_Data[channel].e_Slop)
                {
                    case CONTROL_GND: //S1, 000:
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR
                        break;
                    case CONTROL_47K: //S2, 001
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_SET
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR
                        break;
                    case CONTROL_36K: //S3, 010
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_SET
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR
                        break;
                    case CONTROL_24K: //S4, 011
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_SET
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_SET
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR
                        break;
                    case CONTROL_13K:  //S5, 100
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_SET
                        break;
                    case CONTROL_3R9K: //S6, 101
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_SET
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_SET
                        break;
                    case CONTROL_1K: //S7, 110
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_SET
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_SET
                        break;
                    case CONTROL_OPEN: //S8, 111
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_SET
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_SET
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_SET
                        break;
                    default:
                        DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR
                        DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR
                        DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR
                        break;
                }
                break;
        }
        DELAY_US(1);
        s_Data[channel].e_OldSlop = s_Data[channel].e_Slop;
    }

    return (s_Data[channel]);
}

S_CONTROL_VDAC CONTROL_GateOffsetDAC(Uint16 channel, float32 f_VolatageValue)
{
    volatile static S_CONTROL_VDAC s_Data[4] = {
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 }
    };

    Uint16 ui_Temp = 0;
    float32 f_Temp = 0.0;

    s_Data[channel].f_Value = f_VolatageValue;

    if (s_Data[channel].f_OldValue != s_Data[channel].f_Value) {
        f_Temp = s_Data[channel].f_Value * CONTROL_INTEGRATION_VOLTAGE_GAIN;
        ui_Temp = (unsigned int) ((f_Temp / AD5547_BIPOLAR_GAIN ) + AD5547_ZERO_VOLTAGE_BIPOLAR );

        AD5547_Write_VINT(channel, ui_Temp);

        s_Data[channel].f_OldValue = s_Data[channel].f_Value;
    }

    return (s_Data[channel]);
}

S_CONTROL_MUX CONTROL_GateOutputMUX(Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT e_Mode)
{
    volatile static S_CONTROL_MUX s_Data[4] = {
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT }
    };

    s_Data[channel].e_Mode = e_Mode;

    if (s_Data[channel].e_OldMode != s_Data[channel].e_Mode) {
        switch (channel)
        {
            case 0:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO47_OUTPUT_CH1_DISCHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO46_OUTPUT_CH1_CHARGE_SET
                        break;

                    case CONTROL_DISCHARGE:
                        DSP_GPIO46_OUTPUT_CH1_CHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO47_OUTPUT_CH1_DISCHARGE_SET
                        break;

                    default:
                        DSP_GPIO46_OUTPUT_CH1_CHARGE_CLEAR
                        DSP_GPIO47_OUTPUT_CH1_DISCHARGE_CLEAR
                        break;
                }
                break;
            case 1:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO44_OUTPUT_CH2_DISCHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO45_OUTPUT_CH2_CHARGE_SET
                        break;
                    case CONTROL_DISCHARGE:
                        DSP_GPIO45_OUTPUT_CH2_CHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO44_OUTPUT_CH2_DISCHARGE_SET
                        break;
                    default:
                        DSP_GPIO45_OUTPUT_CH2_CHARGE_CLEAR
                        DSP_GPIO44_OUTPUT_CH2_DISCHARGE_CLEAR
                        break;
                }
                break;
            case 2:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO67_OUTPUT_CH3_DISCHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO43_OUTPUT_CH3_CHARGE_SET
                        break;
                    case CONTROL_DISCHARGE:
                        DSP_GPIO43_OUTPUT_CH3_CHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO67_OUTPUT_CH3_DISCHARGE_SET
                        break;
                    default:
                        DSP_GPIO43_OUTPUT_CH3_CHARGE_CLEAR
                        DSP_GPIO67_OUTPUT_CH3_DISCHARGE_CLEAR
                        break;
                }
                break;
            case 3:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO42_OUTPUT_CH4_DISCHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO68_OUTPUT_CH4_CHARGE_SET
                        break;
                    case CONTROL_DISCHARGE:
                        DSP_GPIO68_OUTPUT_CH4_CHARGE_CLEAR
                        DELAY_US(1);
                        DSP_GPIO42_OUTPUT_CH4_DISCHARGE_SET
                        break;
                    default:
                        DSP_GPIO68_OUTPUT_CH4_CHARGE_CLEAR
                        DSP_GPIO42_OUTPUT_CH4_DISCHARGE_CLEAR
                        break;
                }
                break;
        }
        DELAY_US(1);
        s_Data[channel].e_OldMode = s_Data[channel].e_Mode;
    }
    return (s_Data[channel]);
}

S_CONTROL_MUX CONTROL_GateOutputAMP(Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT e_Mode)
{
    volatile static S_CONTROL_MUX s_Data[4] = {
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT },
            { CONTROL_REST, CONTROL_OPERATION_INIT }
    };

    s_Data[channel].e_Mode = e_Mode;

    if (s_Data[channel].e_OldMode != s_Data[channel].e_Mode) {
        switch (channel)
        {
            case 0:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO52_DSP_CH1_DISCHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO53_DSP_CH1_CHG_EN_SET
                        break;
                    case CONTROL_DISCHARGE:
                        DSP_GPIO53_DSP_CH1_CHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO52_DSP_CH1_DISCHG_EN_SET
                        break;
                    default:
                        DSP_GPIO53_DSP_CH1_CHG_EN_CLEAR
                        DSP_GPIO52_DSP_CH1_DISCHG_EN_CLEAR
                        break;
                }
                break;
            case 1:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO58_DSP_CH2_DISCHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO51_DSP_CH2_CHG_EN_SET
                        break;

                    case CONTROL_DISCHARGE:
                        DSP_GPIO51_DSP_CH2_CHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO58_DSP_CH2_DISCHG_EN_SET
                        break;

                    default:
                        DSP_GPIO51_DSP_CH2_CHG_EN_CLEAR
                        DSP_GPIO58_DSP_CH2_DISCHG_EN_CLEAR
                        break;
                }
                break;
            case 2:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO60_DSP_CH3_DISCHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO59_DSP_CH3_CHG_EN_SET
                        break;

                    case CONTROL_DISCHARGE:
                        DSP_GPIO59_DSP_CH3_CHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO60_DSP_CH3_DISCHG_EN_SET
                        break;

                    default:
                        DSP_GPIO59_DSP_CH3_CHG_EN_CLEAR
                        DSP_GPIO60_DSP_CH3_DISCHG_EN_CLEAR
                        break;
                }
                break;
            case 3:
                switch (s_Data[channel].e_Mode)
                {
                    case CONTROL_CHARGE:
                        DSP_GPIO61_DSP_CH4_DISCHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO62_DSP_CH4_CHG_EN_SET
                        break;

                    case CONTROL_DISCHARGE:
                        DSP_GPIO62_DSP_CH4_CHG_EN_CLEAR
                        DELAY_US(5);
                        DSP_GPIO61_DSP_CH4_DISCHG_EN_SET
                        break;

                    default:
                        DSP_GPIO62_DSP_CH4_CHG_EN_CLEAR
                        DSP_GPIO61_DSP_CH4_DISCHG_EN_CLEAR
                        break;
                }
                break;
        }
        DELAY_US(5);
        s_Data[channel].e_OldMode = s_Data[channel].e_Mode;
    }

    return(s_Data[channel]);
}

S_CONTROL_VDAC CONTROL_VoltageReferenceDAC(Uint16 channel, float32 f_VolatageValue)
{
    volatile static S_CONTROL_VDAC s_Data[4] = {
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 }
    };

    Uint16 ui_Temp = 0;
    float32 f_Temp = 0.0;

    s_Data[channel].f_Value = f_VolatageValue;

    if (s_Data[channel].f_OldValue != s_Data[channel].f_Value) {
        f_Temp = s_Data[channel].f_Value * CONTROL_VOLTAGE_GAIN;
        ui_Temp = (unsigned int) ((f_Temp / AD5547_BIPOLAR_GAIN ) + AD5547_ZERO_VOLTAGE_BIPOLAR );
        AD5547_Write_VREF(channel, ui_Temp);
        s_Data[channel].f_OldValue = s_Data[channel].f_Value;
    }

    return (s_Data[channel]);
}


S_CONTROL_DAC CONTROL_FineCurrentReferenceDAC(Uint16 channel, float32 f_CurrentValue)
{
    volatile static S_CONTROL_DAC s_Data[4] = {
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 },
    };

    Uint16 ui_Temp = 0;
    float32 f_Temp = 0.0;

    s_Data[channel].f_Value = f_CurrentValue;

    if (s_Data[channel].f_OldValue != s_Data[channel].f_Value) {
        f_Temp = s_Data[channel].f_Value * CONTROL_CURRENT_GAIN_FINE;
        ui_Temp = (unsigned int) ((f_Temp / AD5547_IFINE_BIPOLAR_GAIN ) + AD5547_ZERO_VOLTAGE_BIPOLAR );
        I_Fine_Value = ui_Temp;
        AD5547_Write_IREF(channel, ui_Temp);
        s_Data[channel].f_OldValue = s_Data[channel].f_Value;
    }
    return(s_Data[channel]);
}


S_CONTROL_DAC CONTROL_CurrentReferenceDAC(Uint16 channel, float32 f_CurrentValue)
{
    volatile static S_CONTROL_DAC s_Data[4] = {
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 },
            { 0.0, -1.0 },
    };

    Uint16 ui_Temp = 0;
    Uint32 Temp = 0;
    float32 f_Temp = 0.0;

    s_Data[channel].f_Value = f_CurrentValue;

        if (s_Data[channel].f_OldValue != s_Data[channel].f_Value) {
            f_Temp = s_Data[channel].f_Value * CONTROL_CURRENT_GAIN;
            f_Temp = f_Temp * 0.5;  // Gain 2น่

            Temp = (Uint32)(f_Temp * AD5664R_RESOLUTION / (AD5664R_EXTERNAL_REFERENCE_VOLTAGE));
            ui_Temp = (Uint16)Temp;

            switch (channel)
            {
            case 0:
                AD5664R_Write(AD5664R_CH1, AD5664R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
                break;
            case 1:
                AD5664R_Write(AD5664R_CH2, AD5664R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
                break;
            case 2:
                AD5664R_Write(AD5664R_CH3, AD5664R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
                break;
            case 3:
                AD5664R_Write(AD5664R_CH4, AD5664R_COMMAND_WRITE_INPUT_N_UPDATE_N, ui_Temp);
                break;
            }

            s_Data[channel].f_OldValue = s_Data[channel].f_Value;
        }

        return(s_Data[channel]);
}

