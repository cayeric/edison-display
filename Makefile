CC = gcc
CFLAGS = -lmraa
OBJ = lcdpbm ks0108fb.o pbmimage.o 
DEPS = ks0108fb.h pbmimage.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:	lcdpbm

lcdpbm:	$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) lcdpbm
	$(RM) *.o

install: lcdpbm
	cp lcdpbm /usr/local/bin

	