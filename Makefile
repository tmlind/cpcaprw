all: clean
	$(CROSS_COMPILE)clang -DANDROID_PLATFORM=android-21 -Wall -o cpcaprw cpcaprw.c

clean:
	rm -f cpcaprw *~
