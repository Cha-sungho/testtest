/*
 * PhoenixonControls_GPIO.c
 *
 *  Created on: 2018. 5. 19.
 *      Author: BGKim
 */
#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"

void DSP_GpioInit(void)
{
    Device_initGPIO();

    GPIO_setPinConfig(GPIO_0_GPIO0);        //FAN_OUT
    GPIO_setPinConfig(GPIO_1_GPIO1);        //ADC_nCS_2
    GPIO_setPinConfig(GPIO_2_GPIO2);        //ADC_BUSY_2
    GPIO_setPinConfig(GPIO_4_GPIO4);        //OPMODE_CH2_SEL1
    GPIO_setPinConfig(GPIO_5_GPIO5);        //OPMODE_CH1_SEL0
    GPIO_setPinConfig(GPIO_6_GPIO6);        //OPMODE_CH1_SEL1
    GPIO_setPinConfig(GPIO_7_GPIO7);        //OPMODE_CH2_SEL0
    GPIO_setPinConfig(GPIO_8_GPIO8);        //INTG_SLOPE_MUX_CH1_A2
    GPIO_setPinConfig(GPIO_9_GPIO9);        //INTG_SLOPE_MUX_CH2_A2

    GPIO_setPinConfig(GPIO_10_GPIO10);      //OPMODE_CH3_SEL0
    GPIO_setPinConfig(GPIO_11_GPIO11);      //OPMODE_CH3_SEL1
    GPIO_setPinConfig(GPIO_12_GPIO12);      //OPMODE_CH4_SEL0
    GPIO_setPinConfig(GPIO_13_GPIO13);      //INTG_SLOPE_MUX_CH3_A2
    GPIO_setPinConfig(GPIO_14_GPIO14);      //INTG_SLOPE_MUX_CH4_A2
    GPIO_setPinConfig(GPIO_15_GPIO15);      //INTG_CH4_RESET
    GPIO_setPinConfig(GPIO_16_GPIO16);      //INTG_CH1_RESET()
    GPIO_setPinConfig(GPIO_17_GPIO17);      //INTG_CH2_RESET()
    GPIO_setPinConfig(GPIO_18_GPIO18);      //INTG_CH3_RESET()

    GPIO_setPinConfig(GPIO_19_GPIO19);      //ADC_nCS
    GPIO_setPinConfig(GPIO_20_GPIO20);      //IFINE_DAC_SYNC

    GPIO_setPinConfig(GPIO_21_GPIO21);      //ADC_RANGE

//    GPIO_setPinConfig(GPIO_30_GPIO30);      //DSP_RS485_DE     (CPU2 할당)

    GPIO_setPinConfig(GPIO_33_GPIO33);      //ADC_BUSY
    GPIO_setPinConfig(GPIO_35_GPIO35);      //EEPROM_CS
    GPIO_setPinConfig(GPIO_36_GPIO36);      //ADC_OS2

    GPIO_setPinConfig(GPIO_42_GPIO42);      //OUTPUT_CH4_DISCHARGE
    GPIO_setPinConfig(GPIO_43_GPIO43);      //OUTPUT_CH3_CHARGE
    GPIO_setPinConfig(GPIO_44_GPIO44);      //OUTPUT_CH2_DISCHARGE
    GPIO_setPinConfig(GPIO_45_GPIO45);      //OUTPUT_CH2_CHARGE
    GPIO_setPinConfig(GPIO_46_GPIO46);      //OUTPUT_CH1_CHARGE
    GPIO_setPinConfig(GPIO_47_GPIO47);      //OUTPUT_CH1_DISCHARGE
    GPIO_setPinConfig(GPIO_48_GPIO48);      //ADC_OS1
    GPIO_setPinConfig(GPIO_49_GPIO49);      //ADC_OS0

    GPIO_setPinConfig(GPIO_50_GPIO50);      //EMERGENCY_IN
    GPIO_setPinConfig(GPIO_51_GPIO51);      //DSP_CH2_CHG_EN
    GPIO_setPinConfig(GPIO_52_GPIO52);      //DSP_CH1_DISCHG_EN
    GPIO_setPinConfig(GPIO_53_GPIO53);      //DSP_CH1_CHG_EN
    GPIO_setPinConfig(GPIO_54_GPIO54);      //INTG_SLOPE_MUX_CH3_A0
    GPIO_setPinConfig(GPIO_55_GPIO55);      //INTG_SLOPE_MUX_CH3_A1
    GPIO_setPinConfig(GPIO_56_GPIO56);      //INTG_SLOPE_MUX_CH4_A0
    GPIO_setPinConfig(GPIO_57_GPIO57);      //INTG_SLOPE_MUX_CH4_A1
    GPIO_setPinConfig(GPIO_58_GPIO58);      //DSP_CH2_DISCHG_EN
    GPIO_setPinConfig(GPIO_59_GPIO59);      //DSP_CH3_CHG_EN

    GPIO_setPinConfig(GPIO_60_GPIO60);      //DSP_CH3_DISCHG_EN
    GPIO_setPinConfig(GPIO_61_GPIO61);      //DSP_CH4_DISCHG_EN
    GPIO_setPinConfig(GPIO_62_GPIO62);      //DSP_CH4_CHG_EN
    GPIO_setPinConfig(GPIO_63_GPIO63);      //DSP_CH4_RLY_ON
    GPIO_setPinConfig(GPIO_64_GPIO64);      //DSP_CH3_RLY_ON
    GPIO_setPinConfig(GPIO_65_GPIO65);      //DSP_CH2_RLY_ON
    GPIO_setPinConfig(GPIO_66_GPIO66);      //DSP_CH1_RLY_ON
    GPIO_setPinConfig(GPIO_67_GPIO67);      //OUTPUT_CH3_DISCHARGE
    GPIO_setPinConfig(GPIO_68_GPIO68);      //OUTPUT_CH4_CHARGE

    GPIO_setPinConfig(GPIO_84_GPIO84);      //INTG_SLOPE_MUX_CH1_A0
    GPIO_setPinConfig(GPIO_86_GPIO86);      //DAC_NRESET
    GPIO_setPinConfig(GPIO_87_GPIO87);      //INTG_SLOPE_MUX_CH1_A1
    GPIO_setPinConfig(GPIO_88_GPIO88);      //DAC_A1
    GPIO_setPinConfig(GPIO_89_GPIO89);      //DAC_A0

    GPIO_setPinConfig(GPIO_90_GPIO90);      //ADC_RESET
    GPIO_setPinConfig(GPIO_91_GPIO91);      //INTG_SLOPE_MUX_CH2_A0
    GPIO_setPinConfig(GPIO_93_GPIO93);      //INTG_SLOPE_MUX_CH2_A1
    GPIO_setPinConfig(GPIO_94_GPIO94);      //OPMODE_CH4_SEL1
//  GPIO_setPinConfig(GPIO_99_GPIO99);      //DSP_STATUS  // CPU2에 할당

    GPIO_setPinConfig(GPIO_22_SCITXDB);     //DSP_RS232C_TX
    GPIO_setPinConfig(GPIO_23_SCIRXDB);     //DSP_RS232C_RX
    GPIO_setPinConfig(GPIO_24_SPISIMOB);    //SPISIMOB
    GPIO_setPinConfig(GPIO_25_SPISOMIB);    //SPISOMIB
    GPIO_setPinConfig(GPIO_26_SPICLKB);     //SPICLKB
    GPIO_setPinConfig(GPIO_27_SPISTEB);     //SPISTEB
//  GPIO_setPinConfig(GPIO_28_SCIRXDA);     //DSP_RS485_RX      CPU2 할당
//  GPIO_setPinConfig(GPIO_29_SCITXDA);     //DSP_RS485_TX      CPU2 할당

    GPIO_setPinConfig(GPIO_85_EM1D0);       //DSP_XD0
    GPIO_setPinConfig(GPIO_83_EM1D1);       //DSP_XD1
    GPIO_setPinConfig(GPIO_82_EM1D2);       //DSP_XD2
    GPIO_setPinConfig(GPIO_81_EM1D3);       //DSP_XD3
    GPIO_setPinConfig(GPIO_80_EM1D4);       //DSP_XD4
    GPIO_setPinConfig(GPIO_79_EM1D5);       //DSP_XD5
    GPIO_setPinConfig(GPIO_78_EM1D6);       //DSP_XD6
    GPIO_setPinConfig(GPIO_77_EM1D7);       //DSP_XD7
    GPIO_setPinConfig(GPIO_76_EM1D8);       //DSP_XD8
    GPIO_setPinConfig(GPIO_75_EM1D9);       //DSP_XD9
    GPIO_setPinConfig(GPIO_74_EM1D10);      //DSP_XD10
    GPIO_setPinConfig(GPIO_73_EM1D11);      //DSP_XD11
    GPIO_setPinConfig(GPIO_72_EM1D12);      //DSP_XD12
    GPIO_setPinConfig(GPIO_71_EM1D13);      //DSP_XD13
    GPIO_setPinConfig(GPIO_70_EM1D14);      //DSP_XD14
    GPIO_setPinConfig(GPIO_69_EM1D15);      //DSP_XD15
    GPIO_setPinConfig(GPIO_92_EM1BA1);      //DSP_XA0
    GPIO_setPinConfig(GPIO_38_EM1A0);       //DSP_XA1
    GPIO_setPinConfig(GPIO_39_EM1A1);       //DSP_XA2
    GPIO_setPinConfig(GPIO_40_EM1A2);       //DSP_XA3
    GPIO_setPinConfig(GPIO_41_EM1A3);       //DSP_XA4
    GPIO_setPinConfig(GPIO_34_EM1CS2N);     //NXCS
    GPIO_setPinConfig(GPIO_37_EM1OEN);      //NXRD
    GPIO_setPinConfig(GPIO_31_EM1WEN);      //NXWE


////********************************  GPIO OUT PIN  ////////////////////*********************************
    GPIO_setDirectionMode(1, GPIO_DIR_MODE_OUT);        //DSP_GPIO1_ADC_nCS_2
    GPIO_setDirectionMode(10, GPIO_DIR_MODE_OUT);       //DSP_GPIO10_OPMODE_CH3_SEL0
    GPIO_setDirectionMode(11, GPIO_DIR_MODE_OUT);       //DSP_GPIO11_OPMODE_CH3_SEL1
    GPIO_setDirectionMode(12, GPIO_DIR_MODE_OUT);       //DSP_GPIO12_OPMODE_CH4_SEL0
    GPIO_setDirectionMode(13, GPIO_DIR_MODE_OUT);       //DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2
    GPIO_setDirectionMode(14, GPIO_DIR_MODE_OUT);       //DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2
    GPIO_setDirectionMode(15, GPIO_DIR_MODE_OUT);       //DSP_GPIO15_INTG_CH4_RESET
    GPIO_setDirectionMode(16, GPIO_DIR_MODE_OUT);       //DSP_GPIO16_INTG_CH1_RESET
    GPIO_setDirectionMode(17, GPIO_DIR_MODE_OUT);       //DSP_GPIO17_INTG_CH2_RESET
    GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);       //DSP_GPIO18_INTG_CH3_RESET

    GPIO_setDirectionMode(19, GPIO_DIR_MODE_OUT);       //DSP_GPIO19_ADC_nCS
    GPIO_setDirectionMode(20, GPIO_DIR_MODE_OUT);       //DSP_GPIO20_IFINE_DAC_SYNC
    GPIO_setDirectionMode(21, GPIO_DIR_MODE_OUT);       //DSP_GPIO21_ADC_RANGE

    GPIO_setDirectionMode(35, GPIO_DIR_MODE_OUT);       //DSP_GPIO35_EEPROM_NCS
    GPIO_setDirectionMode(36, GPIO_DIR_MODE_OUT);       //DSP_GPIO36_ADC_OS2

    GPIO_setDirectionMode(4, GPIO_DIR_MODE_OUT);        //DSP_GPIO4_OPMODE_CH2_SEL1
    GPIO_setDirectionMode(42, GPIO_DIR_MODE_OUT);       //DSP_GPIO42_OUTPUT_CH4_DISCHARGE
    GPIO_setDirectionMode(43, GPIO_DIR_MODE_OUT);       //DSP_GPIO43_OUTPUT_CH3_CHARGE
    GPIO_setDirectionMode(44, GPIO_DIR_MODE_OUT);       //DSP_GPIO44_OUTPUT_CH2_DISCHARGE
    GPIO_setDirectionMode(45, GPIO_DIR_MODE_OUT);       //DSP_GPIO45_OUTPUT_CH2_CHARGE
    GPIO_setDirectionMode(46, GPIO_DIR_MODE_OUT);       //DSP_GPIO46_OUTPUT_CH1_CHARGE
    GPIO_setDirectionMode(47, GPIO_DIR_MODE_OUT);       //DSP_GPIO47_OUTPUT_CH1_DISCHARGE
    GPIO_setDirectionMode(48, GPIO_DIR_MODE_OUT);       //DSP_GPIO48_ADC_OS1
    GPIO_setDirectionMode(49, GPIO_DIR_MODE_OUT);       //DSP_GPIO49_ADC_OS0

    GPIO_setDirectionMode(5, GPIO_DIR_MODE_OUT);        //DSP_GPIO5_OPMODE_CH1_SEL0
    GPIO_setDirectionMode(51, GPIO_DIR_MODE_OUT);       //DSP_GPIO51_DSP_CH2_CHG_EN
    GPIO_setDirectionMode(52, GPIO_DIR_MODE_OUT);       //DSP_GPIO52_DSP_CH1_DISCHG_EN
    GPIO_setDirectionMode(53, GPIO_DIR_MODE_OUT);       //DSP_GPIO53_DSP_CH1_CHG_EN
    GPIO_setDirectionMode(54, GPIO_DIR_MODE_OUT);       //DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0
    GPIO_setDirectionMode(55, GPIO_DIR_MODE_OUT);       //DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1
    GPIO_setDirectionMode(56, GPIO_DIR_MODE_OUT);       //DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0
    GPIO_setDirectionMode(57, GPIO_DIR_MODE_OUT);       //DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1
    GPIO_setDirectionMode(58, GPIO_DIR_MODE_OUT);       //DSP_GPIO58_DSP_CH2_DISCHG_EN
    GPIO_setDirectionMode(59, GPIO_DIR_MODE_OUT);       //DSP_GPIO59_DSP_CH3_CHG_EN

    GPIO_setDirectionMode(6, GPIO_DIR_MODE_OUT);        //DSP_GPIO6_OPMODE_CH1_SEL1
    GPIO_setDirectionMode(60, GPIO_DIR_MODE_OUT);       //DSP_GPIO60_DSP_CH3_DISCHG_EN
    GPIO_setDirectionMode(61, GPIO_DIR_MODE_OUT);       //DSP_GPIO61_DSP_CH4_DISCHG_EN
    GPIO_setDirectionMode(62, GPIO_DIR_MODE_OUT);       //DSP_GPIO62_DSP_CH4_CHG_EN
    GPIO_setDirectionMode(63, GPIO_DIR_MODE_OUT);       //DSP_GPIO63_DSP_CH4_RLY_ON
    GPIO_setDirectionMode(64, GPIO_DIR_MODE_OUT);       //DSP_GPIO64_DSP_CH3_RLY_ON
    GPIO_setDirectionMode(65, GPIO_DIR_MODE_OUT);       //DSP_GPIO65_DSP_CH2_RLY_ON
    GPIO_setDirectionMode(66, GPIO_DIR_MODE_OUT);       //DSP_GPIO66_DSP_CH1_RLY_ON
    GPIO_setDirectionMode(67, GPIO_DIR_MODE_OUT);       //DSP_GPIO67_OUTPUT_CH3_DISCHARGE
    GPIO_setDirectionMode(68, GPIO_DIR_MODE_OUT);       //DSP_GPIO68_OUTPUT_CH4_CHARGE

    GPIO_setDirectionMode(7, GPIO_DIR_MODE_OUT);        //DSP_GPIO7_OPMODE_CH2_SEL0

    GPIO_setDirectionMode(8, GPIO_DIR_MODE_OUT);        //DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2
    GPIO_setDirectionMode(84, GPIO_DIR_MODE_OUT);       //DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0
    GPIO_setDirectionMode(86, GPIO_DIR_MODE_OUT);       //DSP_GPIO86_DAC_NRESET
    GPIO_setDirectionMode(87, GPIO_DIR_MODE_OUT);       //DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1
    GPIO_setDirectionMode(88, GPIO_DIR_MODE_OUT);       //DSP_GPIO88_DAC_A1
    GPIO_setDirectionMode(89, GPIO_DIR_MODE_OUT);       //DSP_GPIO89_DAC_A0

    GPIO_setDirectionMode(9, GPIO_DIR_MODE_OUT);        //DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2
    GPIO_setDirectionMode(90, GPIO_DIR_MODE_OUT);       //DSP_GPIO90_ADC_RESET
    GPIO_setDirectionMode(91, GPIO_DIR_MODE_OUT);       //DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0
    GPIO_setDirectionMode(93, GPIO_DIR_MODE_OUT);       //DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1
    GPIO_setDirectionMode(94, GPIO_DIR_MODE_OUT);       //DSP_GPIO94_OPMODE_CH4_SEL1
