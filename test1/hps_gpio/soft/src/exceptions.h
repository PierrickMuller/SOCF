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
 * Ver    Date        Engineer      Comments
 * 0.0    16.02.2018  SMS           Initial version.
 *
*****************************************************************************************/
#include <stdint.h>

// Define the IRQ exception handler
void __attribute__ ((interrupt)) __cs3_isr_irq(void);

// Define the remaining exception handlers
void __attribute__ ((interrupt)) __cs3_reset (void);

void __attribute__ ((interrupt)) __cs3_isr_undef (void);

void __attribute__ ((interrupt)) __cs3_isr_swi (void);

void __attribute__ ((interrupt)) __cs3_isr_pabort (void);

void __attribute__ ((interrupt)) __cs3_isr_dabort (void);

void __attribute__ ((interrupt)) __cs3_isr_fiq (void);
/*
 * Initialize the banked stack pointer register for IRQ mode
*/
void set_A9_IRQ_stack(void);

/*
 * Turn on interrupts in the ARM processor
*/
void enable_A9_interrupts(void);
