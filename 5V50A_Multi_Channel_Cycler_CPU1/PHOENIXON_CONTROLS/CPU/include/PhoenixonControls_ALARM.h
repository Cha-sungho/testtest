/*
 * PhoenixonControls_ALARM.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_ALARM_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_ALARM_H_

#define ALARM_OVER_VOLTAGE_LIMIT                    (float)(4.3)    //10mV
#define ALARM_VOLTAGE_LIMIT						    (float)(0.01)   //10mV

#define ALARM_REST_OVER_CURRENT_LIMIT               (float)(0.5)   //0.5A
//#define ALARM_OVER_CURRENT_LIMIT                    (float)(21.5)   //21.5A
#define ALARM_OVER_CURRENT_LIMIT                    (float)(5.5)   //51.5A
//#define ALARM_CURRENT_LIMIT                         (float)(0.5)    // 500mA
#define ALARM_CURRENT_LIMIT                         (float)(1.0)    // 1A

#define ALARM_TEMPERATURE_LIMIT	                    (float)(70.0)   //차 수정 -> 70도
#define ALARM_BATTERY_VOLTAGE_LIMIT				    (float)(0.1)    //100mV
#define ALARM_BATTERY_CURRENT_LIMIT				    (float)(0.1)    //차 수정 -> 1mA

#define ALARM_VOLTAGE_TIME						(Uint16)(100 * 2)//Time of ms
#define ALARM_CURRENT_TIME	                    (Uint16)(100 * 2)//Time of 100ms
#define ALARM_CURRENT_MASTER_TIME	            (Uint16)(1)//Time of ms
#define ALARM_CURRENT_SLAVE_TIME	            (Uint16)(1)//Time of ms
#define ALARM_REFERENCE_VOLTAGE_TIME	        (Uint16)(1)//Time of ms
#define ALARM_REFERENCEFINECURRENTMASTER_TIME	(Uint16)(1)//Time of ms
#define ALARM_REFERENCEFINECURRENTSLAVE_TIME	(Uint16)(1)//Time of ms
#define ALARM_REFERENCE_CURRENT_MASTER_TIME		(Uint16)(1)//Time of ms
#define ALARM_REFERENCE_CURRENT_SLAVE_TIME	    (Uint16)(1)//Time of ms
#define ALARM_TEMPERATURE_TIME	                (Uint16)(60000 * 2)//Time of 60s
#define ALARM_INNER_VOLTAGE_TIME	            (Uint16)(100 * 2)//Time of ms
#define ALARM_COMMUINCATION_TIME	            (Uint16)(60000)//Time of 10s
#define ALARM_CC_TIME							(Uint16)(100 * 2)
#define ALARM_CV_TIME							(Uint16)(100 * 2)


#define ALARM_NEGATIVE_02V_POWER_VALUE	        (float)(2.5)
#define ALARM_POSITIVE_07V_POWER_VALUE	        (float)(7.5)
#define ALARM_POSITIVE_05V_POWER_VALUE	        (float)(5.0)
#define ALARM_POSITIVE_15V_POWER_VALUE	        (float)(15.0)
#define ALARM_NEGATIVE_15V_POWER_VALUE	        (float)(-15.0)

#define ALARM_NEGATIVE_02V_POWER_LIMIT	        (float)(0.6)
#define ALARM_POSITIVE_07V_POWER_LIMIT	        (float)(0.6)
#define ALARM_POSITIVE_05V_POWER_LIMIT	        (float)(0.6)
#define ALARM_POSITIVE_15V_POWER_LIMIT	        (float)(0.6)
#define ALARM_NEGATIVE_15V_POWER_LIMIT	        (float)(0.6)

#define ALARM_NEGATIVE_02V_POWER_MIN	        (float)(ALARM_NEGATIVE_02V_POWER_VALUE - ALARM_NEGATIVE_02V_POWER_LIMIT)
#define ALARM_POSITIVE_07V_POWER_MIN	        (float)(ALARM_POSITIVE_07V_POWER_VALUE - ALARM_POSITIVE_07V_POWER_LIMIT)
#define ALARM_POSITIVE_05V_POWER_MIN	        (float)(ALARM_POSITIVE_05V_POWER_VALUE - ALARM_POSITIVE_05V_POWER_LIMIT)
#define ALARM_POSITIVE_15V_POWER_MIN	        (float)(ALARM_POSITIVE_15V_POWER_VALUE - ALARM_POSITIVE_15V_POWER_LIMIT)
#define ALARM_NEGATIVE_15V_POWER_MIN	        (float)(ALARM_NEGATIVE_15V_POWER_VALUE - ALARM_NEGATIVE_15V_POWER_LIMIT)

#define ALARM_NEGATIVE_02V_POWER_MAX	        (float)(ALARM_NEGATIVE_02V_POWER_VALUE + ALARM_NEGATIVE_02V_POWER_LIMIT)
#define ALARM_POSITIVE_07V_POWER_MAX	        (float)(ALARM_POSITIVE_07V_POWER_VALUE + ALARM_POSITIVE_07V_POWER_LIMIT)
#define ALARM_POSITIVE_05V_POWER_MAX	        (float)(ALARM_POSITIVE_05V_POWER_VALUE + ALARM_POSITIVE_05V_POWER_LIMIT)
#define ALARM_POSITIVE_15V_POWER_MAX	        (float)(ALARM_POSITIVE_15V_POWER_VALUE + ALARM_POSITIVE_15V_POWER_LIMIT)
#define ALARM_NEGATIVE_15V_POWER_MAX	        (float)(ALARM_NEGATIVE_15V_POWER_VALUE + ALARM_NEGATIVE_15V_POWER_LIMIT)

#define ALARM_NEGATIVE_02V_POWER_TIME	        (Uint16)(1000 * 2)//Time of 1s
#define ALARM_POSITIVE_07V_POWER_TIME	        (Uint16)(1000 * 2)//Time of 1s
#define ALARM_POSITIVE_05V_POWER_TIME	        (Uint16)(1000 * 2)//Time of 1s
#define ALARM_POSITIVE_15V_POWER_TIME	        (Uint16)(1000 * 2)//Time of 1s
#define ALARM_NEGATIVE_15V_POWER_TIME	        (Uint16)(1000 * 2)//Time of 1s

Uint16 ALARM_VoltageFault               (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_CurrentFault               (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_VolatageInnerFault         (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_VoltageReferenceFault      (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_CurrentReferenceFault      (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_CC_Fault                   (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_CV_Fault                   (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_N02VPowerFault             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_P07VPowerFault             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_P05VPowerFault             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_P15VPowerFault             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_N15VPowerFault             (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_TemperatureFault           (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_PowerConditionCheck        (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_LineConditionCheck         (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_CurrentConditionCheck      (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_VoltageConditionCheck      (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_HeatConditionCheck         (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_CommunicationConditionCheck(S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_BatteryConditionCheck      (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_BatteryFaultCheck          (S_OPERATION_FILE *sp_OP, Uint16 channel);
Uint16 ALARM_BatteryNoneCheck           (S_OPERATION_FILE *sp_OP, Uint16 channel);
void   ALARM_BatterySensingConnectorCheckOperating  (S_OPERATION_FILE *sp_OP, Uint16 channel);
void   ALARM_BatteryCurrentConnectorCheckOperating  (S_OPERATION_FILE *sp_OP, Uint16 channel);

Uint16 ALARM_Emergency                  (S_OPERATION_FILE *sp_OP, Uint16 channel);
#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_ALARM_H_ */
