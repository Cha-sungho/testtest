/*
 * PhoenixonControls_TUNING.c
 *
 *  Created on: 2019. 9. 19.
 *      Author: HW Team
 */

//
// Included Files
//
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

extern S_OPERATION_FILE s_Operation[CONTROL_CHANNEL_PER_BOARD];
extern float32 VoltageCharge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ];
extern float32 VoltageDischarge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ];
extern float32 VoltageCharge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ];
extern float32 VoltageDischarge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ];
extern float32 CurrentCharge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ];
extern float32 CurrentDischarge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ];
extern float32 CurrentCharge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ];
extern float32 CurrentDischarge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ];

extern float32 CurrentCharge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentCharge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];
extern float32 CurrentDischarge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH];

unsigned int TOTAL_CHECK_ADC = 0;

VITuneProperties g_vi_tune;          //Voltage & Current Calibration Properties Value

static Uint16 TuneVADC                  (S_OPERATION_FILE *sp_OP, Uint16 channel, Uint16 voltage, E_CONTROL_OPERATION_MODE_SELECT mode);
static Uint16 VerifyVADC                (S_OPERATION_FILE *sp_OP, Uint16 channel, Uint16 voltage, E_CONTROL_OPERATION_MODE_SELECT mode);
static Uint16 TuneIADC                  (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);
static Uint16 TuneForceIADC             (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);
static Uint16 VerifyIADC                (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);

static Uint16 TuneLowIADC               (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);
static Uint16 VerifyLowIADC             (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);

static void ControlCurrentCoarseFineDac(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current);
static void ControlCurrentCoarseFineDacForITune(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current);
static Uint16 TuneForceFINEIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);

static void ProcessVADCTune             (void);
static void ProcessIADCTune             (void);
static void InsertionSort               (float *Data, int Number);

static void ControlInitial              (S_OPERATION_FILE *sp_OP, Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT mode);
static void ControlSlopMux              (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);
static void ControlGateOffset           (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode);
static void ControlCurrentDacForITune   (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current);
static void ControlCurrentDac           (S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current);
static void ControlVoltageDac           (S_OPERATION_FILE *sp_OP, Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT mode);

void InitVIADCTuneProperties(void)
{
    Uint16 i,j;

    g_vi_tune.adc_sum = 0.0;
    g_vi_tune.adc_sum_low_current = 0.0;

    for (i = 0; i < 1024; i++){
        g_vi_tune.adc_array[i] = 0.0;}
    for (j = 0; j < 1024; j++){
            g_vi_tune.adc_array_low_current[i] = 0.0;}

    g_vi_tune.adc_average = 0.0;
    g_vi_tune.adc_average_low_current = 0.0;

    for (i = 0; i < CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ; i++)
        g_vi_tune.verify_charge_v_adc[i] = 0.0;

    for (i = 0; i < CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ; i++)
            g_vi_tune.verify_discharge_v_adc[i] = 0.0;

    for (i = 0; i < CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ; i++)
        g_vi_tune.verify_charge_i_adc[i] = 0.0;

    for (i = 0; i < CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ; i++)
            g_vi_tune.verify_discharge_i_adc[i] = 0.0;

    g_vi_tune.channel = 0;
    g_vi_tune.mode = kNone;
    g_vi_tune.command = 100;
    g_vi_tune.start = 0.0;
    g_vi_tune.end = 0.0;
    g_vi_tune.force_cur = 0.0;
    g_vi_tune.force_fine_cur = 0.0;
    g_vi_tune.force_stop = 0;
    g_vi_tune.force_loop_stop = 0;
    g_vi_tune.loop = 10000;     //Must be greater than 1000
    g_vi_tune.loop_temp = 1000;
    g_vi_tune.loop_count = 0;
    g_vi_tune.slope = CONTROL_47K;
    g_vi_tune.gate_mode = 0;
    g_vi_tune.gate_offset = 0.0;
    g_vi_tune.result = kOk;

    g_vi_tune.test_I_Fine_Current = 0.0;
    g_vi_tune.test_I_Coarse_Current = 0.0;
    g_vi_tune.test_Current_Offset_Charge = 0.0;
    g_vi_tune.test_Current_Offset_Discharge = 0.0;
}

void RunVIADCTune(void)
{
    while(1){
        ProcessVADCTune();
        ProcessIADCTune();
    }
}

