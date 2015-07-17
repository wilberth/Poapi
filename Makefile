CFLAGS	+= -std=c99 -I/opt/NDIoapi/ndlib/include/ -I .  -O0 -g3 -DPLATFORM_LINUX 

LDLIBS = -L/opt/NDIoapi/ndlib/lib -loapi -lm -ldl

# makefile rules ***********************************************************

.PHONY : all install clean cleanall

all: poapi

poapi: poapi.o
	gcc poapi.o $(LDLIBS) -o poapi

install: poapi poapi.py
	sudo cp poapi.py poapi /usr/local/lib/python2.7/dist-packages/

clean:
	rm -f *.o poapi

cleanall:
	rm -f *.o poapi *.pyc

