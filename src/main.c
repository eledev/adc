
#include "main.h"

#include <util/delay.h>

uint8_t message[] = "test message\n";
ISR(TIMER1_COMPA_vect) {
	if(TESTBIT(PORTD, PIND0)) {
		CLEARBIT(PORTD, PIND0);
	} else {
		SETBIT(PORTD, PIND0);
	}
}

ISR(USART_TXC_vect) {
	UDR='H';
}



int main(void) {
  	OCR1A = F_CPU / TIMERFRQ - 1;
	TCCR1B = (1 << CS10) | (1 << WGM12);
 	TIMSK = TIMSK | (1 << OCIE1A);

	UBRRH = (uint8_t) (UART_UBRR >> 8);
	UBRRL = (uint8_t) UART_UBRR;
	/* Enable receiver and transmitter */
	UCSRB = (1 << TXEN) | (1<<TXCIE);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1);
	DDRD = 0xFF; //PORTD as output
	SETBIT(PORTD, PIND0);

	sei();
	while ( !( UCSRA & (1<<UDRE)));
	UDR='H';	
	while(1){
	};
	return 0;
}
