TARGET=snakegame
CC=g++
DEBUG=-g
OPT=-O0
WARN=-Wall
CCFLAGS=$(DEBUG) $(OPT) $(WARN)
LD=g++
OBJS= main.o snakegame.o
NCURSES=-lncurses
TINFO=-ltinfo
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(NCURSES) $(TINFO)
 
main.o: main.cpp
	$(CC) -c $(CCFLAGS) $(NCURSES) $(TINFO) main.cpp -o main.o
 
snakegame.o: snakegame.cpp
	$(CC) -c $(CCFLAGS) $(NCURSES) $(TINFO) snakegame.cpp  -o snakegame.o
 
clean:
	@rm -f *.o
