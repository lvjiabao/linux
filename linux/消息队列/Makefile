.PHONY:all
all:client server

client:client.o comm.o
	gcc -o $@ $^
server:server.o comm.o
	gcc -o $@ $^

%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -rf server client *.o
	

