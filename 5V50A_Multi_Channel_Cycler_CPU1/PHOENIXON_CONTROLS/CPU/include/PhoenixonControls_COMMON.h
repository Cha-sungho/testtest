/*
 * PhoenixonControls_COMMON.h
 *
 *  Created on: 2018. 5. 20.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_COMMON_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_COMMON_H_

#define VI_ADC_TUNE 0

#define PID 1
//
//#define LOW_CURRENT_CALIBRATION 1

#define ALARM_ACTIVATE 1                // 1 : EMO SW 키기, 0 : EMO SW 끄기

//#define LOW_CURRENT_ADC_ACTIVATE 0

#define LOW_CAL_VERSION 4               // 0 : 원래 비율(0.5A 이하 Fine만 동작)
                                        // 1 : 0A ~ 0.5A Coarse 0.05A + Fine DAC 나머지, 0.5A~0.6A 비율 동작
                                        // 2 : ~ 0.1A Fine만 동작, 0.1A ~ 0.5A Coarse 0.05A + Fine DAC 나머지, 0.5A~0.6A
                                        // 3 :
                                        // 4 : Coarse Offset 100mA 주고 캘 시작
#define SENSING_MAIN     1
#define SENSING_TIMER    2
#define SENSING_SEPERATE 3

#define SENSING_TYPE     SENSING_SEPERATE

#define MODE_CHECK_OLD              1
#define MODE_CHECK_CURRENT          2
#define MODE_CHECK_CURRENT_POWER    3

#define MODE_CHECK_TYPE             MODE_CHECK_CURRENT_POWER

#define SET 1
#define CLEAR 0

#define ADDR32(A)			(*((volatile ulong *)(A)))
#define ADDR16(A)			(*((volatile ushort *)(A)))
#define ADDR8(A)			(*((volatile uchar *)(A)))

// Byte swapping.
#define SWAP8(A)		(A)
#define SWAP16(A)		((((A << 8 ) & 0xFF00)) | ((A >> 8)& 0x00FF))
#define SWAP32(A)		((((A<<24)&0xFF000000)) | (((A<<8)&0x00FF0000)) | (((A>>8)&0x0000FF00)) | (((A>>24)&0x000000FF)))

#define NEWLINE					"\n"

#define ABS_F(x) ((x) > 0 ? (x) : ((x) * (float)(-1.0)))        // 절대값 구하는 함수

#define MAX_16BIT_COUNT (65535)
#define MAX_32BIT_COUNT	(4294967295)

#define INCREASE_16BIT(x) {if(x < MAX_16BIT_COUNT) x += 1;}
#define INCREASE_32BIT(x) {if(x < MAX_32BIT_COUNT) x += 1;}

#define DECREASE_16BIT(x) {if(x) x -= 1;}
#define DECREASE_32BIT(x) {if(x) x -= 1;}
#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_COMMON_H_ */
