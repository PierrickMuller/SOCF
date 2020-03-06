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

typedef volatile unsigned short vushort;
typedef unsigned short ushort;
typedef volatile unsigned int vuint;

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


#define BASE_ADRESSE          0xFF200000

//Addresse des registres permettant d'écrire, de set la direction des gpios et d'écrire sur la gpio
#define LEDS            *(vuint *) (BASE_ADRESSE + 0x0)
#define HEX3_0          *(vuint *) (BASE_ADRESSE + 0x20)
#define SWITCHS         *(vuint *) (BASE_ADRESSE + 0x40)
#define KEYS            *(vuint *) (BASE_ADRESSE + 0x50)


int main(void){

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
