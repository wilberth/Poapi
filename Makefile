CFLAGS	+= \
	-std=c99 -I/usr/include/python2.7 -I/usr/share/pyshared/numpy/core/include -I /opt/NDIoapi/ndlib/include/ -I . \
	-O0 -g3 -DPLATFORM_LINUX 

LDLIBS = -L/opt/NDIoapi/ndlib/lib -loapi -lm -ldl

# makefile rules ***********************************************************

.PHONY : all clean cleanall install uninstall

all: poapi

poapi: poapi.o
	gcc poapi.o $(LDLIBS) -o poapi

optotrak.o: optotrak.c
	gcc -c $(CFLAGS) poapi.c -o poapi.o 

clean:
	rm -f *.o poapi

