SRC = UserManager.cpp client.cpp UserManagerI.cpp

all: client

my_route_lookup: $(SRC)
	gcc -I. -c $(SRC)
	gcc -o client client.o UserManager.o -lIce -lIceUtil -pthread -lstdc++

.PHONY: clean

clean:
	rm -f  client


