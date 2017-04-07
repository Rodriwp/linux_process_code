SRC = UserManager.cpp AlertSystem.cpp server.cpp

all: server

server: $(SRC)
	gcc -I. -c $(SRC)
	gcc -o server server.o UserManager.o AlertSystem.o -lIce -lIceUtil -pthread -lstdc++

.PHONY: clean

clean:
	rm -f  server
