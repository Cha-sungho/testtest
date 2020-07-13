/*
 * PhoenixonControls_CALIBRATION.c
 *
 *  Created on: 2019. 6. 24.
 *  Ochang 1, 30A 40CH
 *  FW Team
 */

#include "../../../common/include/F28x_Project.h"
#include "../../../common/include/driverlib.h"
#include "../../../common/include/device.h"
#pragma CODE_SECTION(CALIBRATION_ConversionREF, ".TI.ramfunc");
//#pragma CODE_SECTION(CALIBRATION_ConversionADC, ".TI.ramfunc");
#pragma CODE_SECTION(CALIBRATION_ConversionCurrentADC, ".TI.ramfunc");
#pragma CODE_SECTION(CALIBRATION_ConversionVoltageADC, ".TI.ramfunc");

#ifdef _DEBUG_PRINTF_CALL
#include <stdio.h>
#endif
//20SH011-2_5V50A8CH
//
//**********************BOARD_01_START**********************
//CH 1, 2, 3, 4
#if CONTROL_BOARD == 1
//---------------------------------------------------------------------------------------------------------
float32 VoltageCharge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
    {
      0.000,
      1.000,
      2.000,
      3.000,
      4.000,
      5.000
    },
    {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
    },
    {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
    },
    {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
    }
};
float32 VoltageDischarge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
   {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
   },
   {
    0.000,
    1.000,
    2.000,
    3.000,
    4.000,
    5.000
   },
   {
    0.000,
    1.000,
    2.000,
    3.000,
    4.000,
    5.000
   },
   {
    0.000,
    1.000,
    2.000,
    3.000,
    4.000,
    5.000
   }
};
float32 VoltageCharge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
    {
      0.000,
      1.000,
      2.000,
      3.000,
      4.000,
      5.000
    },
    {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
    },
    {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
    },
    {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
    }
};
float32 VoltageDischarge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
   {
     0.000,
     1.000,
     2.000,
     3.000,
     4.000,
     5.000
   },
   {
    0.000,
    1.000,
    2.000,
    3.000,
    4.000,
    5.000
   },
   {
    0.000,
    1.000,
    2.000,
    3.000,
    4.000,
    5.000
   },
   {
    0.000,
    1.000,
    2.000,
    3.000,
    4.000,
    5.000
   }
};

float32 CurrentCharge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
       {
               0.011236727    ,
               0.111091       ,
               0.210946       ,
               0.310357       ,
               0.410224       ,
               0.509899       ,
               0.609746       ,

        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       },
       {
        0.0    ,
        0.1    ,
        0.2    ,
        0.3    ,
        0.4    ,
        0.5    ,
        0.6    ,
        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       },
       {
        0.0    ,
        0.1    ,
        0.2    ,
        0.3    ,
        0.4    ,
        0.5    ,
        0.6    ,
        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       },
       {
        0.0    ,
        0.1    ,
        0.2    ,
        0.3    ,
        0.4    ,
        0.5    ,
        0.6    ,
        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       }
};

float32 CurrentDischarge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
      {
              0.052652182   ,
              0.152510      ,
              0.252368      ,
              0.352395      ,
              0.452102      ,
              0.551989      ,
              0.651594      ,

       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      },
      {
       0.0    ,
       0.1    ,
       0.2    ,
       0.3    ,
       0.4    ,
       0.5    ,
       0.6    ,
       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      },
      {
       0.0    ,
       0.1    ,
       0.2    ,
       0.3    ,
       0.4    ,
       0.5    ,
       0.6    ,
       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      },
      {
       0.0    ,
       0.1    ,
       0.2    ,
       0.3    ,
       0.4    ,
       0.5    ,
       0.6    ,
       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      }
};

float32 CurrentCharge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
       {
               0.029782101  ,
               0.129803     ,
               0.229825     ,
               0.329823     ,
               0.429939     ,
               0.529994     ,
               0.630038     ,

        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       },
       {
        0.0    ,
        0.1    ,
        0.2    ,
        0.3    ,
        0.4    ,
        0.5    ,
        0.6    ,
        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       },
       {
        0.0    ,
        0.1    ,
        0.2    ,
        0.3    ,
        0.4    ,
        0.5    ,
        0.6    ,
        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       },
       {
        0.0    ,
        0.1    ,
        0.2    ,
        0.3    ,
        0.4    ,
        0.5    ,
        0.6    ,
        0.7    ,
        0.8    ,
        0.9    ,
        1.0    ,
        1.1
       }
};
float32 CurrentDischarge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
      {
              0.034291358   ,
              0.134337      ,
              0.234382      ,
              0.334390      ,
              0.434394      ,
              0.534567      ,
              0.634498      ,

       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      },
      {
       0.0    ,
       0.1    ,
       0.2    ,
       0.3    ,
       0.4    ,
       0.5    ,
       0.6    ,
       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      },
      {
       0.0    ,
       0.1    ,
       0.2    ,
       0.3    ,
       0.4    ,
       0.5    ,
       0.6    ,
       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      },
      {
       0.0    ,
       0.1    ,
       0.2    ,
       0.3    ,
       0.4    ,
       0.5    ,
       0.6    ,
       0.7    ,
       0.8    ,
       0.9    ,
       1.0    ,
       1.1
      }
};