void ProcessVADCTune(void)
{
    if (g_vi_tune.mode == kVoltage) {
        switch (g_vi_tune.command)
        {
            case 0:  //0V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 0, CONTROL_CHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 0, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 1:  //1V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 1, CONTROL_CHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 1, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 2:  //2V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 2, CONTROL_CHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 2, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 3:  //3V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 3, CONTROL_CHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 3, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 4:  //4V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 4, CONTROL_CHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 4, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 5:  //5V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 5, CONTROL_CHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 5, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 10:  //0V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 0, CONTROL_DISCHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 0, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 11:  //1V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 1, CONTROL_DISCHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 1, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 12:  //2V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 2, CONTROL_DISCHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 2, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 13:  //3V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 3, CONTROL_DISCHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 3, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 14:  //4V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 4, CONTROL_DISCHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 4, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 15:  //5V Calibration & Verify
                TuneVADC(s_Operation, g_vi_tune.channel, 5, CONTROL_DISCHARGE);
                VerifyVADC(s_Operation, g_vi_tune.channel, 5, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 50:    //Alarm reset
                OPERATION_AlramReset(s_Operation, g_vi_tune.channel);
                g_vi_tune.command = 100;
                break;
            case 51:    //Calibration value reset
                InitVIADCTuneProperties();
                g_vi_tune.command = 100;
                break;
            default:
                break;
        }

        OPERATION_Sensing(s_Operation, g_vi_tune.channel);
        if (OPERATION_Alram(s_Operation, g_vi_tune.channel)) {
            OPERATION_Stop(s_Operation, g_vi_tune.channel);
            s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
        }
    }
}

void ProcessIADCTune(void)
{
    float32 i;

    if (g_vi_tune.mode == kCurrent) {
        switch (g_vi_tune.command)
        {

        // 0.1A ~ 1.1A
            case 1:  //charge 1 Low value find
                TuneLowIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 2:  //charge 1 Low value check
                VerifyLowIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 3:  //discharge 1 Low value find
                TuneLowIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 4:  //discharge 1 Low value check
                VerifyLowIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;

            case 5:    //Low Range loop
                for (i = g_vi_tune.start; i <= g_vi_tune.end; i += 0.1) {
                    TuneLowIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    TuneLowIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                g_vi_tune.command = 100;
                break;
            case 6:    //Low Range verify loop
                for (i = g_vi_tune.start; i <= g_vi_tune.end; i += 0.1) {
                    VerifyLowIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    VerifyLowIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                g_vi_tune.command = 100;
                break;
//
//            case 7:    // 100mA verify loop
//                for (i = g_vi_tune.start; i <= g_vi_tune.end; i += 0.0001) {
//                    VerifyLowIADC_TEST(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
//                    DELAY_US(100000);
//                    VerifyLowIADC_TEST(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
//                    DELAY_US(100000);
//                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
//                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
//                        g_vi_tune.force_loop_stop = 0;
//                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
//                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
//                        break;
//                    }
//                }
//                g_vi_tune.command = 100;
//                break;

            case 8:
                TuneForceFINEIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_CHARGE );
                g_vi_tune.command = 100;
                break;
            case 9:
                TuneForceFINEIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_DISCHARGE );
                g_vi_tune.command = 100;
                break;
//            case 2:
//                Low_Current_ADC_Sensing_TEST(s_Operation, g_vi_tune.channel);
//                   g_vi_tune.command = 100;
//                   break;
//
//            case 2:
//                ICoarseTest(s_Operation, g_vi_tune.channel, g_vi_tune.test_I_Coarse_Current);
//                g_vi_tune.command = 100;
//                break;

            //Original Range
            // 10A ~ 50A
            case 11:  //charge 1 value find
                TuneIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 12:  //charge 1 value check
                VerifyIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_CHARGE);
                g_vi_tune.command = 100;
                break;
            case 13:  //discharge 1 value find
                TuneIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;
            case 14:  //discharge 1 value check
                VerifyIADC(s_Operation, g_vi_tune.channel, g_vi_tune.start, CONTROL_DISCHARGE);
                g_vi_tune.command = 100;
                break;

            case 15:    //10A Range Tune loop
                for (i = g_vi_tune.start; i <= g_vi_tune.end; i += 10.0) {
                    TuneIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    TuneIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                g_vi_tune.command = 100;
                break;
            case 16:    //5A Range verify loop
                for (i = g_vi_tune.start; i <= g_vi_tune.end; i += 5.0) {
                    VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                g_vi_tune.command = 100;
                break;


            case 20:    //1A Range verify loop
                for (i = g_vi_tune.start; i <= g_vi_tune.end; i += 1.0) {
                    VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                g_vi_tune.command = 100;
                break;

            case 30: // turn key tune ( 0.1~1.1, 10~50 )
                for (i = 0.1; i <= 1.2; i += 0.1) {
                    TuneLowIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    TuneLowIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                for (i = 10; i <= 50; i += 10.0) {
                    TuneIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    TuneIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }


                g_vi_tune.command = 100;
                break;

            case 31:// turn key Verify ( 0.1~1.1, 10~50 )
                VerifyLowIADC(s_Operation, g_vi_tune.channel, 1, CONTROL_CHARGE);
                DELAY_US(100000);
                VerifyLowIADC(s_Operation, g_vi_tune.channel, 1, CONTROL_DISCHARGE);
                DELAY_US(100000);
                TOTAL_CHECK_ADC = 1;
                for (i = 2; i <= 5; i += 1.0) {
                     VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                     DELAY_US(100000);
                     VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                     DELAY_US(100000);
                     OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                     if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                         g_vi_tune.force_loop_stop = 0;
                         OPERATION_Stop(s_Operation, g_vi_tune.channel);
                         s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                         break;
                     }
                 }
                TOTAL_CHECK_ADC = 2;
                for (i = 10; i <= 50; i += 5.0) {
                    VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_CHARGE);
                    DELAY_US(100000);
                    VerifyIADC(s_Operation, g_vi_tune.channel, i, CONTROL_DISCHARGE);
                    DELAY_US(100000);
                    OPERATION_Sensing(s_Operation, g_vi_tune.channel);
                    if (OPERATION_Alram(s_Operation, g_vi_tune.channel) || g_vi_tune.force_loop_stop == 1) {
                        g_vi_tune.force_loop_stop = 0;
                        OPERATION_Stop(s_Operation, g_vi_tune.channel);
                        s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
                        break;
                    }
                }
                TOTAL_CHECK_ADC = 0;
                g_vi_tune.command = 100;
                break;

            case 50:    //Alarm reset
                OPERATION_AlramReset(s_Operation, g_vi_tune.channel);
                g_vi_tune.command = 100;
                break;
            case 51:    //Calibration value reset
                InitVIADCTuneProperties();
                g_vi_tune.command = 100;
                break;
            default:
                break;
        }
        OPERATION_Sensing(s_Operation, g_vi_tune.channel);
        if (OPERATION_Alram(s_Operation, g_vi_tune.channel)) {
            OPERATION_Stop(s_Operation, g_vi_tune.channel);
            s_Operation[g_vi_tune.channel].C_IRELAY = CONTROL_CurrentRELAY(g_vi_tune.channel, CONTROL_OFF);
        }
    }
}

Uint16 TuneVADC(S_OPERATION_FILE *sp_OP, Uint16 channel, Uint16 voltage, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    Uint32 i, j = 0;

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (voltage > 5) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    if (mode == CONTROL_CHARGE) {
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);
    } else {
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_DISCHARGE);
    }

    g_vi_tune.loop_temp = g_vi_tune.loop / 1000;
    if (g_vi_tune.loop / g_vi_tune.loop_temp < 1000) {
        return g_vi_tune.result = kWrongParam;
    }

    for (i = 0; i < g_vi_tune.loop; i++) {

        g_vi_tune.loop_count = i;

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            g_vi_tune.force_stop = 0;
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            return g_vi_tune.result = kOperationFail;
        }
        if ((i % g_vi_tune.loop_temp) == 0) {
            g_vi_tune.adc_array[j++] = ABS_F(sp_OP[channel].s_ADCData.SENSING_Bat_Voltage);
        }
    }

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    InsertionSort(g_vi_tune.adc_array, 999);

    g_vi_tune.adc_sum = 0.0;
    j = 0;

    for (i = 350; i < 750; i++, j++) {
        g_vi_tune.adc_sum += g_vi_tune.adc_array[i];
    }

    g_vi_tune.adc_average = g_vi_tune.adc_sum / (float) (j);

    if (mode == CONTROL_CHARGE) {
        VoltageCharge_ADC[channel][voltage] = g_vi_tune.adc_average;
    } else {
        VoltageDischarge_ADC[channel][voltage] = g_vi_tune.adc_average;
    }

    return g_vi_tune.result = kOk;
}

