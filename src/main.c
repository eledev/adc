#include "main.h"

const uint8_t CMD_HELP[] PROGMEM = "cmd\r";
const uint8_t CMD_CH00[] PROGMEM = "cmd_ch00\r";
const uint8_t CMD_CH01[] PROGMEM = "cmd_ch01\r";

uint8_t message[] = "test message %d\r";
uint8_t cnt = 0;
uint8_t rxBuffer[RX_DATA_BUFFER_LENGTH];
uint16_t rxBufferIndex = 0;
uint16_t rxTimeOut = 0;

APP_STATUS appStatus = LISTENING;


static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static int uart_putchar(char c, FILE *stream) {
	if (c == '\n')
    uart_putchar('\r', stream);
  	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	return 0;
}




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
		rxBuffer[rxBufferIndex] = 0;
		rxTimeOut = RX_DATA_TIMEOUT;
	}
}

// int writeSerial(char* str)
// {
// 	uint16_t i;
// 	uint16_t lenth = strlen(str)+1;
// 	for(i = 0; i < lenth; i++)
// 	{
// 		loop_until_bit_is_set(UCSRA, UDRE);
// 		UDR = str[i];
// 	}

// 	return 0;
// }

int main(void) {
  	OCR1A = F_CPU / TIMERFRQ - 1;
	TCCR1B = (1 << CS10) | (1 << WGM12);
 	TIMSK = TIMSK | (1 << OCIE1A);

	UBRRH = UBRRH_VALUE;
   	UBRRL = UBRRL_VALUE;
  
	/* Enable receiver and transmitter */
	UCSRB = (1 << TXEN) | //(1 << TXCIE) | 
	(1 << RXEN) |  (1 << RXCIE); 
	;
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1 << URSEL) | (1 << USBS) | (1 << UCSZ0) | (1 << UCSZ1);
	DDRD = (1 << DDD1) | (1 << DDD2) | (1 << DDD3); //PORTD as output

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); 

 	stdout = &mystdout;


	sei();
		
	while(1){
		if(appStatus == IS_RECEIVED) {
			appStatus = TRANSMITING;
			if(!strcmp_P(rxBuffer, CMD_HELP)){
				printf(message, 12);
			}

			appStatus = LISTENING;
		}
	};
	return 0;
}
