CFLAGS	+= -std=c99 -I/opt/NDIoapi/ndlib/include/ -I .  -O0 -g3 -DPLATFORM_LINUX 

LDLIBS = -L/opt/NDIoapi/ndlib/lib -loapi -lm -ldl

# makefile rules ***********************************************************

.PHONY : all clean cleanall install uninstall

all: poapi

poapi: poapi.o
	gcc poapi.o $(LDLIBS) -o poapi

clean:
	rm -f *.o poapi

