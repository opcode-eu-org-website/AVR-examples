
#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/twi.h>

#include "../2-UART/serialPrintf-lib4avr.c"

#define I2C_SPEED 400000

uint8_t twiStart() {
	// wyślij sekwencję START
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	
	// poczekaj na koniec operacji
	while ((TWCR & (1<<TWINT)) == 0);
	
	// zwróć status operacji
	return ((TWSR & 0xF8) == 0x08);
}

void twiStop() {
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void twiWrite(uint8_t byte) {
	TWDR = byte;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (! (TWCR & (1<<TWINT)) );
}

uint8_t twiRead(uint8_t ack) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (ack<<TWEA);
	while (! (TWCR & (1<<TWINT)) );
	return TWDR;
}

uint8_t twiSend(uint8_t addr, uint8_t *data, uint8_t dataLen) {
	// inicjujemy szynę
	if (! twiStart() )
		 return 0;
	
	// wysyłamy adres w trybie zapisu
	twiWrite(addr<<1);
	if ((TWSR & 0xF8) != 0x18)
		 return 0;
	
	// wyłamy kolejne bajty danych
	uint8_t i;
	for (i=0; i<dataLen; ++i) {
		twiWrite(data[i]);
		if ((TWSR & 0xF8) != 0x28)
			return i;
	}
	
	// kończymy transmisję
	twiStop();
	
	return i;
}

uint8_t twiReceive(uint8_t addr, uint8_t *data, uint8_t dataLen) {
	// inicjujemy szynę
	if (! twiStart() )
		 return 0;
	
	// wysyłamy adres w trybie odczytu
	twiWrite(addr<<1 | 0x01);
	if ((TWSR & 0xF8) != 0x40)
		 return 0;
	
	// odbieramy kolejne bajty danych
	uint8_t i;
	for (i=0; i<dataLen; ++i) {
		// obieramy dane z ack=1 dopóki oczekujemy kolejnych
		uint8_t ack = 1;
		if (i+1 >= dataLen)
			ack = 0;
		data[i] = twiRead(ack);
	}
	
	// kończymy transmisję
	twiStop();
	
	return 1;
}

main() {
	serialInitOutput("i2c master");
	
	// konfiguracja I2C jako master
	// ustawienie zegara na 400kHz
	TWBR = (F_CPU/I2C_SPEED - 16) >> 1; // 12
	// włączony
	TWCR = (1<<TWEN);
	
	// tablica z danymi dla i2c
	uint8_t d[16];
	d[0] = 15; d[1] = 17; d[2] = 0;
	
	// pętla główna
	while(1) {
		int s = twiSend(0x22, d, 3);
		serialPrintf(64, "send: 0x%x 0x%x 0x%x %d\r\n", d[0], d[1], d[2]++, s);
		_delay_ms(1000);
		
		twiReceive(0x22, d+4, 5);
		serialPrintf(64, "receive: 0x%x 0x%x 0x%x\r\n", d[4], d[5], d[6]);
		_delay_ms(1000);
	}
}