float32 CurrentCharge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
     {
       0.0    ,
       10.0   ,
       20.0   ,
       30.0   ,
       40.0   ,
       50.0   ,
       60.0
     },
     {
             0.0    ,
             10.0   ,
             20.0   ,
             30.0   ,
             40.0   ,
             50.0   ,
             60.0
     },
     {
             0.0    ,
             10.0   ,
             20.0   ,
             30.0   ,
             40.0   ,
             50.0   ,
             60.0
     },
     {
             0.0    ,
             10.0   ,
             20.0   ,
             30.0   ,
             40.0   ,
             50.0   ,
             60.0
     }
};
float32 CurrentDischarge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
        {
          0.0    ,
          10.0   ,
          20.0   ,
          30.0   ,
          40.0   ,
          50.0   ,
          60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        }
   };
float32 CurrentCharge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
        {
          0.0    ,
          10.0   ,
          20.0   ,
          30.0   ,
          40.0   ,
          50.0   ,
          60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        }
   };
float32 CurrentDischarge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
        {
          0.0    ,
          10.0   ,
          20.0   ,
          30.0   ,
          40.0   ,
          50.0   ,
          60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        },
        {
                0.0    ,
                10.0   ,
                20.0   ,
                30.0   ,
                40.0   ,
                50.0   ,
                60.0
        }
   };

#endif
//
//**********************BOARD_01_END************************
//
//
//**********************BOARD_02_START**********************
//CH 5, 6, 7, 8
#if CONTROL_BOARD == 2
//---------------------------------------------------------------------------------------------------------
float32 VoltageCharge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
        {
          0.0005,
          1.0005,
          2.0005,
          3.0005,
          4.0005,
          5.0005
        },
        {
          0.0,
          0.9994,
          1.9994,
          2.9994,
          3.9994,
          4.9994
        },
        {
          0.0001,
          1.0001,
          2.0001,
          3.0001,
          4.0001,
          5.0001
        },
        {
          0.0001,
          1.0001,
          2.0001,
          3.0001,
          4.0001,
          5.0001
        }
};
float32 VoltageDischarge_REF[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
        {
          0.0,
          0.9997,
          1.9997,
          2.9997,
          3.9997,
          4.9997
        },
        {
          0.0,
          0.99895,
          1.99895,
          2.99895,
          3.99895,
          4.99895
        },
        {
          0.0004,
          1.0004,
          2.0004,
          3.0004,
          4.0004,
          5.0004
        },
        {
          0.0,
          0.9998,
          1.9998,
          2.9998,
          3.9998,
          4.9998
        }
};
float32 VoltageCharge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
        {
         0.000226974  ,
         0.999884307  ,
         1.99997592   ,
         2.9999671    ,
         4.00010681   ,
         4.99997997
        },
        {
         0.000130653  ,
         1.0002166    ,
         2.00031018   ,
         3.0002799    ,
         4.0004077    ,
         5
        },
        {
         0.000354862 ,
         1.00034904  ,
         2.00034118  ,
         3.00024223  ,
         4.00035143  ,
         5
        },
        {
         0.000343323  ,
         0.999731898  ,
         1.9998033    ,
         2.99978566   ,
         3.99992776   ,
         4.99985838
        }
};
float32 VoltageDischarge_ADC[4][CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH ] = {
        {
         0.000228977  ,
         0.999887586  ,
         1.99996829   ,
         2.999964     ,
         4.00011683   ,
         4.99998474
        },
        {
         0.000133038  ,
         1.00021791   ,
         2.00030208   ,
         3.00029516   ,
         4.00042582   ,
         5
        },
        {
         0.000361824  ,
         1.00035453   ,
         2.00034904   ,
         3.0002346    ,
         4.00034475   ,
         5
        },
        {
         0.000343323  ,
         0.999734104  ,
         1.99981439   ,
         2.99981236   ,
         3.99993253   ,
         4.99986124
        }
};



