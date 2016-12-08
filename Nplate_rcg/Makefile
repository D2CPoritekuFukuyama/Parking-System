main: main.o  Nplate_trim.o Elem_trimming.o  
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -o main *.o 


Nplate_trim.o: Nplate_trim.cpp labeling.cpp WarpPerspective.cpp 
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -c Nplate_trim.cpp labeling.cpp WarpPerspective.cpp

Elem_trimming.o: Elem_trimming.cpp labeling.cpp 
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -c Elem_trimming.cpp labeling.cpp 

main.o: main.cpp 
	g++ -Wall `pkg-config opencv --cflags` `pkg-config opencv --libs` -c main.cpp 

clean:
	rm -f *.o main