//  GPIO_setDirectionMode(99, GPIO_DIR_MODE_OUT);       //DSP_GPIO99_DSP_STATUS     CPU2 할당


////********************************  GPIO IN PIN  ////////////////////*********************************

    GPIO_setDirectionMode(0, GPIO_DIR_MODE_IN);         //DSP_GPIO0_FAN_OUT
    GPIO_setDirectionMode(2, GPIO_DIR_MODE_IN);        //DSP_GPIO2_ADC_BUSY_2

    GPIO_setDirectionMode(33, GPIO_DIR_MODE_IN);        //DSP_GPIO33_ADC_BUSY
    GPIO_setDirectionMode(50, GPIO_DIR_MODE_IN);        //DSP_GPIO55_EMERGENCY_IN

    GPIO_setQualificationMode(0, GPIO_QUAL_ASYNC);     //DSP_GPIO0_FAN_OUT
    GPIO_setQualificationMode(2, GPIO_QUAL_ASYNC);     //DSP_GPIO2_ADC_BUSY_2

    GPIO_setQualificationMode(33, GPIO_QUAL_ASYNC);     //DSP_GPIO33_ADC_BUSY
    GPIO_setQualificationMode(50, GPIO_QUAL_ASYNC);     //DSP_GPIO55_EMERGENCY_IN


