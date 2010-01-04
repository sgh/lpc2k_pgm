CFLAGS = -Wall -O2
CC = gcc

all: lpc2k_pgm

OBJS = lpc2k_pgm.o gui.o settings.o download.o uuencode.o ihex.o serial.o chipinfo.o boot_2xxx.o boot_23xx.o

lpc2k_pgm: $(OBJS)
	$(CC) -o lpc2k_pgm $(OBJS) `gtk-config --libs`
	strip lpc2k_pgm

chipinfo.o: boot_2xxx.h boot_23xx.h

static: $(OBJS) mkstaticlist
	$(CC) -o lpc2k_pgm $(OBJS) `./mkstaticlist`
	strip lpc2k_pgm

cksum_test: cksum_test.c uuencode.o cksum_test.o
	$(CC) $(CFLAGS) -o cksum_test cksum_test.o uuencode.o

boot_2xxx.c boot_2xxx.h: boot_2xxx.armasm mkbootc
	arm-elf-as -o boot_2xxx.armobj boot_2xxx.armasm
	arm-elf-objdump -d boot_2xxx.armobj | ./mkbootc boot_2xxx

boot_23xx.c boot_23xx.h: boot_23xx.armasm mkbootc
	arm-elf-as -o boot_23xx.armobj boot_23xx.armasm
	arm-elf-objdump -d boot_23xx.armobj | ./mkbootc boot_23xx

clean:
	rm -f lpc2k_pgm cksum_test *.o core core.* *.armobj

gui.o: gui.c
	$(CC) $(CFLAGS) `gtk-config --cflags` -c gui.c


