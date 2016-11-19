SRCDIR = src/
LIBS = -lm
RM = rm -f

all: assignment clean

debug: assignment-debug clean

tests: assignment-test clean

objects:
	@cd $(SRCDIR); $(CC) -c assignment.c

objects-debug:
	@cd $(SRCDIR); $(CC) -c assignment.c -Wall -Wshadow -Wstrict-overflow -fno-strict-aliasing -Wformat -Wformat-security -g

objects-test:
	@cd $(SRCDIR); $(CC) -c *.c -DTEST

assignment: objects
	@cd $(SRCDIR); $(CC) -o ../assignment assignment.o $(LIBS)

assignment-debug: objects-debug
	@cd $(SRCDIR); $(CC) -o ../assignment assignment.o $(LIBS)

assignment-test: objects-test
	@cd $(SRCDIR); $(CC) -o ../assignment_test *.o $(LIBS)

clean:
	@cd $(SRCDIR); $(RM) *.o
