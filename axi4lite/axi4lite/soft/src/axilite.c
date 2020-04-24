/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : fpga_gpio.c
 * Author               : Pierrick Muller
 * Date                 : 21.02.2020
 *
 * Context              : SOCF tutorial lab
 *
 *****************************************************************************************
 * Brief: Programme permettant de gérer deux modes d'affichage pour les Leds et les afficheurs 7 segments
 * 		  en fonction de l'état des switchs (SOCF labo2)
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




int main(void){

  HEX3_0 = 0xFFFFFFFF;
  LEDS = 0x5;
  
  HEX4_5 = 0xFFFF;

    while(1)
    {
        if((KEYS & 0x1))
        {
            // On eteind les leds et l'afficheur 7 segements
            LEDS = SWITCHS;
            // On reporte l'état des switchs sur les leds
            //LEDS = SWITCHS;
          
        }
        else if(!(KEYS & 0x2))
        {
            // On eteind les leds et l'afficheur 7 segements
            LEDS = 0x0;
            // On reporte l'état des switchs sur les leds
            //LEDS = ~SWITCHS;
        }
        else if(!(KEYS & 0x4))
        {
            
        }
        else if(!(KEYS & 0x8))
        {
            
        }

    }
}
