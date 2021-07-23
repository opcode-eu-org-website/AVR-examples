
#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>

main() {
	/// ustawienie prędkości (BAUD), UBRR = F_CPU / (16*BAUD) -1
	UBRR0L = ((((F_CPU >> 4) / BAUD) - 1)     ) & 0xff;
	UBRR0H = ((((F_CPU >> 4) / BAUD) - 1) >> 8) & 0xff;
	
	// włączenie nadajnika i odbiornika UART
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
	// powitalne zamiganie diodą
	DDRB  = 0x38;
	char i;
	for (i=0; i< 3; ++i) {
		PORTB = 0x38;
		_delay_ms(500);
		PORTB = 0x00;
		_delay_ms(500);
	}
	
	// pętla główna
	while(1){
		// czekamy na dane z seriala
		while ( !(UCSR0A & (1<<RXC0)) );
		
		// odbiór bajtu
		char d = UDR0;
		
		// echo - odsyłamy do nadawcy
		UDR0 = d;
		
		// przełączenie diody
		PORTB = (PORTB & (~0x38)) | ((++i & 0x01) << 5);
	}
}
