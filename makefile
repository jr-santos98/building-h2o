CC = gcc
CFLAGS = -g -pthread
PROGRAMS = building-h2o vg-building-h2o
ORIGEN = building-h2o-origen
DFLAGS = -g
DRAW = draw-h2o

all: $(PROGRAMS)

origen: $(ORIGEN)

draw:
	gcc $(DFLAGS) $(DRAW).c -o $(DRAW)

clean:
	rm -f *~ $(PROGRAMS) $(ORIGEN) $(DRAW) 
