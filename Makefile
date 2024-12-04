build_all: 
	g++ client.cpp lib.cpp -o client -lcurl
	g++ server.cpp lib.cpp -o server -lcurl
	g++ gmail.cpp lib.cpp -o gmail -lcurl

client:
	./client

server: 
	./server 

mail: 
	g++ gmail.cpp lib.cpp -o gmail -lcurl && ./gmail