HEADERS = src/keylogger.h
OBJECTS = src/keylogger.o

default: src/keylogger

%.o: %.c $(HEADERS)
	gcc -c $< -o $@

program: $(OBJECTS)
	gcc $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS)
	-rm -f src/keylogger