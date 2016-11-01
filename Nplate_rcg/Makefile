main: main.o labeling.o templateMatch.o
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -o main main.o labeling.o templateMatch.o

labeling.o: labeling.cpp
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -c labeling.cpp

templateMatch.o: templateMatch.hpp 
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -c templateMatch.cpp 

main.o: main.cpp
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -c main.cpp  

clean:
	rm -f *.o main
