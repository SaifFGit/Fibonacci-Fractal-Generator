CFLAGS= -Wall -O2 -ansi -g -pg
fib: main_fib.o fib.o bmp.o timing.o
		$(CXX) -pg -o fib $?
runall:
		./fib 7 30 10 10 100 100 fib7.bmp
		./fib 9 40 10 10 130 130 fib9.bmp
		./fib 25 10 10 10 11000 10000 fib25.bmp
		./fib 26 10 10 10 15000 15000 fib26.bmp
clean:
		@rm -rf fib *.o *bmp
		