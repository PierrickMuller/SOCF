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
  int hex30_val,hex45_val,leds_val,press;

  press = KEYS_INTERRUPT_REGISTER;     // On récupère le bouton qui à causer l'interruption
  KEYS_INTERRUPT_REGISTER = press;     // On nettoie l'interruption dans le registre des interruptions pour les KEYS

  // Si busy capturé
  if(interrupt_ID == 72)
  {
	 
      /*leds_val = LEDS;
      LEDS = 0x0;
      LEDS = (leds_val >> 1) | ((leds_val & (0x1))<<9);
      hex30_val = HEX3_0;
      hex45_val = HEX4_5;
      HEX3_0 = ~0x0;
      HEX4_5 = ~0x0;
      HEX3_0 = (0x0 | ((hex45_val & (0x7F)) << 24) | ((hex30_val & (0x7F << 24)) >> 8) | ((hex30_val & (0x7F << 16)) >> 8) | ((hex30_val & (0x7F << 8))>>8));
      HEX4_5 = (0x0 | ((hex30_val & (0x7F)) << 8) | ((hex45_val & (0x7F << 8))>>8));**/
     /*
      leds_val = LEDS;
      LEDS = (leds_val << 1) | ((leds_val & (0x1 << 9))>>9);
      hex30_val = HEX3_0;
      hex45_val = HEX4_5;
      HEX3_0 = ~0x0;
      HEX4_5 = ~0x0;
      HEX3_0 = (0x0 | ((hex30_val & (0x7F << 16)) << 8) | ((hex30_val & (0x7F << 8)) << 8) | ((hex30_val & (0x7F)) << 8) | ((hex45_val & (0x7F << 8))>> 8));
      HEX4_5 = (0x0 | ((hex45_val & (0x7F)) << 8) | ((hex30_val & (0x7F << 24))>>24));
      */
     
     

  }
    // Si l'interruption à été causer par un bouton, on ne fait rien, ça à été ajouter à qsys mais ça devrait être enlever, ça sert à rien, mais j'ai pas le temps de tout recompiler
  else if (interrupt_ID == 73)
  {}
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
