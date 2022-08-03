#ifndef __MAIN_H
#define __MAIN_H

#include <file.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include "avr/eeprom.h"

#define _F_CPU				11059200UL
#define	TIMERFRQ			1000UL
#define BAUD                9600
#define UART_UBRR           _F_CPU / 16 / BAUD-1

/* 
 * Set BIT in ADDRESS 
 */
#define SETBIT(ADDRESS,BIT) ((ADDRESS) |= (1<<(BIT)))

/* 
 * Clear BIT in ADDRESS 
 */
#define CLEARBIT(ADDRESS,BIT)  ((ADDRESS) &= ~(1<<(BIT)))

/* 
 * Test BIT in ADDRESS 
 */
#define TESTBIT(ADDRESS,BIT)  ((ADDRESS) & (1<<(BIT)))


#endif
