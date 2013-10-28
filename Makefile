
CPP=g++ -c -o
ELINK=g++ -o
RM=rm -f

CPPFLAGS=
CPPLIBS=-lm -lmgl

SOURCES=main.cpp
HEADERS=ModelSimulation.hpp
OBJS=main.o
EXEC=ModelSimulation

all: run

clean:
	$(RM) $(OBJS) $(EXEC)

cleandata: clean
	$(RM) System1_* System2_*

run: $(EXEC)
	./$(EXEC) 1 199 1 1
	./$(EXEC) 1 1999 1 1
	./$(EXEC) 1 19999 1 1
	./$(EXEC) 1 199999 1 1

$(EXEC): $(OBJS)
	$(ELINK) $@ $? $(CPPLIBS)

$(SOURCES): $(HEADERS)

.cpp.o:
	$(CPP) $@ $? $(CPPFLAGS)
