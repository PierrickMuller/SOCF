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

  enable_A9_interrupts();
  set_A9_IRQ_stack();


  ICCICR |= 0x0;
  ICDDCR |= 0x0;
  ICCPMR = 255;
  //Set enable bit
  ICDISER |= 0xFFFFFFFF;//0x1 << (72 % 32);
  KEYS_INTERRUPT |= 0xc;
  //ICDIPTR |= 0x01 << (72 % 4);
  ICDIPTR = 01010101;
  //ICDICFR =  |= EDGE_TRIGGERED << ((72 % 16) + 1);
  ICDICFR =  0xFFFFFFFF;
  ICCICR |= 0x1;
  ICDDCR |= 0x1;
  LEDS = 0x0;
  HEX3_0 = ~0x0;







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
