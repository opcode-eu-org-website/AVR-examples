
// określamy prędkość naszego procesora ... potrzebne m.in. dla _delay_ms()
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

main() {
	// ustawiamy kierunek pinów 3, 4 i 5 portu B (PB3, PB4, PB5) jako wyjściowy
	// numery pinów określane są maską bitową 0x38 = 0b00111000
	DDRB  = 0x38;
	
	// pętla główna
	while(1){
		// ustawiamy stan portu B na maskę bitową 0x38
		// co powoduje ustawienie stanu wysokiego na wszystkich pinach
		// zdefiniowanych wcześniej jako wyjściowe
		PORTB = 0x38;
		// czekamy chwilę
		_delay_ms(200);
		// ustawiamy stan portu B na maskę bitową 0x00
		// co powoduje ustawienie stanu niskiego na wszystkich pinach
		PORTB = 0x00;
		// czekamy chwilę
		_delay_ms(700);
	}
}
