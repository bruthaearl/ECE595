CC=gcc
CCFLAGS=
INCLUDES=
LFLAGS=-L/usr/lib/x86_64-linux-gnu
LIBS=-lcurl -lpthread -lcjson

SRC=main.c enums_structs_etc.h function_decs.h getTemp.c initialize.c makeDecision.c readSetpoints.c
OBJ=$(SRC:.c=.o)
MAIN=test

RM=rm -rf

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MAIN): $(OBJ)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJ) $(LFLAGS) $(LIBS)

all: $(MAIN)

clean:
	$(RM) $(MAIN) *.o *~