float32 CurrentCharge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
       {
         0.072434567 ,
         0.172547    ,
         0.272660    ,
         0.372912    ,
         0.472805    ,
         0.573037    ,
         0.673150    ,
         0.773782    ,
         0.873854    ,
         0.974067    ,
         1.073699    ,
         1.173632
       },
       {
         0.016518761 ,
         0.116491    ,
         0.216464    ,
         0.316736    ,
         0.416869    ,
         0.516861    ,
         0.616974    ,
         0.716926    ,
         0.816639    ,
         0.916851    ,
         1.016744    ,
         1.117036
       },
       {
         -0.032777542 ,
         0.067095     ,
         0.166967     ,
         0.266760     ,
         0.366852     ,
         0.466585     ,
         0.566358     ,
         0.666330     ,
         0.766663     ,
         0.866615     ,
         0.966388     ,
         1.066100
       },
       {
          -0.017978652 ,
          0.082214     ,
          0.182406     ,
          0.282439     ,
          0.382471     ,
          0.482604     ,
          0.582636     ,
          0.682389     ,
          0.781761     ,
          0.881654     ,
          0.981306     ,
          1.081319
       }
};

float32 CurrentDischarge_Low_REF[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
      {
         0.088733345 ,
         0.188826    ,
         0.288918    ,
         0.389191    ,
         0.489323    ,
         0.589356    ,
         0.689408    ,
         0.790201    ,
         0.890453    ,
         0.990766    ,
         1.090138    ,
         1.190231
      },
      {
         -0.059135565 ,
         0.040977     ,
         0.141089     ,
         0.241222     ,
         0.341294     ,
         0.441507     ,
         0.541539     ,
         0.641572     ,
         0.741404     ,
         0.841517     ,
         0.941369     ,
         1.041342
      },
      {
         0.015698823 ,
         0.115591    ,
         0.215484    ,
         0.315316    ,
         0.415189    ,
         0.515061    ,
         0.614954    ,
         0.714986    ,
         0.815239    ,
         0.915011    ,
         1.014804    ,
         1.114696
      },
      {
         -0.001759868 ,
         0.098433     ,
         0.198625     ,
         0.298578     ,
         0.398590     ,
         0.498703     ,
         0.598575     ,
         0.698228     ,
         0.797780     ,
         0.897873     ,
         0.997465     ,
         1.097498
      }
};

float32 CurrentCharge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
       {
         0.077943593 ,
         0.177963    ,
         0.277982    ,
         0.378046    ,
         0.478044    ,
         0.578060    ,
         0.677950    ,
         0.778551    ,
         0.878723    ,
         0.978830    ,
         1.078217    ,
         1.178224
       },
       {
          -0.024066925 ,
          0.075800     ,
          0.175667     ,
          0.275763     ,
          0.375762     ,
          0.475751     ,
          0.575765     ,
          0.675593     ,
          0.775225     ,
          0.875300     ,
          0.975188     ,
          1.075218
       },
       {
          -0.01157546 ,
          0.088499    ,
          0.188573    ,
          0.288460    ,
          0.388631    ,
          0.488486    ,
          0.588452    ,
          0.688602    ,
          0.788986    ,
          0.889037    ,
          0.989053    ,
          1.089026
       },
       {
          -0.009944126 ,
          0.090127     ,
          0.190199     ,
          0.290155     ,
          0.390194     ,
          0.490140     ,
          0.590063     ,
          0.689667     ,
          0.789043     ,
          0.888958     ,
          0.988546     ,
          1.088447
       }
};
float32 CurrentDischarge_Low_ADC[4][CALIBRATION_LOW_CURRENT_MAX_ARRAY_LENGTH ] = {
      {
        0.083432137 ,
        0.183476    ,
        0.283519    ,
        0.383549    ,
        0.483651    ,
        0.583669    ,
        0.683649    ,
        0.784215    ,
        0.884333    ,
        0.984343    ,
        1.083919    ,
        1.183871
      },
      {
        -0.017936334 ,
        0.081943     ,
        0.181823     ,
        0.281843     ,
        0.381838     ,
        0.482026     ,
        0.581852     ,
        0.681748     ,
        0.781400     ,
        0.881323     ,
        0.981077     ,
        1.081121
      },
      {
         -0.005085498 ,
         0.094904     ,
         0.194893     ,
         0.294902     ,
         0.394995     ,
         0.495066     ,
         0.594954     ,
         0.695143     ,
         0.795739     ,
         0.895541     ,
         0.995579     ,
         1.095443
      },
      {
         -0.009222969 ,
         0.090866     ,
         0.190955     ,
         0.290879     ,
         0.390746     ,
         0.490765     ,
         0.590886     ,
         0.690403     ,
         0.789765     ,
         0.889886     ,
         0.989478     ,
         1.089375
      }
};

