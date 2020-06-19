/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : test2.c
 * Author               : Pierrick Muller
 * Description          : J'ai vraiment pas compris ce test, en gros.

*****************************************************************************************/
#include "address_map_arm.h"
#include "defines.h"
#include "exceptions.h"

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

  unsigned int const_value,value;
  unsigned char debounce;
  HEX3_0 = ~0x0;
  HEX4_5 = ~0x0;
  LEDS = 0x0;
  debounce = 0;
  
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
 

  // On active les interruptions pour les boutons KEY3 et KEY2
  KEYS_INTERRUPT_ENABLE = 0xC;
  
  // On active les interruptions sur le processeur
  enable_A9_interrupts();
    while(1)
    {
        while(!(ENABLE & 0x1)){}

        if(!(KEYS & 0x1))
        {
            HEX3_0 = ~0x0;
            HEX4_5 = ~0x0;
            
        }
        else if(!(KEYS & 0x2))
        {
            
        }
        else if (!(KEYS & 0x4))
        {
            
        }
        
        
        if(SWITCH & (1 << 9)) 
        {
            
        }
        else 
        {
            
        }
        
        while(BUSY_AND_INTCAPTURE & (1 << 16)){}
        
        
        
       
    }
}