/////********************************* SPI, SCI /////////////////**************************
    GPIO_setDirectionMode(22, GPIO_DIR_MODE_OUT);       //DSP_GPIO22_DSP_RS232C_TX
    GPIO_setDirectionMode(23, GPIO_DIR_MODE_IN);        //DSP_GPIO23_DSP_RS232C_RX
    GPIO_setDirectionMode(24, GPIO_DIR_MODE_OUT);       //DSP_GPIO24_SPISIMOB
    GPIO_setDirectionMode(25, GPIO_DIR_MODE_IN);        //DSP_GPIO25_SPISOMIB
    GPIO_setDirectionMode(26, GPIO_DIR_MODE_OUT);       //DSP_GPIO26_SPICLKB
    GPIO_setDirectionMode(27, GPIO_DIR_MODE_OUT);       //DSP_GPIO27_SPISTEB


    GPIO_setQualificationMode(22, GPIO_QUAL_ASYNC);     //DSP_GPIO22_DSP_RS232C_TX  // Output핀에 왜 Input Qualifier기능 사용하는지??
    GPIO_setQualificationMode(23, GPIO_QUAL_ASYNC);     //DSP_GPIO23_DSP_RS232C_RX
    GPIO_setQualificationMode(24, GPIO_QUAL_ASYNC);     //DSP_GPIO24_SPISIMOB       // Output핀에 왜 Input Qualifier기능 사용하는지??
    GPIO_setQualificationMode(25, GPIO_QUAL_ASYNC);     //DSP_GPIO25_SPISOMIB
    GPIO_setQualificationMode(26, GPIO_QUAL_ASYNC);     //DSP_GPIO26_SPICLKB        // Output핀에 왜 Input Qualifier기능 사용하는지??
    GPIO_setQualificationMode(27, GPIO_QUAL_ASYNC);     //DSP_GPIO27_SPISTEB        // Output핀에 왜 Input Qualifier기능 사용하는지??


