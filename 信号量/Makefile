
server:server.o comm.o
	gcc -o $@ $^

%.o:%.c
	gcc -c $<

.PHONY:clean
clean:
	rm -rf server *.o
	