Uint16 VerifyVADC(S_OPERATION_FILE *sp_OP, Uint16 channel, Uint16 voltage, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    Uint32 i, j = 0;

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (voltage > 5) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    g_vi_tune.loop_temp = g_vi_tune.loop / 1000;
    if (g_vi_tune.loop / g_vi_tune.loop_temp < 1000) {
        return g_vi_tune.result = kWrongParam;
    }

    if (mode == CONTROL_CHARGE) {
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);
    } else {
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_DISCHARGE);
    }

    if (mode == CONTROL_CHARGE) {
        sp_OP[channel].COMMAND = COM_CHARGE;
    } else {
        sp_OP[channel].COMMAND = COM_DISCHARGE;
    }

    for (i = 0; i < g_vi_tune.loop; i++) {

        g_vi_tune.loop_count = i;

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            g_vi_tune.force_stop = 0;
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            sp_OP[channel].COMMAND = COM_NONE;
            return g_vi_tune.result = kOperationFail;
        }

        if ((i % g_vi_tune.loop_temp) == 0) {
            g_vi_tune.adc_array[j++] = ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage);
        }
    }

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
    sp_OP[channel].COMMAND = COM_NONE;

    InsertionSort(g_vi_tune.adc_array, 999);

    g_vi_tune.adc_sum = 0.0;
    j = 0;

    for (i = 350; i < 750; i++, j++) {
        g_vi_tune.adc_sum += g_vi_tune.adc_array[i];
    }

    g_vi_tune.adc_average = g_vi_tune.adc_sum / (float) (j);

    if (mode == CONTROL_CHARGE) {
        g_vi_tune.verify_charge_v_adc[voltage] = g_vi_tune.adc_average;
    } else {
        g_vi_tune.verify_discharge_v_adc[voltage] = g_vi_tune.adc_average;
    }

    return g_vi_tune.result = kOk;
}

Uint16 TuneIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    Uint32 i, j = 0;
    Uint16 index;

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (current > 52.0) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    g_vi_tune.loop_temp = g_vi_tune.loop / 1000;
    if (g_vi_tune.loop / g_vi_tune.loop_temp < 1000) {
        return g_vi_tune.result = kWrongParam;
    }

    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
    DELAY_US(1000);

    ControlInitial(sp_OP, channel, mode);               //1. Initial
    ControlSlopMux(sp_OP, channel, current, mode);      //2. Slope change

    ControlGateOffset(sp_OP, channel, current, mode);   //3. Gate Offset

    ControlCurrentCoarseFineDac(sp_OP, channel, current);
    ControlVoltageDac(sp_OP, channel, mode);            //5. Voltage DAC ON



    DELAY_US(1000000);  //wait 1s

    for (i = 0; i < g_vi_tune.loop; i++) {

        g_vi_tune.loop_count = i;

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }

        if ((i % g_vi_tune.loop_temp) == 0) {
            g_vi_tune.adc_array[j++] = ABS_F(sp_OP[channel].s_ADCData.SENSING_Current);
        }
    }

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    InsertionSort(g_vi_tune.adc_array, 999);

    g_vi_tune.adc_sum = 0.0;
    j = 0;

    for (i = 350; i < 750; i++, j++) {
        g_vi_tune.adc_sum += g_vi_tune.adc_array[i];
    }

    g_vi_tune.adc_average = g_vi_tune.adc_sum / (float) (j);

    index = (Uint16) (current/10);

    if (mode == CONTROL_CHARGE) {
        CurrentCharge_ADC[channel][index] = g_vi_tune.adc_average;
    } else {
        CurrentDischarge_ADC[channel][index] = g_vi_tune.adc_average;
    }

    return g_vi_tune.result = kOk;
}

