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

//Addresse des registres permettant d'écrire, de set la direction des gpios et d'écrire sur la gpio
#define LEDS            *(vuint *) (0xFF200000)
#define SWITCHS         *(vuint *) (0xFF200040)


int main(void){

    while(1)
    {
      LEDS = SWITCHS;
    }
}
