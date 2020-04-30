/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : axilite.c
 * Author               : Pierrick Muller
 * Date                 : 24.04.2020
 *
 * Context              : SOCF lab 3
 *
 *****************************************************************************************
 * Brief: Programme permettant de gérer deux modes d'affichage pour les Leds et les afficheurs 7 segments
 * 		  en fonction de l'état des switchs (SOCF labo3)
 *
 *****************************************************************************************
 * Sources : https://cyberlearn.hes-so.ch/pluginfile.php/3164985/mod_resource/content/2/gic_altera_manual_short.pdf
 * 
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Student      Comments
 *
 *
*****************************************************************************************/
#include "defines.h"

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

  HEX3_0 = ~0x0;
  HEX4_5 = ~0x0;
  LEDS = 0x0;

    while(1)
    {

        if(!(KEYS & 0x1))
        {
            LEDS = 0x2AA;
        }
        else if(!(KEYS & 0x2))
        {
            //HEX3_0 = ~0x0;
            //HEX4_5 = ~0x0;
            LEDS = 0x0;
            // On eteind les leds et l'afficheur 7 segements
            //LEDS = 0x2AA;//SWITCHS;
            LEDS = SWITCHS;
                        
            // On effectue les manipulations demandées pour l'afficheur 7 segements pour KEY1
            //HEX3_0 = ~(0x0 | (temp[~(CONST_REG & 0xF000) >> 12] << 24) | (temp[~(CONST_REG & 0xF00) >> 8] << 16) | (temp[~(CONST_REG & 0xF0) >> 4 ] << 8) | (temp[~(CONST_REG & 0xF)]));
            //HEX4_5 = ~(0x0 | (temp[~(CONST_REG & 0xF00000) >> 20] << 8) | (temp[~(CONST_REG & 0xF0000) >> 16] ) );
        }
        else if(!(KEYS & 0x4))
        {
            
        }
        else if(!(KEYS & 0x8))
        {
            
        }

    }
}