Uint16 TuneForceIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (current > 33.0) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
    DELAY_US(1000);

    ControlInitial(sp_OP, channel, mode);                   //1. Initial
    //ControlSlopMux(sp_OP, channel, current, mode);          //2. Slope change
    ControlGateOffset(sp_OP, channel, current, mode);       //3. Gate Offset
    //ControlCurrentDac(sp_OP, channel, current);             //4. Current DAC ON
    ControlVoltageDac(sp_OP, channel, mode);                //5. Voltage DAC ON

    g_vi_tune.force_cur = 0.0;
    g_vi_tune.slope = CONTROL_47K;

    while (1) {

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }
        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, g_vi_tune.slope);
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, g_vi_tune.force_cur);
    }
}

Uint16 VerifyIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    Uint32 i, j = 0;
    Uint32 Temp32;
    float32 f_Temp32 = 0.0;
    Uint16 index;

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (current > 52.0) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    g_vi_tune.loop_temp = g_vi_tune.loop / 1000;
    if (g_vi_tune.loop / g_vi_tune.loop_temp < 1000) {
        return g_vi_tune.result = kWrongParam;
    }

    if (mode == CONTROL_CHARGE) {
        sp_OP[channel].COMMAND = COM_CHARGE;
    } else {
        sp_OP[channel].COMMAND = COM_DISCHARGE;
    }

    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
    DELAY_US(1000);

    ControlInitial(sp_OP, channel, mode);               //1. Initial
    ControlSlopMux(sp_OP, channel, current, mode);      //2. Slope change
    ControlGateOffset(sp_OP, channel, current, mode);   //3. Gate Offset

    if (mode == CONTROL_CHARGE) {
//        Temp32 = (Uint32) (CALIBRATION_ConversionREF(current,
        f_Temp32 = (CALIBRATION_ConversionREF(current,
                                                     CurrentCharge_REF[channel],
                                                     CALIBRATION_CURRENT_MIN_VALUE,
                                                     CALIBRATION_CURRENT_UNIT_VALUE,
                                                     CALIBRATION_CURRENT_MAX_ARRAY_LENGTH) * 1000.0);
    } else {
//        Temp32 = (Uint32) (CALIBRATION_ConversionREF(current,
        f_Temp32 = (CALIBRATION_ConversionREF(current,
                                                     CurrentDischarge_REF[channel],
                                                     CALIBRATION_CURRENT_MIN_VALUE,
                                                     CALIBRATION_CURRENT_UNIT_VALUE,
                                                     CALIBRATION_CURRENT_MAX_ARRAY_LENGTH) * 1000.0);
    }

//    ControlCurrentDacForITune(sp_OP, channel, f_Temp32);  //4. Current DAC ON
    ControlCurrentCoarseFineDacForITune(sp_OP, channel, f_Temp32);  //4. Current DAC ON
    ControlVoltageDac(sp_OP, channel, mode);            //5. Voltage DAC ON

    DELAY_US(1000000);            //wait 1s

    for (i = 0; i < g_vi_tune.loop; i++) {

        g_vi_tune.loop_count = i;

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            sp_OP[channel].COMMAND = COM_NONE;
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }

        if ((i % g_vi_tune.loop_temp) == 0) {
            g_vi_tune.adc_array[j++] = ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current);
        }
    }

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
    sp_OP[channel].COMMAND = COM_NONE;

    InsertionSort(g_vi_tune.adc_array, 999);

    g_vi_tune.adc_sum = 0.0;
    j = 0;

    for (i = 350; i < 750; i++, j++) {
        g_vi_tune.adc_sum += g_vi_tune.adc_array[i];
    }

    g_vi_tune.adc_average = g_vi_tune.adc_sum / (float) (j);

    if(TOTAL_CHECK_ADC == 1)
    {
        index = (current / 1);
    }
    else if(TOTAL_CHECK_ADC == 2)
    {

        index = 5 + (Uint16) (current / 5);
    }
    else
    {
        index = (Uint16) (current / 1);
//        index = (Uint16) (current / 5);
    }

    if (mode == CONTROL_CHARGE) {
        g_vi_tune.verify_charge_i_adc[index] = g_vi_tune.adc_average;
    } else {
        g_vi_tune.verify_discharge_i_adc[index] = g_vi_tune.adc_average;
    }

    return g_vi_tune.result = kOk;
}

Uint16 TuneLowIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    Uint32 i, j = 0;
    Uint32 k = 0;
    Uint16 index;

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (current > 1.2) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    g_vi_tune.loop_temp = g_vi_tune.loop / 1000;
    if (g_vi_tune.loop / g_vi_tune.loop_temp < 1000) {
        return g_vi_tune.result = kWrongParam;
    }

    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
    DELAY_US(1000);

    ControlInitial(sp_OP, channel, mode);               //1. Initial
    ControlSlopMux(sp_OP, channel, current, mode);      //2. Slope change
    ControlGateOffset(sp_OP, channel, current, mode);   //3. Gate Offset
