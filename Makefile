all: main.hex

main.hex: main.elf
	avr-objcopy  -j .text -j .data -O ihex main.elf main.hex
	avr-size main.elf

main.elf: main.c lcd.c gfx.c
	avr-gcc -mmcu=atmega324a -Wall -Wextra -Os -o $@ $^

clean:
	rm -rf main.elf main.hex
