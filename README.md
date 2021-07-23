AVR-examples
============

Repozytorium zawiera przykładowe kody demonstrujące użycie podstawowych peryferiów w mikrokntolerze typu AVR.


## Mikrokontrolery AVR

Jest to rodzina 8-bitowych mikrokontrolerów RISC, które mogą być programowane z wykorzystaniem języka C z pomocą odpowiedniej wersji gcc i biblioteki [avr-libc](https://www.nongnu.org/avr-libc/).

Prezentowane kody dedykowane są dla mikrokontrolera ATmega328, będą działać poprawnie także na wielu innych układach, jednak w niektórych wypadkach konieczna może być zmiana nazw niektórych z rejestrów.
Na przykład dla ATmega8:

	#define USART_RX_vect USART_RXC_vect
	#define UDR0          UDR
	#define UBRR0L        UBRRL
	#define UBRR0H        UBRRH
	#define UCSR0B        UCSRB
	#define UCSR0A        UCSRA
	#define TXEN0         TXEN
	#define RXEN0         RXEN
	#define RXCIE0        RXCIE
	#define UDRE0         UDRE

	
## Kompilacja i programowanie mikrontrolera

Kompilację kodów dla ATmega328 oraz wgranie ich do mikrokontrolera można wykonać przy pomocy następujących poleceń:

	# kompilacja
	avr-gcc -mmcu=atmega328p -Os -o avrdemo.o $INPUT_FILE

	# konwersja do formatu hex obsługiwanego przez programatory
	avr-objcopy -O ihex avrdemo.o avrdemo.hex

	# zapis (i weryfikacja) pamięci flash (programator ISP na usb zgodny z usbasp):
	sudo avrdude -c usbasp -p atmega328p -U flash:w:avrdemo.hex

	# alternatywnie - zapis (i weryfikacja) pamięci flash
	# (wgrany w programowanym układzie bootloader z komunikacją UART zgodny z arduino):
	sudo avrdude -q -V -p atmega328p -C /usr/share/arduino/hardware/tools/avrdude.conf -D \
	             -c arduino -b 57600 -P /dev/ttyUSB0 -U flash:w:avrdemo.hex:i

## Przykłady

### obsługa GPIO

* [sterowanie wyjściem](1-GPIO/output.c)
* [odczy stanu wejścia](1-GPIO/input.c)

### obsługa portu szeregowego

* [z aktywnym czekaniem](2-UART/podstawowa.c)
* [z obsługą przewań](2-UART/przerwania.c)
* [wysyłanie danych via UART w stylu printf](2-UART/serialPrintf-lib4avr.c) – biblioteczka używana w dalszych przykładach

### obsługa przetwornika analogo-cyfrowego

* [odczyt wartości z ADC](3-ADC/adc.c)

### obsługa I2C

* [master](4-I2C/master.c) – w tym trybie mikrokontroler zarządza magistralą - generuje sygnał zegara, wybiera układ z którym chce się komunikować oraz tryb tej komunikacji (zapis czy odczyt)
* [slave](4-I2C/slave.c) – w tym trybie mikrokontroler czeka na zainicjowanie komunikacji przez mastera i postępuje zgodnie z otrzymanymi od niego instrukcjami
