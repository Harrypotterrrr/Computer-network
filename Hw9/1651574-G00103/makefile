TARGET = server client 

main: $(TARGET) 

server: server.cpp myutil.cpp
	g++ server.cpp myutil.cpp -o server

client : client.cpp myutil.cpp
	g++ client.cpp myutil.cpp -o client

clean :
	rm -f $(TARGET) *.o *.txt
	rm -rf clientout serverout