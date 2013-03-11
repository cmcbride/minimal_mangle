CC=gcc

INCLUDE_DIRS= -I..

CFLAGS= -O3 -std=c99 -pedantic -Wall -Winline $(INCLUDE_DIRS)
CLINK= -lm

# CFLAGS= -g -O0 -Wall -I./lib -lm

default: mypolyid pix_polycount ply_area trim_by_ply

mypolyid: mypolyid.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

pix_polycount: pix_polycount.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

ply_area: ply_area.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

trim_by_ply: trim_by_ply.c
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

indent:
	gnuindent *.c

clean:
	rm -f *.o mypolyid pix_polycount ply_area trim_by_ply

bak-clean:
	rm -f *.bak *~

