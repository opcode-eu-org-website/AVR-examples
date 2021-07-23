
#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// definicja procedury obsługi przerwania (ISR)
// dla przerwania związanego z odebraniem danych
// z portu szeregowego (USART_RX_vect)
ISR(USART_RX_vect) {
	// blokujemy przerwania
	cli();
	
	// odbiór bajtu
	char d = UDR0;
	
	// echo
	UDR0 = d;
	
	// odblokowujemy przerwania
	sei();
}

main() {
	/// ustawienie prędkości (BAUD), UBRR = F_CPU / (16*BAUD) -1
	UBRR0L = ((((F_CPU >> 4) / BAUD) - 1)     ) & 0xff;
	UBRR0H = ((((F_CPU >> 4) / BAUD) - 1) >> 8) & 0xff;
	
	// włączenie nadajnika i odbiornika UART
	// wraz z generacją przerwań przy odbiorze
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	
	const char* s = "Hello";
	char i;
	for(i=0; i<5; ++i) {
		// czekamy na miejsce w buforze nadawczym
		while(!(UCSR0A & (1<<UDRE0)));
		// zapisujemy znak do bufora nadawczego
		UDR0 = s[i];
	}
	
	// odblokowujemy przerwania
	sei();
	
	// pętla główna
	while(1){
	}
}
