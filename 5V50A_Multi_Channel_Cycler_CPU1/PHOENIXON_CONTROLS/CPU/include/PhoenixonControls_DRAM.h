/*
 * PhoenixonControls_DRAM.h
 *
 *  Created on: 2018. 5. 28.
 *      Author: BGKim
 */

#ifndef PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_DRAM_H_
#define PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_DRAM_H_

//ISSI IS42S86400D 512Mb SDRAM
//16M * 8(bit) * 4(Bank) = 64M(8bit)
//IS42S86400D * 2EA = 64M(8bit) + 64M(8bit) = 64M(16bit)
//64M(16bit) = Uint16 * 64M
//

#define SDRAM_CAPACITY		    536870912  // bits
#define SDRAM_DATA_BANDWIDTH    8		   // 8bit
#define SDRAM_BASE_ADDRESS	    EMIF_CS0_BASE_ADDRESS //CS0 End Address 0x0x8000 0000, End Address 0x8FFF FFFF
#define SDRAM_LENGTH	    	0x4000000 // 64M(16bit) (((SDRAM_CAPACITY / SDRAM_DATA_BANDWIDTH) + SDRAM_BASE_ADDRESS) + 1)
#define SDRAM_MEM_BUFFER_SIZE   0x500


Uint32 SDRAM_Init(void);
void SDRAM_ClearData(Uint32 Ul_Address, Uint32 ul_Size);
Uint32 SDRAM_Check(Uint32 Ul_Address, Uint32 ul_Size);



#endif /* PHOENIXON_CONTROLS_CPU_INCLUDE_PHOENIXONCONTROLS_DRAM_H_ */