/////****************************  EMIF  ///////////////////***************************
    GPIO_setDirectionMode(85, GPIO_DIR_MODE_IN);        //DSP_GPIO85_DSP_XD0
    GPIO_setDirectionMode(83, GPIO_DIR_MODE_IN);        //DSP_GPIO83_DSP_XD1
    GPIO_setDirectionMode(82, GPIO_DIR_MODE_IN);        //DSP_GPIO82_DSP_XD2
    GPIO_setDirectionMode(81, GPIO_DIR_MODE_IN);        //DSP_GPIO81_DSP_XD3
    GPIO_setDirectionMode(80, GPIO_DIR_MODE_IN);        //DSP_GPIO80_DSP_XD4
    GPIO_setDirectionMode(79, GPIO_DIR_MODE_IN);        //DSP_GPIO79_DSP_XD5
    GPIO_setDirectionMode(78, GPIO_DIR_MODE_IN);        //DSP_GPIO78_DSP_XD6
    GPIO_setDirectionMode(77, GPIO_DIR_MODE_IN);        //DSP_GPIO77_DSP_XD7
    GPIO_setDirectionMode(76, GPIO_DIR_MODE_IN);        //DSP_GPIO76_DSP_XD8
    GPIO_setDirectionMode(75, GPIO_DIR_MODE_IN);        //DSP_GPIO75_DSP_XD9
    GPIO_setDirectionMode(74, GPIO_DIR_MODE_IN);        //DSP_GPIO74_DSP_XD10
    GPIO_setDirectionMode(73, GPIO_DIR_MODE_IN);        //DSP_GPIO73_DSP_XD11
    GPIO_setDirectionMode(72, GPIO_DIR_MODE_IN);        //DSP_GPIO72_DSP_XD12
    GPIO_setDirectionMode(71, GPIO_DIR_MODE_IN);        //DSP_GPIO71_DSP_XD13
    GPIO_setDirectionMode(70, GPIO_DIR_MODE_IN);        //DSP_GPIO70_DSP_XD14
    GPIO_setDirectionMode(69, GPIO_DIR_MODE_IN);        //DSP_GPIO69_DSP_XD15

    GPIO_setDirectionMode(92, GPIO_DIR_MODE_OUT);       //DSP_GPIO92_DSP_XA0
    GPIO_setDirectionMode(38, GPIO_DIR_MODE_OUT);       //DSP_GPIO38_DSP_XA0(1)
    GPIO_setDirectionMode(39, GPIO_DIR_MODE_OUT);       //DSP_GPIO39_DSP_XA1(2)
    GPIO_setDirectionMode(40, GPIO_DIR_MODE_OUT);       //DSP_GPIO40_DSP_XA2(3)
    GPIO_setDirectionMode(41, GPIO_DIR_MODE_OUT);       //DSP_GPIO41_DSP_XA3(4)
    GPIO_setDirectionMode(34, GPIO_DIR_MODE_OUT);       //DSP_GPIO34_N_CS2
    GPIO_setDirectionMode(37, GPIO_DIR_MODE_OUT);       //DSP_GPIO37_N_XRD
    GPIO_setDirectionMode(31, GPIO_DIR_MODE_OUT);       //DSP_GPIO31_N_XWE

    GPIO_setQualificationMode(85, GPIO_QUAL_ASYNC);     //DSP_GPIO85_DSP_XD0
    GPIO_setQualificationMode(83, GPIO_QUAL_ASYNC);     //DSP_GPIO83_DSP_XD1
    GPIO_setQualificationMode(82, GPIO_QUAL_ASYNC);     //DSP_GPIO82_DSP_XD2
    GPIO_setQualificationMode(81, GPIO_QUAL_ASYNC);     //DSP_GPIO81_DSP_XD3
    GPIO_setQualificationMode(80, GPIO_QUAL_ASYNC);     //DSP_GPIO80_DSP_XD4
    GPIO_setQualificationMode(79, GPIO_QUAL_ASYNC);     //DSP_GPIO79_DSP_XD5
    GPIO_setQualificationMode(78, GPIO_QUAL_ASYNC);     //DSP_GPIO78_DSP_XD6
    GPIO_setQualificationMode(77, GPIO_QUAL_ASYNC);     //DSP_GPIO77_DSP_XD7
    GPIO_setQualificationMode(76, GPIO_QUAL_ASYNC);     //DSP_GPIO76_DSP_XD8
    GPIO_setQualificationMode(75, GPIO_QUAL_ASYNC);     //DSP_GPIO75_DSP_XD9
    GPIO_setQualificationMode(74, GPIO_QUAL_ASYNC);     //DSP_GPIO74_DSP_XD10
    GPIO_setQualificationMode(73, GPIO_QUAL_ASYNC);     //DSP_GPIO73_DSP_XD11
    GPIO_setQualificationMode(72, GPIO_QUAL_ASYNC);     //DSP_GPIO72_DSP_XD12
    GPIO_setQualificationMode(71, GPIO_QUAL_ASYNC);     //DSP_GPIO71_DSP_XD13
    GPIO_setQualificationMode(70, GPIO_QUAL_ASYNC);     //DSP_GPIO70_DSP_XD14
    GPIO_setQualificationMode(69, GPIO_QUAL_ASYNC);     //DSP_GPIO69_DSP_XD15


