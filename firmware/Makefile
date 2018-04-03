NAME=nintender

SOURCES=$(NAME).c usb_gamepad.c
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

CROSS=avr
CC=$(CROSS)-gcc
OBJCOPY=$(CROSS)-objcopy

MMCU=atmega32u4

CFLAGS=-std=gnu99 -mmcu=$(MMCU) -DF_CPU=16000000UL -Os -I.
CFLAGS+=-funsigned-char -funsigned-bitfields -ffunction-sections -fpack-struct -fshort-enums
#CFLAGS+=-Wall -Wextra -Werror -pedantic

all: $(NAME).hex

flash: $(NAME).hex
	teensy_loader_cli -mmcu=$(MMCU) -w $^

clean:
	rm -rf $(NAME).hex $(NAME).elf *.o

$(NAME).hex: $(NAME).elf
	$(CROSS)-objcopy -j .text -O ihex $^ $@

$(NAME).elf: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

.PHONY: all clean flash
