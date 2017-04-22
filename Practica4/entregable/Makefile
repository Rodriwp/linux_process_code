SRC_CLIENT = UserManager.cpp AlertSystem.cpp client.cpp
SRC_SERVER = UserManager.cpp AlertSystem.cpp server.cpp

all: interface server client

client: $(SRC_CLIENT)
	gcc -I. -c $(SRC_CLIENT)
	gcc -o client client.o UserManager.o AlertSystem.o -lIce -lIceUtil -pthread -lstdc++

server: $(SRC_SERVER)
	gcc -I. -c $(SRC_SERVER)
	gcc -o server server.o UserManager.o AlertSystem.o -lIce -lIceUtil -pthread -lstdc++

interface:
	slice2cpp UserManager.ice
	slice2cpp AlertSystem.ice

.PHONY: clean

clean:
	rm -f  UserManager.o
	rm -f  AlertSystem.o
	rm -f  server.o
	rm -f  client.o
	rm -f  server
	rm -f  client
