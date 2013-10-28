
CPP=g++ -c -o
ELINK=g++ -o
RM=rm -f

CPPFLAGS=
CPPLIBS=-lm -lmgl

SOURCES=main.cpp
HEADERS=ModelSimulation.hpp
OBJS=main.o
EXEC=ModelSimulation

all: exec

clean:
	$(RM) $(OBJS) $(EXEC)

cleandata: clean
	$(RM) System1*.* System2*.*

exec: $(EXEC)

run: $(EXEC)
	./$(EXEC) 1

$(EXEC): $(OBJS)
	$(ELINK) $@ $? $(CPPLIBS)

$(SOURCES): $(HEADERS)

.cpp.o:
	$(CPP) $@ $? $(CPPFLAGS)