////////////////////////////////////////////////////////////////////////////////////

    // GPIO 초기값 설정

    DSP_GPIO49_ADC_OS0_CLEAR
    DSP_GPIO48_ADC_OS1_CLEAR
    DSP_GPIO36_ADC_OS2_CLEAR
    DSP_GPIO21_ADC_RANGE_CLEAR
    DSP_GPIO90_ADC_RESET_CLEAR

    DSP_GPIO89_DAC_A0_CLEAR
    DSP_GPIO88_DAC_A1_CLEAR
    DSP_GPIO86_DAC_NRESET_CLEAR

    DSP_GPIO53_DSP_CH1_CHG_EN_CLEAR
    DSP_GPIO52_DSP_CH1_DISCHG_EN_CLEAR
    DSP_GPIO51_DSP_CH2_CHG_EN_CLEAR
    DSP_GPIO58_DSP_CH2_DISCHG_EN_CLEAR
    DSP_GPIO59_DSP_CH3_CHG_EN_CLEAR
    DSP_GPIO60_DSP_CH3_DISCHG_EN_CLEAR
    DSP_GPIO62_DSP_CH4_CHG_EN_CLEAR
    DSP_GPIO61_DSP_CH4_DISCHG_EN_CLEAR

    DSP_GPIO66_DSP_CH1_RLY_ON_CLEAR
    DSP_GPIO65_DSP_CH2_RLY_ON_CLEAR
    DSP_GPIO64_DSP_CH3_RLY_ON_CLEAR
    DSP_GPIO63_DSP_CH4_RLY_ON_CLEAR


    DSP_GPIO16_INTG_CH1_RESET_CLEAR
    DSP_GPIO17_INTG_CH2_RESET_CLEAR
    DSP_GPIO18_INTG_CH3_RESET_CLEAR
    DSP_GPIO15_INTG_CH4_RESET_CLEAR

    DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR
    DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR
    DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR
    DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR
    DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR
    DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR
    DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR
    DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR
    DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR
    DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR
    DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR
    DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR

    DSP_GPIO5_OPMODE_CH1_SEL0_CLEAR
    DSP_GPIO6_OPMODE_CH1_SEL1_CLEAR
    DSP_GPIO7_OPMODE_CH2_SEL0_CLEAR
    DSP_GPIO4_OPMODE_CH2_SEL1_CLEAR
    DSP_GPIO10_OPMODE_CH3_SEL0_CLEAR
    DSP_GPIO11_OPMODE_CH3_SEL1_CLEAR
    DSP_GPIO12_OPMODE_CH4_SEL0_CLEAR
    DSP_GPIO94_OPMODE_CH4_SEL1_CLEAR

    DSP_GPIO46_OUTPUT_CH1_CHARGE_CLEAR
    DSP_GPIO47_OUTPUT_CH1_DISCHARGE_CLEAR
    DSP_GPIO45_OUTPUT_CH2_CHARGE_CLEAR
    DSP_GPIO44_OUTPUT_CH2_DISCHARGE_CLEAR

    DSP_GPIO43_OUTPUT_CH3_CHARGE_CLEAR
    DSP_GPIO67_OUTPUT_CH3_DISCHARGE_CLEAR
    DSP_GPIO68_OUTPUT_CH4_CHARGE_CLEAR
    DSP_GPIO42_OUTPUT_CH4_DISCHARGE_CLEAR

    DSP_GPIO35_EEPROM_NCS_CLEAR
    DSP_GPIO1_ADC_N_CS2_CLEAR
    DSP_GPIO19_ADC_N_CS_CLEAR
    DSP_GPIO20_IFINE_DAC_N_SYNC_CLEAR
}



//
// End of file
//
