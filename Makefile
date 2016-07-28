##############

MY_BIN_CFLAGS  = -g -O0 -c 
MY_BIN_LDFLAGS = -lpthread -lstdc++ 

PGM  = demo_wait
OBJS = obj/main.o obj/clientFunction.o obj/serverFunction.o
SRC  = src

.PHONY:	clean run 

all:	$(PGM)

obj:	
	mkdir obj

obj/clientFunction.o:	obj src/clientFunction.cpp
	g++ -Wall -g $(MY_BIN_CFLAGS) src/clientFunction.cpp -o $@

obj/main.o:	obj src/main.cpp
	g++ -Wall -g $(MY_BIN_CFLAGS) src/main.cpp -o $@

obj/serverFunction.o:	obj src/serverFunction.cpp
	g++ -Wall -g $(MY_BIN_CFLAGS) src/serverFunction.cpp -o $@
	
$(PGM):	$(OBJS)
	g++ -o $@ $(OBJS) $(LIB_OBJS) $(MY_BIN_LDFLAGS)

clean:
	rm -rf  $(PGM) obj
		
run:	$(PGM)
	./$(PGM)
