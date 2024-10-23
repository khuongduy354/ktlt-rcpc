build_all: 
	g++ client.cpp lib.cpp -o client  
	g++ server.cpp lib.cpp -o server 

client:
	./client

server: 
	./server
