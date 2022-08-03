#include "main.h"

ISR(TIMER1_COMPA_vect) {
}

ISR(USART_TXC_vect) {
	static uint8_t old_symbol;
}



int main(void) {

  	OCR1A = _F_CPU / TIMERFRQ - 1;
	TCCR1B = (1 << CS10) | (1 << WGM12);
 	TIMSK = TIMSK | (1 << OCIE1A);

	UBRRH = (uint8_t) (UART_UBRR >> 8);
	UBRRL = (uint8_t) UART_UBRR;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	while(1);
}
