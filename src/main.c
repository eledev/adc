#include "main.h"

uint8_t message[] = "test message\n";
uint8_t cnt = 0;
ISR(TIMER1_COMPA_vect) {
	// if(TESTBIT(PORTD, PIND0)) {
	// 	CLEARBIT(PORTD, PIND0);
	// } else {
	// 	SETBIT(PORTD, PIND0);
	// }
}

ISR(USART_TXC_vect) {
	cnt++;
	if(cnt>12) {
		cnt = 0;
	}
	UDR=message[cnt];
}

// ISR(USART_RXC_vect) {
// }

int main(void) {
  	OCR1A = F_CPU / TIMERFRQ - 1;
	TCCR1B = (1 << CS10) | (1 << WGM12);
 	TIMSK = TIMSK | (1 << OCIE1A);

	UBRRH = (uint8_t) (UART_UBRR >> 8);
	UBRRL = (uint8_t) UART_UBRR;
	/* Enable receiver and transmitter */
	UCSRB = (1 << TXEN) | 
	//(1 << RXEN) | 
	(1 << TXCIE) //| 
	//(1 << RXCIE) 
	;
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1);
//	DDRD = 0xFF; //PORTD as output

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); 

	sei();
	UDR=message[cnt];	
	while(1){
	};
	return 0;
}
