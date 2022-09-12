#include "main.h"

uint8_t message[] = "test message\r\n";
uint8_t cnt = 0;
uint8_t rxBuffer[RX_DATA_BUFFER_LENGTH];
uint16_t rxBufferIndex = 0;
uint16_t rxTimeOut = 0;

APP_STATUS appStatus = LISTENING;

ISR(TIMER1_COMPA_vect) {
	if(rxTimeOut > 0){
		rxTimeOut--; 
		if(!rxTimeOut){
			appStatus = IS_RECEIVED;
			CLEARBIT(PORTD, PIND2);
		}
	}
}

// ISR(USART_TXC_vect) {
// 	if(cnt > 80 || message[cnt] == '\n') {
// 		appStatus = LISTENING;
// 		cnt = 0;
// //		SETBIT(UCSRB, RXCIE);
// 	} else {
// 		UDR=message[cnt];
// 		cnt++;
// 	}
// }

// ISR(USART_UDRE_vect){
// 	uint8_t symbol = message[cnt];
// 	cnt++;
// 	if(cnt > 80 || symbol == '\n') {	SETBIT(PORTD, PIND2);	

// 		cnt = 0;
// 	}
// 	UDR=symbol;
// }

ISR(USART_RXC_vect) {
	uint8_t symbol = UDR;
	SETBIT(PORTD, PIND2);	
	
	if(appStatus == LISTENING) {
		appStatus = RECEIVING;
		rxBufferIndex = 0;
	}

	if(appStatus == RECEIVING && rxBufferIndex < RX_DATA_BUFFER_LENGTH) {
		rxBuffer[rxBufferIndex] = symbol;
		rxBufferIndex++;
		rxTimeOut = RX_DATA_TIMEOUT;
	}
}

int writeSerial(char* str)
{
	uint16_t i;
	uint16_t lenth = strlen(str)+1;
	for(i = 0; i < lenth; i++)
	{
		while(!(UCSRA & (1<<UDRE))){}; // wait ready of port
		UDR = str[i];
	}

	return 0;
}

int main(void) {
  	OCR1A = F_CPU / TIMERFRQ - 1;
	TCCR1B = (1 << CS10) | (1 << WGM12);
 	TIMSK = TIMSK | (1 << OCIE1A);

	UBRRH = (uint8_t) (UART_UBRR >> 8);
	UBRRL = (uint8_t) UART_UBRR;
	/* Enable receiver and transmitter */
	UCSRB = (1 << TXEN) | //(1 << TXCIE) | 
	(1 << RXEN) |  (1 << RXCIE); 
	;
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1);
	DDRD = (1 << DDD1) | (1 << DDD2) | (1 << DDD3); //PORTD as output

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); 

	sei();
		
	while(1){
		if(appStatus == IS_RECEIVED) {
			appStatus = TRANSMITING;
			
			writeSerial(message);
			appStatus = LISTENING;
//			appStatus = LISTENING;
			// cnt = 1;
			// UDR = message[0];
		}
	};
	return 0;
}
