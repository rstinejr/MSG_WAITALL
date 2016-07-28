##############

MY_BIN_CFLAGS  = -g -O0 
MY_BIN_LDFLAGS = -lpthread -lstdc++ -lrt

PGM  = demo_wait
OBJS = main.o
SRC  = src

.PHONY:	clean run 

all:	$(PGM)

main.o:	main.cpp
	g++ -Wall -g $(MY_BIN_CFLAGS) -c main.cpp -o $@

$(PGM):	$(OBJS)
	g++ -o $@ $(OBJS) $(LIB_OBJS) $(MY_BIN_LDFLAGS)

clean:
	rm -f  $(PGM) $(OBJS) $(LIB_OBJS)
		
run:	$(PGM)
	./$(PGM)