//    ControlCurrentDac(sp_OP, channel, current);         //4. Current DAC ON
    ControlCurrentCoarseFineDac(sp_OP, channel, current);   //4. Current DAC ON
    ControlVoltageDac(sp_OP, channel, mode);            //5. Voltage DAC ON

    DELAY_US(1000000);  //wait 1s

    for (i = 0; i < g_vi_tune.loop; i++) {

        g_vi_tune.loop_count = i;

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }

        if ((i % g_vi_tune.loop_temp) == 0) {
            g_vi_tune.adc_array[j++] = ABS_F(sp_OP[channel].s_ADCData.SENSING_Current);
        }
    }

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    InsertionSort(g_vi_tune.adc_array, 999);
    InsertionSort(g_vi_tune.adc_array_low_current, 999);

    g_vi_tune.adc_sum = 0.0;
    g_vi_tune.adc_sum_low_current = 0.0;
    j = 0;
    k = 0;

    for (i = 350; i < 750; i++, j++, k++) {
        g_vi_tune.adc_sum += g_vi_tune.adc_array[i];
    }

    g_vi_tune.adc_average = g_vi_tune.adc_sum / (float) (j);

    index = (Uint16) (current * 10);

    if (mode == CONTROL_CHARGE) {
        CurrentCharge_Low_ADC[channel][index] = g_vi_tune.adc_average;

    } else {
        CurrentDischarge_Low_ADC[channel][index] = g_vi_tune.adc_average;
    }

    return g_vi_tune.result = kOk;
}

Uint16 VerifyLowIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    Uint32 i, j = 0;
    Uint16 index;
    float32 f_Temp32 = 0.0;

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (current > 1.2) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    g_vi_tune.loop_temp = g_vi_tune.loop / 1000;
    if (g_vi_tune.loop / g_vi_tune.loop_temp < 1000) {
        return g_vi_tune.result = kWrongParam;
    }

    if (mode == CONTROL_CHARGE) {
        sp_OP[channel].COMMAND = COM_CHARGE;
    } else {
        sp_OP[channel].COMMAND = COM_DISCHARGE;
    }

    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
    DELAY_US(1000);

    ControlInitial(sp_OP, channel, mode);               //1. Initial
    ControlSlopMux(sp_OP, channel, current, mode);      //2. Slope change
    ControlGateOffset(sp_OP, channel, current, mode);   //3. Gate Offset

    if (mode == CONTROL_CHARGE) {
        f_Temp32 = CALIBRATION_ConversionREF(current,
                                                     CurrentCharge_Low_REF[channel],
                                                     CALIBRATION_LOW_CURRENT_MIN_VALUE,
                                                     CALIBRATION_LOW_CURRENT_UNIT_VALUE,
                                                     CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH) * 1000;
    } else {
        f_Temp32 = CALIBRATION_ConversionREF(current,
                                                     CurrentDischarge_Low_REF[channel],
                                                     CALIBRATION_LOW_CURRENT_MIN_VALUE,
                                                     CALIBRATION_LOW_CURRENT_UNIT_VALUE,
                                                     CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH) * 1000;
    }

    ControlCurrentCoarseFineDacForITune(sp_OP, channel, f_Temp32);
    ControlVoltageDac(sp_OP, channel, mode);            //5. Voltage DAC ON

    DELAY_US(1000000);            //wait 1s

    for (i = 0; i < g_vi_tune.loop; i++) {

        g_vi_tune.loop_count = i;

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            sp_OP[channel].COMMAND = COM_NONE;
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }

        if ((i % g_vi_tune.loop_temp) == 0) {
            g_vi_tune.adc_array[j++] = ABS_F(sp_OP[channel].s_OldADCData.SENSING_Current);
        }
    }

    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
    sp_OP[channel].COMMAND = COM_NONE;

    InsertionSort(g_vi_tune.adc_array, 999);

    g_vi_tune.adc_sum = 0.0;
    j = 0;

    for (i = 350; i < 750; i++, j++) {
        g_vi_tune.adc_sum += g_vi_tune.adc_array[i];
    }

    g_vi_tune.adc_average = g_vi_tune.adc_sum / (float) (j);

    index = (Uint16) (current * 10);

    if (mode == CONTROL_CHARGE) {
        g_vi_tune.verify_charge_i_adc[index] = g_vi_tune.adc_average;
    } else {
        g_vi_tune.verify_discharge_i_adc[index] = g_vi_tune.adc_average;
    }

    return g_vi_tune.result = kOk;
}

void InsertionSort(float *Data, int Number)
{
    int i, j;
    float Key;

    for (i = 1; i <= Number; i++) {
        Key = Data[i];
        j = i - 1;
        while ((j >= 0) && (Data[j] > Key)) {
            Data[j + 1] = Data[j];
            j--;
        }
        Data[j + 1] = Key;
    }
}

