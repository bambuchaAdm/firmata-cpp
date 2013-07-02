CFLAGS = -g -O -ggdb -fPIC
LIBS = -lpthread
CC = g++

OBJ = Connection.o ADKConnection.o FirmataDataReceiver.o Firmata.o Arduino.o ArduinoJS.o test.o

all: main

clean:
	rm -f *.o test

.c.o:
	$(CC) -c $(INCLUDES) $(CFLAGS) $<

main: $(OBJ)
	$(CC) $(OBJ) $(LIBS) -o test
