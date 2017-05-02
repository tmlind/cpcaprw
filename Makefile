all: clean
	$(CROSS_COMPILE)gcc -Wall -static -o cpcaprw cpcaprw.c

clean:
	rm -f cpcaprw *~
