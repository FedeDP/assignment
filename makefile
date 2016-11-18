SRCDIR = ./
LIBS = -lm

all: objects assignment clean

debug: objects-debug assignment-debug clean

objects:
	@cd $(SRCDIR); $(CC) -c *.c $(CFLAGS)

objects-debug:
	@cd $(SRCDIR); $(CC) -c *.c -Wall $(CFLAGS) -Wshadow -Wstrict-overflow -fno-strict-aliasing -Wformat -Wformat-security -g

assignment: objects
	@cd $(SRCDIR); $(CC) -o assignment *.o $(LIBS)

assignment-debug: objects-debug
	@cd $(SRCDIR); $(CC) -o assignment *.o $(LIBS)

clean:
	@cd $(SRCDIR); $(RM) *.o