float32 CurrentCharge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
     {
       0.072094593 ,
       10.069325   ,
       20.066555   ,
       30.063445   ,
       40.055856   ,
       50.049106   ,
       60.04235682
     },
     {
       0.022198335 ,
       10.005790   ,
       19.989381   ,
       29.979872   ,
       39.961663   ,
       49.942354   ,
       59.92304577
     },
     {
      -0.028757844  ,
       9.951654     ,
       19.932065    ,
       29.911637    ,
       39.885709    ,
       49.862760    ,
       59.83981201
     },
     {
        -0.015698823 ,
        9.955753     ,
        19.927205    ,
        29.895978    ,
        39.864430    ,
        49.829803    ,
        59.79517536
     }
};
float32 CurrentDischarge_REF[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
     {
      0.088813339 ,
      10.085364   ,
      20.081914   ,
      30.077404   ,
      40.068295   ,
      50.059526   ,
      60.05075619
     },
     {
        -0.052876034 ,
        9.927835     ,
        19.908547    ,
        29.894358    ,
        39.870330    ,
        49.844382    ,
        59.81843362
     },
     {
       0.016398469    ,
       10.000050     ,
       19.979702     ,
       29.958073     ,
       39.930625     ,
       49.906717     ,
       59.88280879
     },
     {
        0.00173987   ,
        9.971652     ,
        19.941564    ,
        29.907657    ,
        39.872890    ,
        49.835462    ,
        59.79803515
     }
};
float32 CurrentCharge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
     {
      0.0772915   ,
      10.0651779  ,
      20.0530643  ,
      30.0415363  ,
      40.0278702  ,
      50.0167809  ,
      60.0056916
     },
     {
        -0.01917646 ,
        9.95302677  ,
        19.92523    ,
        29.9054279  ,
        39.8809471  ,
        49.8568726  ,
        59.8327981
     },
     {
        -0.0114553403 ,
        9.99344158  ,
        19.9883385  ,
        29.9829903  ,
        39.9747047  ,
        49.9718056  ,
        59.9689065
     },
     {
        -0.00298225973 ,
        9.957304   ,
        19.9228592 ,
        29.8864594 ,
        39.8530159 ,
        49.8179283 ,
        59.7828407
     }
};
float32 CurrentDischarge_ADC[4][CALIBRATION_CURRENT_MAX_ARRAY_LENGTH ] = {
     {
      0.0825901   ,
      10.0702696  ,
      20.0579491  ,
      30.0454655  ,
      40.0319023  ,
      50.0201263  ,
      60.0083503
     },
     {
        -0.0132427 ,
        9.9568119  ,
        19.9268665 ,
        29.9035034 ,
        39.8745766 ,
        49.8458061 ,
        59.8170356
     },
     {
        -0.00798226 ,
        9.98689747  ,
        19.9817772  ,
        29.9766598  ,
        39.9683876  ,
        49.965271   ,
        59.9621544
     },
     {
        -0.0067921 ,
        9.957757   ,
        19.9223061 ,
        29.8844585 ,
        39.8486061 ,
        49.8117561 ,
        59.7749061
     }
};

#endif
//
//**********************BOARD_02_END************************
//


void CALIBRATION_Init(Uint16 channel)
{
    float32 Temp = 0.0;
    Uint16 ui_i = 0;
#ifdef _DEBUG_PRINTF_CALL
    printf("\n");
    printf("\n");
#endif
    for(ui_i = 0; ui_i < CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH; ui_i++)
    {
        Temp = VoltageCharge_REF[channel][ui_i];
        VoltageCharge_REF[channel][ui_i] = (float32)(((float32)(ui_i) * (float32)(CALIBRATION_VOLATAGE_UNIT_VALUE)) - Temp);
        Temp = VoltageDischarge_REF[channel][ui_i];
        VoltageDischarge_REF[channel][ui_i] = (float32)(((float32)(ui_i) * (float32)(CALIBRATION_VOLATAGE_UNIT_VALUE)) - Temp);
    }

#ifdef _DEBUG_PRINTF_CALL
    for(ui_i = 0; ui_i < CALIBRATION_VOLATAGE_MAX_ARRAY_LENGTH; ui_i++)
        printf("VoltageCharge_REF = %f, VoltageDischarge_REF = %f\n",VoltageCharge_REF[ui_i], VoltageDischarge_REF[ui_i]);
#endif

    for(ui_i = 0; ui_i < CALIBRATION_CURRENT_MAX_ARRAY_LENGTH; ui_i++)
    {
        Temp = CurrentCharge_REF[channel][ui_i];
        CurrentCharge_REF[channel][ui_i] = (float32)(((float32)(ui_i) * (float32)(CALIBRATION_CURRENT_UNIT_VALUE)) - Temp);
        Temp = CurrentDischarge_REF[channel][ui_i];
        CurrentDischarge_REF[channel][ui_i] = (float32)(((float32)(ui_i) * (float32)(CALIBRATION_CURRENT_UNIT_VALUE)) - Temp);
    }
#ifdef _DEBUG_PRINTF_CALL
    for(ui_i = 0; ui_i < CALIBRATION_CURRENT_MAX_ARRAY_LENGTH; ui_i++)
        printf("CurrentCharge_REF = %f, CurrentDischarge_REF = %f\n",CurrentCharge_REF[ui_i], CurrentDischarge_REF[ui_i]);
#endif
}