void ControlInitial(S_OPERATION_FILE *sp_OP, Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, (float) (0.0));
    sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, (float) (0.0));

    sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel,(float) (0.0));
    sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel, (float) (0.0));

    sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_OPERATION_INIT);
    sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_OPERATION_INIT);
    sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);

    sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_ON);
    if (mode == CONTROL_CHARGE) {
        DELAY_US(OPERATION_INTEGRATION_RESET_TIME_US_CHA);
    } else {
        DELAY_US(OPERATION_INTEGRATION_RESET_TIME_US_DIS);
    }
    sp_OP[channel].C_IntResetMUX = CONTROL_IntegrationResetMUX(channel, CONTROL_OFF);
}

void ControlSlopMux(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    if (mode == CONTROL_CHARGE) {
        if (current <= 0.5) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_1K);              // 1k
        } else if (current <= 2.0) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_3R9K);            // 3.9k
        } else {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_13K);            // 13k
        }
    } else {
        if (current <= 0.5) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_1K);            // 1k
        } else if (current <= 2.0) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_3R9K);             // 3.9k
        } else if (current <= 7.0) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_13K);             // 13k
        } else if (current <= 15.0) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_24K);             // 24k
        } else if (current <= 25.0) {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_36K);             // 36k
        } else {
            sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, CONTROL_47K);             // 47k
        }
    }
}


void ControlGateOffset(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    float32 gate_offset = 0.0;

    if (g_vi_tune.gate_mode == 0) {

        if (mode == CONTROL_CHARGE) {
#if 0
            gate_offset = (float) ((30.0 - current) * 0.006) + ((5.0 - ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage)) * 0.5);
#else
            if(current < 0.5) {
                gate_offset = 3.0;
            } else{
                gate_offset = 5.0;
            }
#endif
        } else {
#if 0
            gate_offset = (float) ((30.0 - current) * 0.008) + (ABS_F(sp_OP[channel].s_OldADCData.SENSING_Bat_Voltage) * 1.0);
#else
            gate_offset = 0.0;
#endif
        }

        sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel, gate_offset);

    } else {
        sp_OP[channel].C_GateOffsetDAC = CONTROL_GateOffsetDAC(channel, g_vi_tune.gate_offset);
    }
}

void ControlCurrentDacForITune(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current)
{
    Uint32 Temp32 = 0;
    float32 current_temp;

    Temp32 = (Uint32)current;

    current_temp = (float)(Temp32) * (float)(0.001);

    sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, current_temp);
}

void ControlCurrentDac(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current)
{
    Uint32 Temp32 = 0;
    float32 current_temp;

    Temp32 = (Uint32) (current * 1000.0);

    current_temp = (float)(Temp32) * (float)(0.001);

    sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, current_temp);
}

void ControlVoltageDac(S_OPERATION_FILE *sp_OP, Uint16 channel, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    if (mode == CONTROL_CHARGE) {
        sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, OPERATION_LIMIT_MAX_VOLTAGE);
        sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_CHARGE);
        sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_CHARGE);
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_CHARGE);
    } else {
        sp_OP[channel].C_VRefDAC = CONTROL_VoltageReferenceDAC(channel, OPERATION_LIMIT_MIN_VOLTAGE * (float) (-1.0));
        sp_OP[channel].C_GateOutputAMP = CONTROL_GateOutputAMP(channel, CONTROL_DISCHARGE);
        sp_OP[channel].C_GateOutputMUX = CONTROL_GateOutputMUX(channel, CONTROL_DISCHARGE);
        sp_OP[channel].C_OperationMUX = CONTROL_OperationMUX(channel, CONTROL_DISCHARGE);
    }
}

void ControlCurrentCoarseFineDac(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current)  // Tune함수용 - 6차
{
    Uint32 Temp32 = 0;
    Uint32 FineTemp32 = 0;
    float32 f_Temp32 = 0.0;
    float32 Coarse_current_temp;
    float32 Fine_current_temp;

    Temp32 = (Uint32) (current * 1000.0);   // 현재는 mA까지 밖에 설정 안됨
    f_Temp32 = current * 1000.0;

#if(LOW_CAL_VERSION == 0)
    if (Temp32 <= OPERATION_CURRENT_FINE_CONTROL_RANGE)
    {
        Fine_current_temp = f_Temp32 * 0.001;
        Coarse_current_temp = 0.0;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32 - OPERATION_CURRENT_FINE_OFFSET) % OPERATION_CURRENT_MOD;
        FineTemp32 = FineTemp32 + OPERATION_CURRENT_FINE_OFFSET;
        Temp32 = Temp32 - FineTemp32;

          Fine_current_temp = (float) FineTemp32 * (float) (0.001);

          Coarse_current_temp = (float) (Temp32) * (float) (0.001);

          sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
          sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }

