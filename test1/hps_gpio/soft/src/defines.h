/*****************************************************************************************
 * HEIG-VD
 * Haute Ecole d'Ingenerie et de Gestion du Canton de Vaud
 * School of Business and Engineering in Canton de Vaud
 *****************************************************************************************
 * REDS Institute
 * Reconfigurable Embedded Digital Systems
 *****************************************************************************************
 *
 * File                 : defines.h
 * Author               : Sébastien Masle
 * Date                 : 16.02.2018
 *
 * Context              : SOCF class
 *
 *****************************************************************************************
 * Brief: some definitions
 *
 *****************************************************************************************
 * Modifications :
 * Ver    Date        Engineer      Comments
 * 0.0    16.02.2018  SMS           Initial version.
 * 0.1    15.05.2020  PMR           Modification pour test 01 SOCF
 *
*****************************************************************************************/


typedef volatile unsigned short vushort;
typedef unsigned short ushort;
typedef volatile unsigned int vuint;


#define		EDGE_TRIGGERED			0x1
#define		LEVEL_SENSITIVE			0x0
#define		CPU0					0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE					0x1

#define		USER_MODE				0b10000
#define		FIQ_MODE				0b10001
#define		IRQ_MODE				0b10010
#define		SVC_MODE				0b10011
#define		ABORT_MODE				0b10111
#define		UNDEF_MODE				0b11011
#define		SYS_MODE				0b11111

#define		INT_ENABLE				0b01000000
#define		INT_DISABLE				0b11000000

#define BASE_ADRESSE                    0xFF200000
#define GIC_DISTRIBUTOR_BASE_ADRESSE    0xFFFED000
#define GIC_CPU_INTERFACE_BASE_ADRESSE  0xFFFEC100

// Basé sur l'adresse map fournie pour le test
#define LEDS            *(vuint *) (BASE_ADRESSE + 0x80)
#define HEX2_0          *(vuint *) (BASE_ADRESSE + 0xB0)
#define HEX5_3          *(vuint *) (BASE_ADRESSE + 0xC0)
#define SWITCHS         *(vuint *) (BASE_ADRESSE + 0x30)
#define KEYS            *(vuint *) (BASE_ADRESSE + 0x20)
#define KEYS_INTERRUPT_ENABLE  *(vuint *) (BASE_ADRESSE + 0x20 + 0x8)
#define KEYS_INTERRUPT_REGISTER *(vuint *) (BASE_ADRESSE + 0x20 + 0xC)
#define CONST_REGISTER  *(vuint *) (BASE_ADRESSE + 0x00)
#define TEST_REGISTER   *(vuint *) (BASE_ADRESSE + 0x10)



#define ICCICR          *(vuint *) (GIC_CPU_INTERFACE_BASE_ADRESSE + 0x0)
#define ICCPMR          *(vuint *) (GIC_CPU_INTERFACE_BASE_ADRESSE + 0x4)
#define ICCIAR          *(vuint *) (GIC_CPU_INTERFACE_BASE_ADRESSE + 0xc)
#define ICCEOIR         *(vuint *) (GIC_CPU_INTERFACE_BASE_ADRESSE + 0x10)

#define ICDDCR          *(vuint *) (GIC_DISTRIBUTOR_BASE_ADRESSE + 0x0)
#define ICDISER         *(vuint *) (GIC_DISTRIBUTOR_BASE_ADRESSE + 0x100 + 8  )//(floor(72 / 32) * 4)
#define ICDIPTR         *(vuint *) (GIC_DISTRIBUTOR_BASE_ADRESSE + 0x800 + 72 + 0) //(72/4)*4 + (72%4))
#define ICDICFR         *(vuint *) (GIC_DISTRIBUTOR_BASE_ADRESSE + 0xC00 + 16) //(72/16)*4


//Addresse de base pour les gpios 57 à 29
#define GPIO1_BASE_ADRESSE          0xFF709000

//Masques pour les différents élèments (KEY et LED)
#define MASK_GPIO54_KEY             0x1 << 25
#define MASK_GPIO53_LED             0x1 << 24

//Addresse des registres permettant d'écrire, de set la direction des gpios et d'écrire sur la gpio
#define GPIO1_SWPORTA_DR            *(vuint *) (GPIO1_BASE_ADRESSE + 0x0)
#define GPIO1_SWPORTA_DDR           *(vuint *) (GPIO1_BASE_ADRESSE + 0x4)
#define GPIO1_EXT_PORTA             *(vuint *) (GPIO1_BASE_ADRESSE + 0x50)


