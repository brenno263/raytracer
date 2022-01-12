#compiler
GCC=g++

#exe name
EXE=ray

#ppm name
PPM_NAME=out

ODIR=obj
IDIR=.

CFLAGS=-I$(IDIR)
LIBS=-lm -lpthread

#FLAGS=$(CFLAGS) $(LIBS)

#$(patsubst pattern,replacement,text)
#convert all DEPs from asdf.h to $(IDIR)/asdf.h
DEPS=
DEPS:=$(patsubst %,$(IDIR)/%,$(DEPS))

#convert all OBJs from asdf.o to $(ODIR)/asdf.o
OBJ=main.o
OBJ:=$(patsubst %,$(ODIR)/%,$(OBJ))

#all .o files depend on .c files and deps
$(ODIR)/%.o: %.cpp $(DEPS)
	$(GCC) -c $< -o $@ $(CFLAGS)

#if $(OBJ) has changed, do this to compile main
#$@, $^ - left and right sides of the ':'
$(EXE): $(OBJ)
	$(GCC) -o $@ $^ $(LIBS)

#declares that 'clean' is not a file to look for,
#so the target is to be used as a command
.PHONY: clean fresh run view all

all: $(EXE)

#remove non-src stuff
clean:
	rm -f $(EXE) $(ODIR)/*.o *~ core $(INCDIR)/*~ *.o *.gch

fresh: clean $(EXE)

run: $(EXE)
	./$(EXE)
	
ppm: $(EXE)
	./$(EXE) > $(PPM_NAME).ppm

view: ppm
	eog $(PPM_NAME).ppm &
