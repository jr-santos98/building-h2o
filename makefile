CC = gcc
CFLAGS = -g -pthread
PROGRAMS = building-h2o vg-building-h2o new-codes
CODE2 = building-h2o vg-building-h2o
CODE3 = new-codes
ORIGEN = building-h2o-origen
DFLAGS = -g
DRAW = draw-h2o

all: $(PROGRAMS)

code-2: $(CODE2)

code-3: $(CODE3)

origen: $(ORIGEN)

draw:
	gcc $(DFLAGS) $(DRAW).c -o $(DRAW)

clean:
	rm -f *~ $(PROGRAMS) $(ORIGEN) $(DRAW)
