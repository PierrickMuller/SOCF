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

//Addresse de base pour les gpios 57 à 29
#define GPIO1_BASE_ADRESSE          0xFF709000

//Masques pour les différents élèments (KEY et LED)
#define MASK_GPIO54_KEY             0x1 << 25
#define MASK_GPIO53_LED             0x1 << 24

//Addresse des registres permettant d'écrire, de set la direction des gpios et d'écrire sur la gpio
#define GPIO1_SWPORTA_DR            *(vuint *) (GPIO1_BASE_ADRESSE + 0x0)
#define GPIO1_SWPORTA_DDR           *(vuint *) (GPIO1_BASE_ADRESSE + 0x4)
#define GPIO1_EXT_PORTA             *(vuint *) (GPIO1_BASE_ADRESSE + 0x50)

int main(void){

    //On set la gpio led en mode sortie.
    GPIO1_SWPORTA_DDR |= MASK_GPIO53_LED;
    while(1)
    {
      //Si le bouton est appuyé, on allume la led
      if(GPIO1_EXT_PORTA & MASK_GPIO54_KEY)
      {
        GPIO1_SWPORTA_DR &= ~(MASK_GPIO53_LED);
      }
      //Sinon on l'éteint
      else
      {
        GPIO1_SWPORTA_DR |= MASK_GPIO53_LED;
      }

    }
}