//float32 CALIBRATION_ConversionREF(float REF_Data, float *CAL_Data, float InitialValue, float Unit, Uint16 ArrayLength)
//{
//    Uint32 ArrayAddressValue = 0.0;
//
//    float RefRateValue = 0.0;
//    float RefDeltaValue = 0.0;
//    float ErrorValue = 0.0;
//    float ErrorValuePlus = 0.0;
//    float ErrorResult = 0.0;
//    float Result = 0.0;
//                                                                    // VARIABLE_CurrentRef (PC에서 보내주는 현재 측정 전류)
//    if(REF_Data < InitialValue && REF_Data != 0.0)                  // REF 데이터가 InitialValue(Min값->0.0)보다 작고(and) REF 데이터가 0.0이 아니면
//    {
//        Result = (float32)(REF_Data);                               // REF 데이터를 Result에 넣어라
//        return (Result);
//    }
//                                     //ex)    {(10000 - 0.0 ) * 10000.0 } / (10000.0 * 10.0 ) => ArrayAddressValue는 8
//    ArrayAddressValue   = (Uint32)((Uint32)((REF_Data - InitialValue) * 10000.0) / (Uint32)(10000.0 * Unit));
//                                             // ((10000 - 0.0)*10000.0)/ (10000.0 * 10.0) = 1000
//    ErrorValue          = ((((float)(ArrayAddressValue) * Unit) + InitialValue)     - CAL_Data[ArrayAddressValue]);
//                           // ex)    [{(float) (8) * 10.0} + 0.0 ] - CurrentDischarge_REF[8번째 값 = 측정한 값] = 아마 + 0.071 혹은 - 0.071 이런 값이 나옴 ex) 0.035
//    if(ArrayAddressValue < ArrayLength) // 미리 설정한 CALIBRATION_CURRENT_MAX_ARRAY_LENGTH 값보다 계산된 ArrayAddressValue가 작다면(Cal을 적용해야 하는 상황이라면)
//    {
//        ErrorValuePlus  = ((((float)(ArrayAddressValue + 1) * Unit) + InitialValue) - CAL_Data[ArrayAddressValue + 1]); // 계산된 ArrayAddressValue의 한 스텝 윗값의 ErrorValue를 구해서
//        RefDeltaValue   = (ErrorValuePlus - ErrorValue);                                                                // RefDeltaValue(변화량) = 한 스텝 윗값의 ErrorValuePlus - 현 스텝 ErrorValue이고
//                                                                                                                        //  ex) RefDeltaValue = 0.036 - 0.035 = 0.001
//                        // ex)    (80.0 - 0.0 ) - [{(float) (8) * 10.0 } / 10.0 ] = 72.0
//        RefRateValue    = (((REF_Data - InitialValue) - ((float)(ArrayAddressValue) * Unit)) / Unit);                   //
////        RefRateValue    = ((REF_Data - InitialValue) / Unit); //보정값 범위내 설정값 비율
//        ErrorResult     = (RefDeltaValue * RefRateValue);                   // ex) ErrorResult는 0.001 * 72.0 = 0.072
//    }
//    else
//    {
//        ErrorResult = 0.0;
//    }
//    Result = (REF_Data + ErrorValue) + ErrorResult;                         // 그래서 ex) Result는 ( 80.0 + 0.035) + 0.072 = 80.107
//
//    if(CAL_Data[ArrayAddressValue] == 0.0) Result = (float32)(REF_Data);
//    return (Result);
//}
float32 CALIBRATION_ConversionREF(float32 REF_Data, float32 *CAL_Data, float32 InitialValue, float Unit, Uint16 ArrayLength)
{
    Uint16 ArrayAddressValue = 0.0;
    Uint16 ui_i = 0;

    float32 Result = 0.0;
    float32 CalGain = 0.0;
    float32 CalOffset = 0.0;

    if(REF_Data >= CAL_Data[0])
    {
        for(ui_i = 0; ui_i < ArrayLength; ui_i++)
        {
            if(CAL_Data[ui_i] >= REF_Data)
            {
                break;
            }
        }
        if (ui_i < ArrayLength)
            ArrayAddressValue = ui_i;
        else
            ArrayAddressValue = ui_i - 1;
    }
    else
    {
        ArrayAddressValue = 1;
    }

    CalGain = Unit / (CAL_Data[ArrayAddressValue] - CAL_Data[ArrayAddressValue - 1]);               // CalGain = (xH - xL)/(yH - yL)
    CalOffset = (CAL_Data[ArrayAddressValue - 1] * CalGain) - ((ArrayAddressValue - 1) * Unit);     // CalOffset = yL * CalGain - xL
    Result = REF_Data * CalGain - CalOffset;                                                        // x = y * CalGain - CalOffset

    return (Result);
}
//float32 CALIBRATION_ConversionADC(float ADC_Data, float *Measure_Data, float *CAL_Data, float InitialValue, float Unit, Uint16 ArrayLength)
//{
//    Uint16 ui_i = 0;
//    Uint16 ArrayAddressValue = 0.0;
//    float AdcRateValue = 0.0;
//    float AdcDeltaValue = 0.0;
//    float MeasureDeltaValue = 0.0;
//    float ErrorValueResult = 0.0;
//    float Result = 0.0;
//
//    //ADC_Data 양수
//    //CAL_Data 양수
//    //InitialValue 양수
//    for(ui_i = ArrayAddressValue; ui_i < ArrayLength; ui_i++)
//    {
//        if(CAL_Data[ui_i] >= ADC_Data)break;
//        else ArrayAddressValue++;
//    }
//
//    if(ADC_Data < InitialValue)
//    {
//        Result = (float32)(ADC_Data);
//        return (Result);
//    }
//    else if(ArrayAddressValue < ArrayLength)
//    {
//        AdcDeltaValue       = (CAL_Data[ArrayAddressValue + 1] - CAL_Data[ArrayAddressValue]) - Unit;
//        MeasureDeltaValue   = (Measure_Data[ArrayAddressValue + 1] - Measure_Data[ArrayAddressValue]);
//        AdcRateValue        = (ADC_Data - Measure_Data[ArrayAddressValue]) / MeasureDeltaValue ; //보정값 범위내 설정값 비율
//
//        ErrorValueResult    = (AdcDeltaValue * AdcRateValue);
//    }
//    else
//    {
//        ErrorValueResult    = 0.0;
//    }
//
//    //Result = ((float)(ArrayAddressValue) * Unit) + ((Measure_Data[ArrayAddressValue] - ADC_Data) + InitialValue) + ErrorValueResult;
//    Result = CAL_Data[ArrayAddressValue] + ErrorValueResult;
//
//    if(CAL_Data[ArrayAddressValue] == 0.0) Result = (float32)(ADC_Data);
//
//    return (Result);
//}

