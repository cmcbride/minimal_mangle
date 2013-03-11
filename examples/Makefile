CC=gcc

INCLUDE_DIRS= -I..

CFLAGS= -O3 -std=c99 -pedantic -Wall -Winline $(INCLUDE_DIRS)
CLINK= -lm

# CFLAGS= -g -O0 -Wall -I./lib -lm

default: mply_area mply_pix_polycount mply_polyid mply_trim

mply_area: mply_area.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

mply_pix_polycount: mply_pix_polycount.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

mply_polyid: mply_polyid.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

mply_trim: mply_trim.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

indent:
	gnuindent *.c

clean:
	rm -f *.o

bu-clean: clean
	rm -f *.bak *~

real-clean: clean
	rm -f mply_area mply_pix_polycount mply_polyid mply_trim

