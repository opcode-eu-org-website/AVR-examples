
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "../2-UART/serialPrintf-lib4avr.c"

#define I2C_ADDRESS 0x22

uint8_t twiOutputInit(uint8_t* data) {
	static uint8_t i = 0;
	data[0] = 13;
	data[1] = ++i;
	serialPrintf(64, "sending: 0x%x 0x%x 0x%x\n\r", data[0], data[1], data[2]);
	return 3;
}

uint8_t twiCheckInput(uint8_t* data, uint8_t i) {
	if (i>=2) {
		serialPrintf(64, "received: 0x%x 0x%x 0x%x\n\r", data[0], data[1], data[2]);
		return 1;
	}
	return 0;
}

// definicja procedury obsługi przerwania (ISR) dla przerwania związanego
// z stanem pracy układu obsługi magistrali I2C (TWI_vect)
ISR(TWI_vect) {
	// blokujemy przerwania
	cli();
	
	uint8_t status = TWSR & 0xf8;
	
	uint8_t ack = 1;
	static uint8_t di, diMax, data[8];
	
	serialPrintf(64, "status = 0x%x di = %d\n\r", status, di);
	
	switch (status) {
		// RECIVER (i2c write mode)
		case 0x68:
		case 0x60: // otrzymano adres, zwrócono ACK
		           //  ==>> inicjalizacja odbioru danych
			di = 0;
			break;
		case 0x80: // otrzymano dane, zwrócono ACK
		           //  ==>> odbiór danych (kolejne bajty)
			data[di] = TWDR;
			
			if (twiCheckInput(data, di)) {
				ack = 0; // koniec odbioru ... na kolejne dane zwracamy NOT ACK
			} else {
				++di;
			}
			break;
		case 0x88: // otrzymano dane, zwrócono NOT ACK
		           //  ==>> (błąd protokołu)  ==>> oczekiwanie na adres
		case 0xA0: // otrzymano STOP lub powtórzony START
		           //  ==>> oczekiwanie na adres
			break;
		
		// TRANSMITER (i2c read mode)
		case 0xB0:
		case 0xA8: // otrzymano adres, zwrócono ACK
		           //  ==>> wysyłanie danych (pierwszy bajt)
			di = 0;
			diMax = twiOutputInit(data);
		case 0xB8: // przesłano bajt z TWDR, otrzymano ACK
		           //  ==>> wysyłanie danych (kolejne bajty)
			if (di < diMax) {
				TWDR = data[di++];
			} else {
				ack = 0;
			}
			break;
		case 0xC0: // przesłano bajt z TWDR, otrzymano NOT ACK
		           //  ==>> (błąd protokołu)  ==>>  oczekiwanie na adres
			break;
		case 0xC8: // przesłano ostatni bajt z TWDR, otrzymano ACK
		           //  ==>> oczekiwanie na adres
			break;
		
		// BUS ERROR
		case 0x00:
			TWCR = (1<<TWSTO)|(1<<TWINT);
			break;
	}
	
	// clear interrupt flag
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWINT) | (ack<<TWEA);
	
	// odblokowujemy przerwania
	sei();
}

main() {
	serialInitOutput("i2c slave");
	
	// konfiguracja I2C jako slave
	// adres, nie reagujemy na generall call
	TWAR = I2C_ADDRESS << 1;
	
	// włączony | włączone generowanie przerwań | wyczyszczona flaga przerwań
	//          | włączone generowanie ACK
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT) | (1 << TWEA);
	
	// włączenie obsługi przerwań
	sei();
	
	// pętla główna
	while(1) {
	}
}
