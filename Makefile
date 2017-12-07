#Compiles the three files using the pthread library
# 'make newshell' compiles the shell
# 'make run' runs the executable
# 'make clean' removes the executable
CC=gcc
CFLAGS=-g
newshell: svrMajor2.c cliMajor2.c 
	$(CC) -o newshell $(CFLAGS) svrMajor2.c cliMajor2.c
clean:
	$(RM) newshell
run: newshell
	./newshell
