all:
	gcc sender.c Other/DieWithError.c -o sender
	gcc receiver.c Other/DieWithError.c -o receiver
	gcc server.c Other/DieWithError.c Other/AcceptTCPConnection.c Other/CreateTCPServerSocket.c -o server

clear:
	rm receiver server sender