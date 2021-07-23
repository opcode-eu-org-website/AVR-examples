
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../2-UART/serialPrintf-lib4avr.c"

main() {
	serialInitOutput("adc");
	
	// pętla główna
	while(1) {
		// kanał numer 2, napięcie referencyjne z AVcc
		ADMUX = 2 | (1 << REFS0);
		
		// włączamy, rozpoczynamy konwersję, czyścimy flagę gotowości,
		// ACD pracuje z zegarem F_CPU/128
		ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADIF) | 0x07;
		
		// czekamy na gotowość danych
		// zamiast czekać możemy zlecić generowanie przerwania
		// gdy konwersja zakończona i tam obsłużyć odbiór wyniku
		while(!(ADCSRA & (1<<ADIF)));
		
		// odczyt danych
		int val = ADCL | (ADCH << 8);
		
		serialPrintf(16, "%d\n\r", val*5);
		
		_delay_ms(1000);
	}
}