#elif(LOW_CAL_VERSION == 1)
    if (Temp32 <= OPERATION_CURRENT_FINE_CONTROL_RANGE)
    {
        Coarse_current_temp = 0.05;                     //0.05A
        Fine_current_temp = (f_Temp32 * 0.001) - Coarse_current_temp;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else if (Temp32 <= (OPERATION_CURRENT_FINE_CONTROL_RANGE + 100))
    {
        Fine_current_temp = f_Temp32 * 0.001 * 0.85;
        Coarse_current_temp = f_Temp32 * 0.001 * 0.15;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32 - OPERATION_CURRENT_FINE_OFFSET) % OPERATION_CURRENT_MOD;
        FineTemp32 = FineTemp32 + OPERATION_CURRENT_FINE_OFFSET;
        Temp32 = Temp32 - FineTemp32;

          Fine_current_temp = (float) FineTemp32 * (float) (0.001);

          Coarse_current_temp = (float) (Temp32) * (float) (0.001);

          sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
          sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
#elif(LOW_CAL_VERSION == 2)
    if (Temp32 <= 100)
    {
        Fine_current_temp = f_Temp32 * 0.001;
        Coarse_current_temp = 0.0;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else if (Temp32 <= OPERATION_CURRENT_FINE_CONTROL_RANGE)
    {
        Coarse_current_temp = 0.05;                     //0.05A
        Fine_current_temp = (f_Temp32 * 0.001) - Coarse_current_temp;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else if (Temp32 <= (OPERATION_CURRENT_FINE_CONTROL_RANGE + 100))
    {
        Fine_current_temp = f_Temp32 * 0.001 * 0.85;
        Coarse_current_temp = f_Temp32 * 0.001 * 0.15;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32 - OPERATION_CURRENT_FINE_OFFSET) % OPERATION_CURRENT_MOD;
        FineTemp32 = FineTemp32 + OPERATION_CURRENT_FINE_OFFSET;
        Temp32 = Temp32 - FineTemp32;

          Fine_current_temp = (float) FineTemp32 * (float) (0.001);

          Coarse_current_temp = (float) (Temp32) * (float) (0.001);

          sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
          sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
#elif(LOW_CAL_VERSION == 3)
    if (Temp32 < OPERATION_CURRENT_FINE_CONTROL_RANGE + 100)
     {
         Coarse_current_temp = f_Temp32 * 0.001 * 0.2;
         Fine_current_temp = f_Temp32 * 0.001 * 0.8;
         //차성호
         sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
         sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
     }
     else
     {
         FineTemp32 = (Temp32 - OPERATION_CURRENT_FINE_OFFSET) % OPERATION_CURRENT_MOD;
         FineTemp32 = FineTemp32 + OPERATION_CURRENT_FINE_OFFSET;
         Temp32 = Temp32 - FineTemp32;

           Fine_current_temp = (float) FineTemp32 * (float) (0.001);

           Coarse_current_temp = (float) (Temp32) * (float) (0.001);

           sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
           sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
     }
#elif(LOW_CAL_VERSION == 4)
    if (Temp32 < (OPERATION_CURRENT_FINE_CONTROL_RANGE + 200))
    {
        Coarse_current_temp = 0.2;                     //0.2A
        Fine_current_temp = (f_Temp32 * 0.001) - Coarse_current_temp;

        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32 - OPERATION_CURRENT_FINE_OFFSET) % OPERATION_CURRENT_MOD;
        FineTemp32 = FineTemp32 + OPERATION_CURRENT_FINE_OFFSET;
        Temp32 = Temp32 - FineTemp32;

          Fine_current_temp = (float) FineTemp32 * (float) (0.001);

          Coarse_current_temp = (float) (Temp32) * (float) (0.001);

          sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
          sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
#endif
}

void ControlCurrentCoarseFineDacForITune(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current)      // 6차
{
    Uint32 Temp32 = 0;
    Uint32 Temp32_x100 = 0;
    Uint32 FineTemp32 = 0;
    float32 f_Temp32 = 0.0;
    float32 Coarse_current_temp = 0.0;
    float32 Fine_current_temp = 0.0;

    Temp32 = (Uint32)current;
    f_Temp32 = current;
    Temp32_x100 = (Uint32) (f_Temp32 * 100);

#if(LOW_CAL_VERSION == 0)
    if (Temp32 <= OPERATION_CURRENT_FINE_CONTROL_RANGE)
        {
            Fine_current_temp = f_Temp32 * 0.001;
            Coarse_current_temp = 0.0;

            sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
            sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);

        }
        else
        {
            FineTemp32 = (Temp32_x100 - OPERATION_CURRENT_FINE_OFFSET * 100) % (OPERATION_CURRENT_MOD * 100);
            FineTemp32 = FineTemp32 + (OPERATION_CURRENT_FINE_OFFSET * 100);
            Temp32_x100 = Temp32_x100 - FineTemp32;

            Fine_current_temp = (float32) FineTemp32 * (float32) (0.00001);

            Coarse_current_temp = (float32) (Temp32_x100) * (float32) (0.00001);

            sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
            sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);

        }
#elif(LOW_CAL_VERSION == 1)
    if (Temp32 <= OPERATION_CURRENT_FINE_CONTROL_RANGE)
    {
        Coarse_current_temp = 0.05;                     //0.05A
        Fine_current_temp = (f_Temp32 * 0.001) - Coarse_current_temp;

        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else if (Temp32 <= (OPERATION_CURRENT_FINE_CONTROL_RANGE + 100))
    {
        Fine_current_temp = f_Temp32 * 0.001 * 0.85;
        Coarse_current_temp = f_Temp32 * 0.001 * 0.15;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32_x100 - OPERATION_CURRENT_FINE_OFFSET * 100) % (OPERATION_CURRENT_MOD * 100);
        FineTemp32 = FineTemp32 + (OPERATION_CURRENT_FINE_OFFSET * 100);
        Temp32_x100 = Temp32_x100 - FineTemp32;

        Fine_current_temp = (float32) FineTemp32 * (float32) (0.00001);

        Coarse_current_temp = (float32) (Temp32_x100) * (float32) (0.00001);

        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }

#elif(LOW_CAL_VERSION == 2)
    if (Temp32 <= 100)
    {
        Fine_current_temp = f_Temp32 * 0.001;
        Coarse_current_temp = 0.0;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else if (Temp32 <= OPERATION_CURRENT_FINE_CONTROL_RANGE)
    {
        Coarse_current_temp = 0.05;                     //0.05A
        Fine_current_temp = (f_Temp32 * 0.001) - Coarse_current_temp;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else if (Temp32 <= (OPERATION_CURRENT_FINE_CONTROL_RANGE + 100))
    {
        Fine_current_temp = f_Temp32 * 0.001 * 0.85;
        Coarse_current_temp = f_Temp32 * 0.001 * 0.15;
        //차성호
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32_x100 - OPERATION_CURRENT_FINE_OFFSET * 100) % (OPERATION_CURRENT_MOD * 100);
        FineTemp32 = FineTemp32 + (OPERATION_CURRENT_FINE_OFFSET * 100);
        Temp32_x100 = Temp32_x100 - FineTemp32;

        Fine_current_temp = (float32) FineTemp32 * (float32) (0.00001);

        Coarse_current_temp = (float32) (Temp32_x100) * (float32) (0.00001);

        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
#elif(LOW_CAL_VERSION == 3)
    if (Temp32 < OPERATION_CURRENT_FINE_CONTROL_RANGE + 100)
     {
         Coarse_current_temp = f_Temp32 * 0.001 * 0.2;
         Fine_current_temp = f_Temp32 * 0.001 * 0.8;

         sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
         sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
     }
     else
     {
         FineTemp32 = (Temp32_x100 - OPERATION_CURRENT_FINE_OFFSET * 100) % (OPERATION_CURRENT_MOD * 100);
         FineTemp32 = FineTemp32 + (OPERATION_CURRENT_FINE_OFFSET * 100);
         Temp32_x100 = Temp32_x100 - FineTemp32;

         Fine_current_temp = (float32) FineTemp32 * (float32) (0.00001);

         Coarse_current_temp = (float32) (Temp32_x100) * (float32) (0.00001);

         sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
         sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
     }
#elif(LOW_CAL_VERSION == 4)
    if (Temp32 < (OPERATION_CURRENT_FINE_CONTROL_RANGE + 200))
    {
        Coarse_current_temp = 0.2;                     //0.2A
        Fine_current_temp = (f_Temp32 * 0.001) - Coarse_current_temp;

        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
    else
    {
        FineTemp32 = (Temp32_x100 - OPERATION_CURRENT_FINE_OFFSET * 100) % (OPERATION_CURRENT_MOD * 100);
        FineTemp32 = FineTemp32 + (OPERATION_CURRENT_FINE_OFFSET * 100);
        Temp32_x100 = Temp32_x100 - FineTemp32;

        Fine_current_temp = (float32) FineTemp32 * (float32) (0.00001);

        Coarse_current_temp = (float32) (Temp32_x100) * (float32) (0.00001);

        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, Coarse_current_temp);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, Fine_current_temp);
    }
#endif
}

Uint16 TuneForceFINEIADC(S_OPERATION_FILE *sp_OP, Uint16 channel, float32 current, E_CONTROL_OPERATION_MODE_SELECT mode)
{
    OPERATION_Stop(sp_OP, channel);
    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);

    if (current > 1.2) return g_vi_tune.result = kWrongParam;
    if (mode != CONTROL_CHARGE && mode != CONTROL_DISCHARGE) return g_vi_tune.result = kWrongParam;

    sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_ON);
    DELAY_US(1000);

    ControlInitial(sp_OP, channel, mode);                   //1. Initial
    //ControlSlopMux(sp_OP, channel, current, mode);          //2. Slope change
    ControlGateOffset(sp_OP, channel, current, mode);       //3. Gate Offset
    //ControlCurrentDac(sp_OP, channel, current);             //4. Current DAC ON
    ControlVoltageDac(sp_OP, channel, mode);                //5. Voltage DAC ON

    g_vi_tune.force_cur = 0.0;
    g_vi_tune.force_fine_cur = 0.0;

    if(mode == CONTROL_CHARGE){
        g_vi_tune.slope = CONTROL_3R9K;
    } else g_vi_tune.slope = CONTROL_13K;

    while (1) {
        if(g_vi_tune.slope == CONTROL_OPEN){
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }

        OPERATION_Sensing(sp_OP, channel);

        if (OPERATION_Alram(sp_OP, channel) || g_vi_tune.force_stop == 1) {
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kOperationFail;
        }
        if((g_vi_tune.force_fine_cur >= 0.51)||(g_vi_tune.force_cur >= 0.7)){

                   //  1Mohm일때 0.51A까지
            OPERATION_Stop(sp_OP, channel);
            sp_OP[channel].C_IRELAY = CONTROL_CurrentRELAY(channel, CONTROL_OFF);
            g_vi_tune.force_stop = 0;
            return g_vi_tune.result = kWrongParam;
        }
        sp_OP[channel].C_IntSlopMUX = CONTROL_IntegrationSlopMUX(channel, g_vi_tune.slope);
        sp_OP[channel].C_IRefDAC = CONTROL_CurrentReferenceDAC(channel, g_vi_tune.force_cur);
        sp_OP[channel].C_IFINERefDAC = CONTROL_FineCurrentReferenceDAC(channel, g_vi_tune.force_fine_cur);

    }
    return g_vi_tune.result = kOk;
}

