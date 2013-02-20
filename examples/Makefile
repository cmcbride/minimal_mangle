CC=gcc
GSL_INC= -I/opt/local/include
GSL_LINK= -L/opt/local/lib -lgsl -lgslcblas

INCLUDE_DIRS= -I./lib 

CFLAGS= -O3 -std=c99 -pedantic -Wall -Winline -I./lib -I.. $(GSL_INC) 
CLINK= -lm $(GSL_LINK)

# CFLAGS= -g -O0 -Wall -I./lib -lm

default: mypolyid

mypolyid: mypolyid.c 
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

trim_to_ply: trim_to_ply.c 
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

pix_polycount: pix_polycount.c 
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

indent:
	gnuindent *.c

clean:
	rm -f *.o mypolyid trim_to_ply pix_polycount

bak-clean:
	rm -f *.bak *~ 

