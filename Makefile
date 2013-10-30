
CPP=g++ -c -o
ELINK=g++ -o
RM=rm -f

CPPFLAGS=
CPPLIBS=-lm

SOURCES=main.cpp
HEADERS=ModelSimulation.hpp
OBJS=main.o
EXEC=ModelSimulation

all: exec

clean:
	$(RM) $(OBJS) $(EXEC)

cleandata:
	$(RM) System1*.* System2*.* System3*.*

cleanall: clean cleandata

exec: $(EXEC)

run: $(EXEC)
	./$(EXEC) 1

$(EXEC): $(OBJS)
	$(ELINK) $@ $? $(CPPLIBS)

$(SOURCES): $(HEADERS)

.cpp.o:
	$(CPP) $@ $? $(CPPFLAGS)
