/*
 * PhoenixonControls_GPIO.h
 *
 *  Created on: 2018. 5. 19.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_GPIO_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_GPIO_H_


#define DSP_GPIO35_EEPROM_NCS_SET                   {GpioDataRegs.GPBCLEAR.bit.GPIO35  = 1;}
#define DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_SET         {GpioDataRegs.GPASET.bit.GPIO9  = 1;}
#define DSP_GPIO49_ADC_OS0_SET                      {GpioDataRegs.GPBSET.bit.GPIO49  = 1;}
#define DSP_GPIO48_ADC_OS1_SET                      {GpioDataRegs.GPBSET.bit.GPIO48  = 1;}
#define DSP_GPIO36_ADC_OS2_SET                      {GpioDataRegs.GPBSET.bit.GPIO36  = 1;}

#define DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_SET         {GpioDataRegs.GPASET.bit.GPIO8  = 1;}
#define DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_SET        {GpioDataRegs.GPCSET.bit.GPIO84  = 1;}
#define DSP_GPIO86_DAC_NRESET_SET                   {GpioDataRegs.GPCCLEAR.bit.GPIO86  = 1;}
#define DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_SET        {GpioDataRegs.GPCSET.bit.GPIO87  = 1;}
#define DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_SET        {GpioDataRegs.GPASET.bit.GPIO14  = 1;}
#define DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_SET        {GpioDataRegs.GPASET.bit.GPIO13  = 1;}
#define DSP_GPIO90_ADC_RESET_SET                    {GpioDataRegs.GPCSET.bit.GPIO90  = 1;}
#define DSP_GPIO53_DSP_CH1_CHG_EN_SET               {GpioDataRegs.GPBCLEAR.bit.GPIO53  = 1;}
#define DSP_GPIO52_DSP_CH1_DISCHG_EN_SET            {GpioDataRegs.GPBCLEAR.bit.GPIO52  = 1;}
#define DSP_GPIO66_DSP_CH1_RLY_ON_SET               {GpioDataRegs.GPCSET.bit.GPIO66  = 1;}
#define DSP_GPIO51_DSP_CH2_CHG_EN_SET               {GpioDataRegs.GPBCLEAR.bit.GPIO51  = 1;}
#define DSP_GPIO58_DSP_CH2_DISCHG_EN_SET            {GpioDataRegs.GPBCLEAR.bit.GPIO58  = 1;}
#define DSP_GPIO65_DSP_CH2_RLY_ON_SET               {GpioDataRegs.GPCSET.bit.GPIO65  = 1;}
#define DSP_GPIO59_DSP_CH3_CHG_EN_SET               {GpioDataRegs.GPBCLEAR.bit.GPIO59  = 1;}
#define DSP_GPIO60_DSP_CH3_DISCHG_EN_SET            {GpioDataRegs.GPBCLEAR.bit.GPIO60  = 1;}
#define DSP_GPIO64_DSP_CH3_RLY_ON_SET               {GpioDataRegs.GPCSET.bit.GPIO64  = 1;}
#define DSP_GPIO62_DSP_CH4_CHG_EN_SET               {GpioDataRegs.GPBCLEAR.bit.GPIO62  = 1;}
#define DSP_GPIO61_DSP_CH4_DISCHG_EN_SET            {GpioDataRegs.GPBCLEAR.bit.GPIO61  = 1;}
#define DSP_GPIO63_DSP_CH4_RLY_ON_SET               {GpioDataRegs.GPBSET.bit.GPIO63  = 1;}

#define DSP_GPIO89_DAC_A0_SET                       {GpioDataRegs.GPCSET.bit.GPIO88  = 1;}
#define DSP_GPIO88_DAC_A1_SET                       {GpioDataRegs.GPCSET.bit.GPIO89  = 1;}

#define DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_SET        {GpioDataRegs.GPCSET.bit.GPIO91  = 1;}
#define DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_SET        {GpioDataRegs.GPCSET.bit.GPIO93  = 1;}
#define DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_SET        {GpioDataRegs.GPBSET.bit.GPIO57  = 1;}
#define DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_SET        {GpioDataRegs.GPBSET.bit.GPIO56  = 1;}
#define DSP_GPIO44_OUTPUT_CH2_DISCHARGE_SET         {GpioDataRegs.GPBSET.bit.GPIO44  = 1;}

#define DSP_GPIO16_INTG_CH1_RESET_SET               {GpioDataRegs.GPASET.bit.GPIO16  = 1;}
#define DSP_GPIO17_INTG_CH2_RESET_SET               {GpioDataRegs.GPASET.bit.GPIO17  = 1;}
#define DSP_GPIO18_INTG_CH3_RESET_SET               {GpioDataRegs.GPASET.bit.GPIO18  = 1;}
#define DSP_GPIO15_INTG_CH4_RESET_SET               {GpioDataRegs.GPASET.bit.GPIO15  = 1;}

#define DSP_GPIO5_OPMODE_CH1_SEL0_SET               {GpioDataRegs.GPASET.bit.GPIO5  = 1;}
#define DSP_GPIO6_OPMODE_CH1_SEL1_SET               {GpioDataRegs.GPASET.bit.GPIO6  = 1;}
#define DSP_GPIO7_OPMODE_CH2_SEL0_SET               {GpioDataRegs.GPASET.bit.GPIO7  = 1;}
#define DSP_GPIO4_OPMODE_CH2_SEL1_SET               {GpioDataRegs.GPASET.bit.GPIO4  = 1;}
#define DSP_GPIO10_OPMODE_CH3_SEL0_SET              {GpioDataRegs.GPASET.bit.GPIO10  = 1;}
#define DSP_GPIO11_OPMODE_CH3_SEL1_SET              {GpioDataRegs.GPASET.bit.GPIO11  = 1;}
#define DSP_GPIO12_OPMODE_CH4_SEL0_SET              {GpioDataRegs.GPASET.bit.GPIO12  = 1;}
#define DSP_GPIO94_OPMODE_CH4_SEL1_SET              {GpioDataRegs.GPCSET.bit.GPIO94  = 1;}

#define DSP_GPIO46_OUTPUT_CH1_CHARGE_SET            {GpioDataRegs.GPBSET.bit.GPIO46  = 1;}
#define DSP_GPIO47_OUTPUT_CH1_DISCHARGE_SET         {GpioDataRegs.GPBSET.bit.GPIO47  = 1;}
#define DSP_GPIO45_OUTPUT_CH2_CHARGE_SET            {GpioDataRegs.GPBSET.bit.GPIO45  = 1;}
#define DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_SET        {GpioDataRegs.GPBSET.bit.GPIO54  = 1;}
#define DSP_GPIO43_OUTPUT_CH3_CHARGE_SET            {GpioDataRegs.GPBSET.bit.GPIO43  = 1;}
#define DSP_GPIO67_OUTPUT_CH3_DISCHARGE_SET         {GpioDataRegs.GPCSET.bit.GPIO67  = 1;}
#define DSP_GPIO68_OUTPUT_CH4_CHARGE_SET            {GpioDataRegs.GPCSET.bit.GPIO68  = 1;}
#define DSP_GPIO42_OUTPUT_CH4_DISCHARGE_SET         {GpioDataRegs.GPBSET.bit.GPIO42  = 1;}
#define DSP_GPIO21_ADC_RANGE_SET                    {GpioDataRegs.GPASET.bit.GPIO21  = 1;}
#define DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_SET        {GpioDataRegs.GPBSET.bit.GPIO55  = 1;}

#define DSP_GPIO19_ADC_N_CS_SET                     {GpioDataRegs.GPACLEAR.bit.GPIO19 = 1;}
#define DSP_GPIO20_IFINE_DAC_N_SYNC_SET             {GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;}
#define DSP_GPIO1_ADC_N_CS2_SET                     {GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;}
//------------------------------------------------------------------------------------------------

#define DSP_GPIO35_EEPROM_NCS_CLEAR                 {GpioDataRegs.GPBSET.bit.GPIO35  = 1;}
#define DSP_GPIO9_INTG_SLOPE_MUX_CH2_A2_CLEAR       {GpioDataRegs.GPACLEAR.bit.GPIO9  = 1;}
#define DSP_GPIO49_ADC_OS0_CLEAR                    {GpioDataRegs.GPBCLEAR.bit.GPIO49  = 1;}
#define DSP_GPIO48_ADC_OS1_CLEAR                    {GpioDataRegs.GPBCLEAR.bit.GPIO48  = 1;}
#define DSP_GPIO36_ADC_OS2_CLEAR                    {GpioDataRegs.GPBCLEAR.bit.GPIO36  = 1;}
#define DSP_GPIO8_INTG_SLOPE_MUX_CH1_A2_CLEAR       {GpioDataRegs.GPACLEAR.bit.GPIO8  = 1;}
#define DSP_GPIO84_INTG_SLOPE_MUX_CH1_A0_CLEAR      {GpioDataRegs.GPCCLEAR.bit.GPIO84  = 1;}
#define DSP_GPIO86_DAC_NRESET_CLEAR                 {GpioDataRegs.GPCSET.bit.GPIO86  = 1;}

#define DSP_GPIO87_INTG_SLOPE_MUX_CH1_A1_CLEAR      {GpioDataRegs.GPCCLEAR.bit.GPIO87  = 1;}
#define DSP_GPIO14_INTG_SLOPE_MUX_CH4_A2_CLEAR      {GpioDataRegs.GPACLEAR.bit.GPIO14  = 1;}
#define DSP_GPIO13_INTG_SLOPE_MUX_CH3_A2_CLEAR      {GpioDataRegs.GPACLEAR.bit.GPIO13  = 1;}
#define DSP_GPIO90_ADC_RESET_CLEAR                  {GpioDataRegs.GPCCLEAR.bit.GPIO90  = 1;}
#define DSP_GPIO53_DSP_CH1_CHG_EN_CLEAR             {GpioDataRegs.GPBSET.bit.GPIO53  = 1;}
#define DSP_GPIO52_DSP_CH1_DISCHG_EN_CLEAR          {GpioDataRegs.GPBSET.bit.GPIO52  = 1;}
#define DSP_GPIO66_DSP_CH1_RLY_ON_CLEAR             {GpioDataRegs.GPCCLEAR.bit.GPIO66  = 1;}
#define DSP_GPIO51_DSP_CH2_CHG_EN_CLEAR             {GpioDataRegs.GPBSET.bit.GPIO51  = 1;}
#define DSP_GPIO58_DSP_CH2_DISCHG_EN_CLEAR          {GpioDataRegs.GPBSET.bit.GPIO58  = 1;}
#define DSP_GPIO65_DSP_CH2_RLY_ON_CLEAR             {GpioDataRegs.GPCCLEAR.bit.GPIO65  = 1;}
#define DSP_GPIO59_DSP_CH3_CHG_EN_CLEAR             {GpioDataRegs.GPBSET.bit.GPIO59  = 1;}
#define DSP_GPIO60_DSP_CH3_DISCHG_EN_CLEAR          {GpioDataRegs.GPBSET.bit.GPIO60  = 1;}
#define DSP_GPIO64_DSP_CH3_RLY_ON_CLEAR             {GpioDataRegs.GPCCLEAR.bit.GPIO64  = 1;}
#define DSP_GPIO62_DSP_CH4_CHG_EN_CLEAR             {GpioDataRegs.GPBSET.bit.GPIO62  = 1;}
#define DSP_GPIO61_DSP_CH4_DISCHG_EN_CLEAR          {GpioDataRegs.GPBSET.bit.GPIO61  = 1;}
#define DSP_GPIO63_DSP_CH4_RLY_ON_CLEAR             {GpioDataRegs.GPBCLEAR.bit.GPIO63  = 1;}

#define DSP_GPIO89_DAC_A0_CLEAR                     {GpioDataRegs.GPCCLEAR.bit.GPIO88  = 1;}
#define DSP_GPIO88_DAC_A1_CLEAR                     {GpioDataRegs.GPCCLEAR.bit.GPIO89  = 1;}

#define DSP_GPIO91_INTG_SLOPE_MUX_CH2_A0_CLEAR      {GpioDataRegs.GPCCLEAR.bit.GPIO91  = 1;}
#define DSP_GPIO93_INTG_SLOPE_MUX_CH2_A1_CLEAR      {GpioDataRegs.GPCCLEAR.bit.GPIO93  = 1;}
#define DSP_GPIO57_INTG_SLOPE_MUX_CH4_A1_CLEAR      {GpioDataRegs.GPBCLEAR.bit.GPIO57  = 1;}
#define DSP_GPIO56_INTG_SLOPE_MUX_CH4_A0_CLEAR      {GpioDataRegs.GPBCLEAR.bit.GPIO56  = 1;}
#define DSP_GPIO44_OUTPUT_CH2_DISCHARGE_CLEAR       {GpioDataRegs.GPBCLEAR.bit.GPIO44  = 1;}

#define DSP_GPIO16_INTG_CH1_RESET_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO16  = 1;}
#define DSP_GPIO17_INTG_CH2_RESET_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO17  = 1;}
#define DSP_GPIO18_INTG_CH3_RESET_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO18  = 1;}
#define DSP_GPIO15_INTG_CH4_RESET_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO15  = 1;}

#define DSP_GPIO5_OPMODE_CH1_SEL0_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO5  = 1;}
#define DSP_GPIO6_OPMODE_CH1_SEL1_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO6  = 1;}
#define DSP_GPIO7_OPMODE_CH2_SEL0_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO7  = 1;}
#define DSP_GPIO4_OPMODE_CH2_SEL1_CLEAR             {GpioDataRegs.GPACLEAR.bit.GPIO4  = 1;}
#define DSP_GPIO10_OPMODE_CH3_SEL0_CLEAR            {GpioDataRegs.GPACLEAR.bit.GPIO10  = 1;}
#define DSP_GPIO11_OPMODE_CH3_SEL1_CLEAR            {GpioDataRegs.GPACLEAR.bit.GPIO11  = 1;}
#define DSP_GPIO12_OPMODE_CH4_SEL0_CLEAR            {GpioDataRegs.GPACLEAR.bit.GPIO12  = 1;}
#define DSP_GPIO94_OPMODE_CH4_SEL1_CLEAR            {GpioDataRegs.GPCCLEAR.bit.GPIO94  = 1;}

#define DSP_GPIO46_OUTPUT_CH1_CHARGE_CLEAR          {GpioDataRegs.GPBCLEAR.bit.GPIO46  = 1;}
#define DSP_GPIO47_OUTPUT_CH1_DISCHARGE_CLEAR       {GpioDataRegs.GPBCLEAR.bit.GPIO47  = 1;}
#define DSP_GPIO45_OUTPUT_CH2_CHARGE_CLEAR          {GpioDataRegs.GPBCLEAR.bit.GPIO45  = 1;}
#define DSP_GPIO54_INTG_SLOPE_MUX_CH3_A0_CLEAR      {GpioDataRegs.GPBCLEAR.bit.GPIO54  = 1;}
#define DSP_GPIO43_OUTPUT_CH3_CHARGE_CLEAR          {GpioDataRegs.GPBCLEAR.bit.GPIO43  = 1;}
#define DSP_GPIO67_OUTPUT_CH3_DISCHARGE_CLEAR       {GpioDataRegs.GPCCLEAR.bit.GPIO67  = 1;}
#define DSP_GPIO68_OUTPUT_CH4_CHARGE_CLEAR          {GpioDataRegs.GPCCLEAR.bit.GPIO68  = 1;}
#define DSP_GPIO42_OUTPUT_CH4_DISCHARGE_CLEAR       {GpioDataRegs.GPBCLEAR.bit.GPIO42  = 1;}
#define DSP_GPIO21_ADC_RANGE_CLEAR                  {GpioDataRegs.GPACLEAR.bit.GPIO21  = 1;}
#define DSP_GPIO55_INTG_SLOPE_MUX_CH3_A1_CLEAR      {GpioDataRegs.GPBCLEAR.bit.GPIO55  = 1;}

#define DSP_GPIO1_ADC_N_CS2_CLEAR                   {GpioDataRegs.GPASET.bit.GPIO1 = 1;}
#define DSP_GPIO19_ADC_N_CS_CLEAR                   {GpioDataRegs.GPASET.bit.GPIO19 = 1;}
#define DSP_GPIO20_IFINE_DAC_N_SYNC_CLEAR           {GpioDataRegs.GPASET.bit.GPIO20 = 1;}
//------------------------------------------------------------------------------------------------
#define DSP_GPIO33_ADC_BUSY_DATA                    GpioDataRegs.GPBDAT.bit.GPIO33
#define DSP_GPIO50_EMERGENCY_IN_DATA                GpioDataRegs.GPBDAT.bit.GPIO50

#define DSP_GPIO0_FAN_OUT_DATA                      GpioDataRegs.GPADAT.bit.GPIO0
#define DSP_GPIO2_ADC_BUSY2_DATA                    GpioDataRegs.GPADAT.bit.GPIO2
//------------------------------------------------------------------------------------------------
void DSP_GpioInit(void);


#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_GPIO_H_ */
