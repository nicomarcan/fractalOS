
include Makefile.inc

FRACTAL=fractal.bin
SOURCES=$(wildcard *.c)
SOURCES_ASM=asmlib.asm
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm
LOADEROBJECT=$(LOADERSRC:.asm=.o)



all: $(FRACTAL)

$(FRACTAL): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T fractal.ld -o $(FRACTAL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)
	
clean:
	rm -rf asm/*.o *.o *.bin

.PHONY: all clean
