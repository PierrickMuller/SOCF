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
 * Context              : SOCF Test 1
 *
 *****************************************************************************************
 * Brief: Programme correspondant au demandes faites durant le test 1 de SOCF (15.05.2020)
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
#include "address_map_arm.h"
#include "defines.h"
#include "exceptions.h"


// Tableau permettant de gérer l'affichage sur les 7 segments
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

    int value;
    unsigned int const_value;
    
    // PROCESSUS D'ACTIVATION DES INTERUPTIONS et de setup
    
    //On set la gpio led en mode sortie.
    GPIO1_SWPORTA_DDR |= MASK_GPIO53_LED;
    
    //On réinitialise à 0 les valeurs des leds et des afficheurs 7 segments
    LEDS = 0x0;
    HEX2_0 = ~0x0;
    HEX5_3 = ~0x0;
    
    // Initialize the banked stack pointer register for IRQ mode
    set_A9_IRQ_stack();

    // On active l'envoi d'interruption au core via l'interface CPU 0
    ICCICR = 1;

    // On met le niveau nécéssaire pour qu'une interruption soit transmise au cpu au minimum afin que toutes les interruptions passent.
    ICCPMR = 0xFFFF;
    
    // On active le distributeur
    ICDDCR = 1;
    
    // On calcule la valeur à mettre dans le registre ICDISER (Interrupt Set Enable Registers) et on remplit le registre
    // Le calcul pour la valeur peut être trouvé dans le document sous source dans l'entête.
    value = 0x1<<(72%32); 
    ICDISER |= value;
    
    //On renseigne que notre interruption doit être envoyée à l'interface CPU 0
    ICDIPTR = 1;
    

    // On active les interruptions pour tous les boutons
    KEYS_INTERRUPT_ENABLE = 0xF;
    
    // On active les interruptions sur le processeur
    enable_A9_interrupts();

    
    //PARTIE LANCEMENT DU PROGRAMME 
    CONST_REGISTER = 0x11111111;
    const_value = CONST_REGISTER;
    // On effectue les manipulations demandées pour l'afficheur 7 au lancement du programme 
    HEX2_0 = ~(0x0 | (temp[(const_value & 0xF00) >> 8] << 16) | (temp[(const_value & 0xF0) >> 4 ] << 8) | (temp[(const_value & 0xF)]));
    HEX5_3 = ~(0x0 | (temp[(const_value & 0xF00000) >> 20] << 16) | (temp[(const_value & 0xF0000) >> 16] << 8) | temp[(const_value & 0xF000) >> 12] );
    
    TEST_REGISTER = CONST_REGISTER;
    if(TEST_REGISTER != CONST_REGISTER)
    {
        LEDS = 0x3FF;
    }
  
    while(!(GPIO1_EXT_PORTA & MASK_GPIO54_KEY))
    {
        
    }
    LEDS = 0xAA;
  
    while(1)
    {
	  
    }
}
