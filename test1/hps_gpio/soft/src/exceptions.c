/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : execptions.c
 * Author               : Sébastien Masle
 * Date                 : 16.02.2018
 *
 * Context              : SOCF class
 *
 *****************************************************************************************
 * Brief: defines exception vectors for the A9 processor
 *        provides code that sets the IRQ mode stack, and that dis/enables interrupts
 *        provides code that initializes the generic interrupt controller
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      	Comments
 * 0.0    16.02.2018  SMS           	Initial version.
 * 0.1    21.03.2020  Muller Pierrick 	Ajout gestion IRQ pour Laboratoire 2 SOCF
*****************************************************************************************/
#include <stdint.h>

#include "address_map_arm.h"
#include "defines.h"

// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq(void)
{
  // On lit le registre de l'interface CPU pour savoir quel périphérique a causé l'interruption 
  int interrupt_ID = ICCIAR;
  int hex20_val,hex53_val,, press;

  press = KEYS_INTERRUPT_REGISTER;     // On récupère le bouton qui à causer l'interruption
  KEYS_INTERRUPT_REGISTER = press;     // On nettoie l'interruption dans le registre des interruptions pour les KEYS

  // Si l'interruption à été causer par un bouton
  if(interrupt_ID == 72)
  {
    if(press & 0x1) // KEY0 
    {
        //Etat des switchs sur leds 
        LEDS = SWITCHS;
    }
    if(press & 0x2) // KEY1 
    {
        //rotation droite en fonction de l'état de switch0 
      hex20_val = HEX2_0;
      hex53_val = HEX5_3;
      HEX2_0 = ~0x0;
      HEX5_3 = ~0x0;
      HEX2_0 = (0x0 | ((hex54_val & (0x7F)) << 16) | ((hex20_val & (0x7F << 16)) >> 8) | ((hex20_val & (0x7F << 8))>>8));
      HEX5_3 = (0x0 | ((hex20_val & (0x7F)) << 16) | ((hex54_val & (0x7F << 16)) >> 8) | ((hex54_val & (0x7F << 8))>>8));
      if(SWITCHS & 0x1)
      {
        hex20_val = HEX2_0;
        hex53_val = HEX5_3;
        HEX2_0 = ~0x0;
        HEX5_3 = ~0x0;
        HEX2_0 = (0x0 | ((hex54_val & (0x7F)) << 16) | ((hex20_val & (0x7F << 16)) >> 8) | ((hex20_val & (0x7F << 8))>>8));
        HEX5_3 = (0x0 | ((hex20_val & (0x7F)) << 16) | ((hex54_val & (0x7F << 16)) >> 8) | ((hex54_val & (0x7F << 8))>>8));
      }
        
    }
    if (press & 0x4)       // KEY2
    {
        // rotation gauche de 1 
	  hex20_val = HEX2_0;
      hex53_val = HEX5_3;
      HEX2_0 = ~0x0;
      HEX5_3 = ~0x0;
      HEX2_0 = (0x0 | ((hex20_val & (0x7F << 8)) << 8) | ((hex20_val & (0x7F)) << 8) | ((hex54_val & (0x7F << 16)) >> 16));
      HEX5_3 = (0x0 | ((hex54_val & (0x7F << 8)) << 8) | ((hex54_val & (0x7F)) << 8) | ((hex20_val & (0x7F << 16)) >> 16));
    }
    if (press & 0x8)	// KEY3
    {
	  GPIO1_SWPORTA_DR = ~(GPIO1_SWPORTA_DR & MASK_GPIO53_LED);
    }

  }
  
	// On nettoie l'interruption dans le registre de interruptions pour le processeur
	ICCEOIR = interrupt_ID;
	return;
}

// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
{
    while(1);
}

/*
 * Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void)
{
	uint32_t stack, mode;
	stack = A9_ONCHIP_END - 7;		// top of A9 onchip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = INT_DISABLE | IRQ_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));

	/* go back to SVC mode before executing subroutine return! */
	mode = INT_DISABLE | SVC_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
}

/*
 * Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void)
{
	uint32_t status = SVC_MODE | INT_ENABLE;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}
