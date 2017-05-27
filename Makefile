all: compile link

compile:
	$ clear
	$ g++ -c *.cpp -std=c++11

link:
	$ g++ -o main *.o `pkg-config --libs --cflags opencv`
	rm *.o
clean:
	$ rm *.o