//float32 CALIBRATION_ConversionCurrentADC(float32 ADC_Data, float32 *Measure_Data, float32 *CAL_Data, float32 InitialValue, float32 Unit, Uint16 ArrayLength)
//{
//    Uint16 ArrayAddressValue = 0.0;
//    Uint16 ui_i = 0;
//    float AdcRateValue = 0.0;
//    float AdcDeltaValue = 0.0;
//    float MeasureDeltaValue = 0.0;
//    float ErrorValueResult = 0.0;
//    float Result = 0.0;
//    Uint16 Flag = CLEAR;
//    //ADC_Data 양수
//    //CAL_Data 양수
//    //InitialValue 양수
//    if(ADC_Data >= InitialValue)
//    {
////        ArrayAddressValue = ((Uint16)(((ADC_Data - InitialValue) / Unit) * 1000)) / (Uint16)(1000);
//        for(ui_i = 0; ui_i < ArrayLength; ui_i++)
//        {
//            if(Measure_Data[ui_i] >= ADC_Data)
//            {
//                Flag = SET;
//                ArrayAddressValue = ui_i;
//                break;
//            }
////            else ArrayAddressValue++;
//        }
//        if(Flag == CLEAR)
//        {
//            ArrayAddressValue = ArrayLength - 1;
//        }
//    }
//    else
//    {
//        Result = (float32)(ADC_Data);
//        return (Result);
//    }
//
//    if(ArrayAddressValue < ArrayLength - 1)
//    {
//
//        AdcDeltaValue       = (((((ArrayAddressValue + 1) * Unit) + InitialValue) - (CAL_Data[ArrayAddressValue + 1])) - (((ArrayAddressValue * Unit) + InitialValue) - CAL_Data[ArrayAddressValue]));
//
//        MeasureDeltaValue   = (Unit);
//        AdcRateValue        = (ADC_Data - Measure_Data[ArrayAddressValue]) / MeasureDeltaValue ; //보정값 범위내 설정값 비율
//
//        ErrorValueResult    = (CAL_Data[ArrayAddressValue] - Measure_Data[ArrayAddressValue]) + (AdcDeltaValue * ABS_F(AdcRateValue));
//        Result = ADC_Data + ErrorValueResult;
//    }
//    else //if(ArrayAddressValue < ArrayLength)
//    {
//        ErrorValueResult    = (CAL_Data[ArrayLength - 1] - Measure_Data[ArrayLength - 1]);
//        Result = ADC_Data + ErrorValueResult;
//    }
////    else
////    {
////        ErrorValueResult    = (CAL_Data[ArrayAddressValue - 1] - Measure_Data[ArrayAddressValue - 1]);
////        Result = ADC_Data + ErrorValueResult;
////    }
//
//    if(CAL_Data[ArrayAddressValue] == 0.0) Result = (float32)(ADC_Data);
//
//    return (Result);
////    if(ADC_Data >= InitialValue)ArrayAddressValue = ((Uint16)(((ADC_Data - InitialValue) / Unit) * 1000)) / (Uint16)(1000);
////    else
////    {
////        Result = (float32)(ADC_Data);
////        return (Result);
////    }
////
////    if(ArrayAddressValue < ArrayLength - 1)
////    {
////
////        AdcDeltaValue       = ((CAL_Data[ArrayAddressValue + 1] - Measure_Data[ArrayAddressValue + 1]) -
////                               (CAL_Data[ArrayAddressValue])    - Measure_Data[ArrayAddressValue]);
////
////        MeasureDeltaValue   = (Unit);
////        AdcRateValue        = (ADC_Data - Measure_Data[ArrayAddressValue]) / MeasureDeltaValue ; //보정값 범위내 설정값 비율
////
////        ErrorValueResult    = (CAL_Data[ArrayAddressValue] - Measure_Data[ArrayAddressValue]) + (AdcDeltaValue * ABS_F(AdcRateValue));
////        Result = ADC_Data + ErrorValueResult;
////    }
////    else if(ArrayAddressValue < ArrayLength)
////    {
////        ErrorValueResult    = (CAL_Data[ArrayAddressValue] - Measure_Data[ArrayAddressValue]);
////        Result = ADC_Data + ErrorValueResult;
////    }
////    else
////    {
////        ErrorValueResult    = (CAL_Data[ArrayAddressValue - 1] - Measure_Data[ArrayAddressValue - 1]);
////        Result = ADC_Data + ErrorValueResult;
////    }
////
////    if(CAL_Data[ArrayAddressValue] == 0.0) Result = (float32)(ADC_Data);
////
////    return (Result);
//}
float32 CALIBRATION_ConversionCurrentADC(float32 ADC_Data, float32 *Measure_Data, float32 *CAL_Data, float32 InitialValue, float32 Unit, Uint16 ArrayLength)
{
    Uint16 ArrayAddressValue = 0.0;
    Uint16 ui_i = 0;

    float Result = 0.0;
    float CalGain = 0.0;
    float CalOffset = 0.0;

    if(ADC_Data > Measure_Data[0])
    {
        for(ui_i = 0; ui_i < ArrayLength; ui_i++)
        {
            if(Measure_Data[ui_i] >= ADC_Data)
            {
                break;
            }
        }
        if (ui_i < ArrayLength)
            ArrayAddressValue = ui_i;
        else
            ArrayAddressValue = ui_i - 1;
    }
    else
    {
        ArrayAddressValue = 1;
    }

    CalGain = (CAL_Data[ArrayAddressValue] - CAL_Data[ArrayAddressValue - 1]) / (Measure_Data[ArrayAddressValue] - Measure_Data[ArrayAddressValue - 1]);    //CalGain = (xH - xL)/(yH - yL)
    CalOffset = (Measure_Data[ArrayAddressValue - 1] * CalGain) - CAL_Data[ArrayAddressValue - 1];                                                          //CalOffset = yL * CalGain - xL
    Result = ADC_Data * CalGain - CalOffset;                                                                                                                //x = y * CalGain - CalOffset

    return (Result);
}
//float32 CALIBRATION_ConversionCurrentADC(float32 ADC_Data, float32 *Measure_Data, float32 *CAL_Data, float32 InitialValue, float32 Unit, Uint16 ArrayLength)
//{
//    Uint16 ui_i = 0;
//    Uint16 ArrayAddressValue = 0.0;
//    float AdcRateValue = 0.0;
//    float AdcDeltaValue = 0.0;
//    float MeasureDeltaValue = 0.0;
//    float ErrorValueResult = 0.0;
//    float Result = 0.0;
//
//    //ADC_Data 양수
//    //CAL_Data 양수
//    //InitialValue 양수
//    for(ui_i = ArrayAddressValue; ui_i < ArrayLength; ui_i++)
//    {
//        if(CAL_Data[ui_i] >= ADC_Data)break;
//        else ArrayAddressValue++;
//    }
//
//    if(ADC_Data < InitialValue)
//    {
//        Result = (float32)(ADC_Data);
//        return (Result);
//    }
//    else if(ArrayAddressValue < ArrayLength)
//    {
//        AdcDeltaValue       = (CAL_Data[ArrayAddressValue + 1] - CAL_Data[ArrayAddressValue]) - Unit;
//        MeasureDeltaValue   = (Measure_Data[ArrayAddressValue + 1] - Measure_Data[ArrayAddressValue]);
//        AdcRateValue        = (ADC_Data - Measure_Data[ArrayAddressValue]) / MeasureDeltaValue ; //보정값 범위내 설정값 비율
//
//        ErrorValueResult    = (AdcDeltaValue * AdcRateValue);
//    }
//    else
//    {
//        ErrorValueResult    = 0.0;
//    }
//
//    //Result = ((float)(ArrayAddressValue) * Unit) + ((Measure_Data[ArrayAddressValue] - ADC_Data) + InitialValue) + ErrorValueResult;
//    Result = CAL_Data[ArrayAddressValue] + ErrorValueResult;
//
//    if(CAL_Data[ArrayAddressValue] == 0.0) Result = (float32)(ADC_Data);
//
//    return (Result);
//}

