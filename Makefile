all:
	gcc -o mztx06a -l rt src/mztx06a.c src/bcm2835.c

init:
	gcc -o lcdinit -l rt src/lcdinit.c src/bcm2835.c

new:
	gcc -o lcdpi -l rt src/lcdpi.c src/bcm2835.c
