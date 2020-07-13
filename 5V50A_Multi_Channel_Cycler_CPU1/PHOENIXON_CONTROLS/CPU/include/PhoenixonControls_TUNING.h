/*
 * PhoenixonControls_TUNING.h
 *
 *  Created on: 2019. 9. 19.
 *      Author: HW Team
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_TUNING_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_TUNING_H_

typedef enum
{
    kOk,
    kWrongParam,
    kOperationFail,
    kNotFound
} TuneResult;

typedef enum
{
    kNone,
    kCurrent,
    kVoltage
} VITuneMode;

typedef struct
{
    float32 adc_sum;
    float32 adc_sum_low_current;
    float32 adc_array[1024];
    float32 adc_array_low_current[1024];
    float32 adc_average;
    float32 adc_average_low_current;
    float32 verify_charge_v_adc[CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH];
    float32 verify_discharge_v_adc[CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH];
//    float32 verify_charge_i_adc[CALIBRATION_CURRENT_MAX_ARRAY_LENGTH];
//    float32 verify_discharge_i_adc[CALIBRATION_CURRENT_MAX_ARRAY_LENGTH];
    float32 verify_charge_i_adc[51];
    float32 verify_discharge_i_adc[51];
    Uint16 channel;
    VITuneMode mode;
    Uint16 command;
    float32 start;
    float32 end;
    float32 force_cur;
    float32 force_fine_cur;
    Uint16 force_stop;
    Uint16 force_loop_stop;
    Uint32 loop;
    Uint32 loop_temp;
    Uint32 loop_count;
    E_CONTROL_SLOP_SELECT slope;
    Uint16 gate_mode;
    float32 gate_offset;
    TuneResult result;

    float32 test_I_Coarse_Current;
    float32 test_I_Fine_Current;

    float32 test_Current_Offset_Charge;
    float32 test_Current_Offset_Discharge;
} VITuneProperties;

extern VITuneProperties g_vi_tune;

void InitVIADCTuneProperties(void);
void RunVIADCTune(void);

#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_TUNING_H_ */