//float32 CALIBRATION_ConversionVoltageADC(float32 ADC_Data, float32 *CAL_Data, float32 InitialValue, float32 Unit, Uint16 ArrayLength)
//{
//    Uint16 ArrayAddressValue = 0.0;
//    Uint16 ui_i = 0;
//    float AdcRateValue = 0.0;
//    float AdcDeltaValue = 0.0;
//    float MeasureDeltaValue = 0.0;
//    float ErrorValueResult = 0.0;
//    float Result = 0.0;
//    Uint16 Flag = CLEAR;
//    //ADC_Data 양수
//    //CAL_Data 양수
//    //InitialValue 양수
//    if(ADC_Data >= InitialValue)
//    {
////        ArrayAddressValue = ((Uint16)(((ADC_Data - InitialValue) / Unit) * 1000)) / (Uint16)(1000);
//        for(ui_i = 0; ui_i < ArrayLength; ui_i++)
//        {
//            if(CAL_Data[ui_i] >= ADC_Data)
//            {
//                Flag = SET;
//                ArrayAddressValue = ui_i;
//                break;
//            }
////            else ArrayAddressValue++;
//        }
//        if(Flag == CLEAR)
//        {
//            ArrayAddressValue = ArrayLength - 1;
//        }
//    }
//    else
//    {
//        Result = (float32)(ADC_Data);
//        return (Result);
//    }
//
//    if(ArrayAddressValue < ArrayLength - 1)
//    {
//        AdcDeltaValue       = (((((ArrayAddressValue + 1) * Unit) + InitialValue) - (CAL_Data[ArrayAddressValue + 1])) -
//                                (((ArrayAddressValue * Unit)      + InitialValue) -  CAL_Data[ArrayAddressValue]));
//
//        MeasureDeltaValue   = (Unit);
//        AdcRateValue        = (ADC_Data - CAL_Data[ArrayAddressValue]) / MeasureDeltaValue ; //보정값 범위내 설정값 비율
//
//        ErrorValueResult    = (((ArrayAddressValue * Unit) + InitialValue) - CAL_Data[ArrayAddressValue]) + (AdcDeltaValue * ABS_F(AdcRateValue));
//        Result = ADC_Data + ErrorValueResult;
//    }
//    else
//    {
//        ErrorValueResult    = (((ArrayAddressValue * Unit) + InitialValue) - CAL_Data[ArrayLength - 1]);
//        Result = ADC_Data + ErrorValueResult;
//    }
//
//    if(CAL_Data[ArrayAddressValue] == 0.0) Result = (float32)(ADC_Data);
//
//    return (Result);
//}
float32 CALIBRATION_ConversionVoltageADC(float32 ADC_Data, float32 *CAL_Data, float32 InitialValue, float32 Unit, Uint16 ArrayLength)
{
    Uint16 ArrayAddressValue = 0;
    Uint16 ui_i = 0;

    float Result = 0.0;
    float CalGain = 0.0;
    float CalOffset = 0.0;

    if(ADC_Data > CAL_Data[0])
    {
        for(ui_i = 0; ui_i < ArrayLength; ui_i++)
        {
            if(CAL_Data[ui_i] >= ADC_Data)
            {
                break;
            }
        }
        if (ui_i < ArrayLength)
            ArrayAddressValue = ui_i;
        else
            ArrayAddressValue = ui_i - 1;
    }
    else
    {
        return (ADC_Data);
    }

    CalGain = Unit / (CAL_Data[ArrayAddressValue] - CAL_Data[ArrayAddressValue - 1]);               //CalGain = (xH - xL)/(yH - yL)
    CalOffset = (CAL_Data[ArrayAddressValue - 1] * CalGain) - ((ArrayAddressValue - 1) * Unit);     //CalOffset = yL * CalGain - xL
    Result = ADC_Data * CalGain - CalOffset;                                                        //x = y * CalGain - CalOffset

    return (Result);
}
//
// End of file
//

