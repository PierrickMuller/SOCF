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

  unsigned int const_value,hex30_val,hex45_val,leds_val;
  unsigned char debounce;
  HEX3_0 = ~0x0;
  HEX4_5 = ~0x0;
  LEDS = 0x0;
  debounce = 0;
    while(1)
    {
        //Permet de tester si la constante peut être modifiée ou pas
        //CONST_REG = 0x1;
        if(!(KEYS & 0x1))
        {
            HEX3_0 = ~0x0;
            HEX4_5 = ~0x0;
            LEDS = 0x0;
            
            LEDS = SWITCHS & MASK_SWITCH;
            
            const_value = CONST_REG;
            // On effectue les manipulations demandées pour l'afficheur 7 segements pour KEY1
            HEX3_0 = ~(0x0 | (temp[(const_value & 0xF000) >> 12] << 24) | (temp[(const_value & 0xF00) >> 8] << 16) | (temp[(const_value & 0xF0) >> 4 ] << 8) | (temp[(const_value & 0xF)]));
            HEX4_5 = ~(0x0 | (temp[(const_value & 0xF00000) >> 20] << 8) | (temp[(const_value & 0xF0000) >> 16] ) );
            
            
        }
        else if(!(KEYS & 0x2))
        {
            HEX3_0 = ~0x0;
            HEX4_5 = ~0x0;
            LEDS = 0x0;
            // On eteind les leds et l'afficheur 7 segements

            LEDS = ~SWITCHS & MASK_SWITCH;
            const_value = CONST_REG;
            // On effectue les manipulations demandées pour l'afficheur 7 segements pour KEY1
            HEX3_0 = ~(0x0 | (temp[(~const_value & 0xF000) >> 12] << 24) | (temp[(~const_value & 0xF00) >> 8] << 16) | (temp[(~const_value & 0xF0) >> 4 ] << 8) | (temp[(~const_value & 0xF)]));
            HEX4_5 = ~(0x0 | (temp[(~const_value & 0xF00000) >> 20] << 8) | (temp[(~const_value & 0xF0000) >> 16] ) );
        }
        else if(!(KEYS & 0x4))
        {
            // On deplace les leds vers la droite ainsi que les valeurs des afficheurs 7 segments
            if(debounce == 0)
            {
                leds_val = LEDS;
                LEDS = 0x0;
                LEDS = (leds_val >> 1) | ((leds_val & (0x1))<<9);
                hex30_val = HEX3_0;
                hex45_val = HEX4_5;
                HEX3_0 = ~0x0;
                HEX4_5 = ~0x0;
                HEX3_0 = (0x0 | ((hex45_val & (0x7F)) << 24) | ((hex30_val & (0x7F << 24)) >> 8) | ((hex30_val & (0x7F << 16)) >> 8) | ((hex30_val & (0x7F << 8))>>8));
                HEX4_5 = (0x0 | ((hex30_val & (0x7F)) << 8) | ((hex45_val & (0x7F << 8))>>8));
                debounce = 1;
            }
            
        }
        else if(!(KEYS & 0x8))
        {
            if(debounce == 0)
            {
                leds_val = LEDS;
                LEDS = (leds_val << 1) | ((leds_val & (0x1 << 9))>>9);
                hex30_val = HEX3_0;
                hex45_val = HEX4_5;
                HEX3_0 = ~0x0;
                HEX4_5 = ~0x0;
                HEX3_0 = (0x0 | ((hex30_val & (0x7F << 16)) << 8) | ((hex30_val & (0x7F << 8)) << 8) | ((hex30_val & (0x7F)) << 8) | ((hex45_val & (0x7F << 8))>> 8));
                HEX4_5 = (0x0 | ((hex45_val & (0x7F)) << 8) | ((hex30_val & (0x7F << 24))>>24));
                debounce = 1;
            }
        }
        else
        {
            debounce = 0;
        }

    }
}
