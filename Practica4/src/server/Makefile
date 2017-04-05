SRC = UserManager.cpp server.cpp

all: server

server: $(SRC)
	gcc -I. -c $(SRC)
	gcc -o server server.o UserManager.o -lIce -lIceUtil -pthread -lstdc++

.PHONY: clean

clean:
	rm -f  server
