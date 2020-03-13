/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : hps_gpio.c
 * Author               : Pierrick Muller
 * Date                 : 21.02.2020
 *
 * Context              : SOCF tutorial lab
 *
 *****************************************************************************************
 * Brief: light HPS user LED up when HPS user button pressed, for DE1-SoC board
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 *
 *
*****************************************************************************************/
#include "address_map_arm.h"
#include "defines.h"
#include "exceptions.h"
//typedef volatile unsigned short vushort;
//typedef unsigned short ushort;
//typedef volatile unsigned int vuint;

const char temp[16] = {
      0x3f, // 0
      0x06, // 1
      0x5b, // 2
      0xcf, // 3
      0xe6, // 4
      0xed, // 5
      0xfd, // 6
      0x87, // 7
      0xff, // 8
      0xef, // 9
      0x77, // A
      0x7C, // B
      0x39, // C
      0x5E, // D
      0x79, // E
      0x71  // F
  };

int main(void){

  int reg_offset, index, value, address;

  LEDS = 0x0;
  HEX3_0 = ~0x0;
  
  set_A9_IRQ_stack();

  reg_offset = (72>>3) & 0xFFFFFFFC;
  index = 72 & 0x1F;
  value = 0x1<<index;
  address = 0xFFFED100 + reg_offset;
  *(int*)address|= value;

  reg_offset = (72 & 0xFFFFFFFC);
  index = 72 & 0x3;
  address = 0xFFFED800 + reg_offset + index;
  *(char*)address = (char) 1;

  ICCPMR = 0xFFFF;
  ICCICR = 1;
  ICDDCR = 1;

  KEYS_INTERRUPT_ENABLE = 0xC;
  KEYS_INTERRUPT_REGISTER = 0xF;
  enable_A9_interrupts();

    while(1)
    {
      if(!(KEYS & 0x1))
      {
        LEDS = 0x0;
        HEX3_0 = ~0x0;
        LEDS = SWITCHS;
        HEX3_0 = ~(0x0 | (temp[(LEDS & 0x200) >> 9] << 24) | (temp[(LEDS & 0x100) >> 8] << 16) | (temp[(LEDS & 0xF0) >> 4 ] << 8) | (temp[(LEDS & 0xF)]));

      }
      else if(!(KEYS & 0x2))
      {
        LEDS = 0x0;
        HEX3_0 = ~0x0;
        LEDS = ~SWITCHS;
        HEX3_0 = ~(0x0 | (temp[(LEDS & 0x200) >> 9] << 24) | (temp[(LEDS & 0x100) >> 8] << 16) | (temp[(LEDS & 0xF0) >> 4 ] << 8) | (temp[(LEDS & 0xF)]));
      }
    }
}
