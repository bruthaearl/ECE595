BUILDROOT_HOME=/home/jake/school/introIOT/project/build/buildroot/

CC=$(BUILDROOT_HOME)/output/host/bin/arm-linux-gcc
CFLAGS=--sysroot=$(BUILDROOT_HOME)/output/staging
INCLUDES=
LFLAGS=-L/usr/lib/x86_64-linux-gnu
LIBS=-lcurl -lpthread 

SRC=main.c enums_structs_etc.h function_decs.h getTemp.c initialize.c makeDecision.c readSetpoints.c publish.c cJSON.c cJSON.h
OBJ=$(SRC:.c=.o)
MAIN=hotbox

RM=rm -rf

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MAIN): $(OBJ)
	$(CC) $(CCFLAGS) $(INCLUDES) -o $(MAIN) $(OBJ) $(LFLAGS) $(LIBS)

all: $(MAIN)

clean:
	$(RM) $(MAIN) *.o *~
