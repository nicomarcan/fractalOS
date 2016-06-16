include ../Makefile.inc

MODULE=0000-fractal.bin
SOURCES=$(wildcard *.c)

all: $(MODULE)

$(MODULE): $(SOURCES)
	$(GCC) $(GCCFLAGS) -T fractal.ld $(SOURCES) -o ../$(MODULE)

clean:
	rm -rf *.o

.PHONY: all clean
