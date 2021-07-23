
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

main() {
	// ustawiamy kierunek pinów 3, 4 i 5 portu B (PB3, PB4, PB5) jako wyjściowy
	// numery pinów określane są maską bitową 0x38 = 0b00111000
	DDRB  = 0x38;
	
	// ustawiamy stan portu B na maskę bitową 0x01
	// co (przy uwzględnieniu wcześniej ustawionego kierunku poszczególnych pinów tego portu)
	// powoduje ustawienie załączenie rezystora podciągającego do Vcc na pinie 0 (PB0)
	PORTB = 0x01;
	
	// pętla główna
	while(1){
		// odczytujemy wartość portu B i jeżeli pin 0 (PB0) jest w stanie wysokim to:
		if (PINB & 0x01) {
			// ustawiamy stan portu B na maskę bitową 0x09
			// co powoduje ustawienie stanu wysokiego na PB3 oraz zachowanie podciągania na PB0
			PORTB = 0x39;
			// czekamy chwilę
			_delay_ms(200);
			
			// ustawiamy stan portu B na maskę bitową 0x31
			// co powoduje ustawienie stanu wysokiego na PB5 i PB4
			// oraz zachowanie podciągania na PB0
			PORTB = 0x31;
			// czekamy chwilę
			_delay_ms(700);
		}
	}
}
