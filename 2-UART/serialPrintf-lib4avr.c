
// mikro biblioteczka "serial-printf-lib4avr.c"
// wykorzystywana w dalszych przykładach

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define BAUD 9600

// funkcja z niezdefiniowaną ilością argumentów
// obudowująca funkcję z rodziny printf
void serialPrintf(int bufSize, char* format, ...) {
	// alokacja buforu o podanej wielkości
	uint8_t* buf = malloc(bufSize);
	int i, len;
	
	// wywołanie funkcji z rodziny printf zapisującej
	// rezultat do podanego bufora z uwzględnieniem
	// jego maksymalnej długości w oparciu o listę
	// argumentów typu va_list
	va_list args;
	va_start (args, format);
	len = vsnprintf (buf, bufSize, format, args);
	
	// wysyłanie danych z bufora
	for(i=0; i<len; ++i) {
		// czekamy na miejsce w buforze nadawczym
		while(!(UCSR0A & (1<<UDRE0)));
		// zapisujemy znak do bufora nadawczego
		UDR0 = buf[i];
	}
	
	// zwolnienie bufora
	free(buf);
}

void serialInitOutput(const char* s) {
	/// ustawienie prędkości (BAUD), UBRR = F_CPU / (16*BAUD) -1
	UBRR0L = ((((F_CPU >> 4) / BAUD) - 1)     ) & 0xff;
	UBRR0H = ((((F_CPU >> 4) / BAUD) - 1) >> 8) & 0xff;
	
	// włączenie nadajnika
	UCSR0B = (1 << TXEN0);
	
	// powitalny napis
	serialPrintf(64, "\r\nstart %s\r\n", s);
}
