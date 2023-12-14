FOPTION=posix
RUNOPTIONS=WarAndPeace.txt 2
CC=gcc
CFLAGS= -g -I.
LIBS =-l pthread
DEPS = 
OBJ = $(FOPTION).o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

$(FOPTION): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm *.o $(FOPTION)

run: $(FOPTION)
	./$(FOPTION) $(RUNOPTIONS)

vrun: $(FOPTION)
	valgrind ./$(FOPTION) $(RUNOPTIONS)
