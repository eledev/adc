#ifndef __MAIN_H
#define __MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define F_CPU				            11059200UL
#define	TIMERFRQ			            1000UL
#define BAUD                            9600
#define UART_UBRR                       (F_CPU / 16UL / BAUD) - 1

#define RX_DATA_BUFFER_LENGTH           80UL
#define RX_DATA_TIMEOUT                 500


typedef enum {
    LISTENING,
    RECEIVING,
    IS_RECEIVED,
    TRANSMITING,
    TRANSMITED,
    READY,
    DATA_SIZE_FAILURE
} APP_STATUS;

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
