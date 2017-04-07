SRC = UserManager.cpp AlertSystem.cpp client.cpp

all: client

client: $(SRC)
	gcc -I. -c $(SRC)
	gcc -o client client.o UserManager.o AlertSystem.o -lIce -lIceUtil -pthread -lstdc++

.PHONY: clean

clean:
	rm -f  client
