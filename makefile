build: main.o prot.o
	g++ -o 2pa main.o prot.o
	strip 2pa
main.o:	main.cpp
	g++ -c -Ofast main.cpp
prot.o: prot.cpp prot.h
	g++ -c -Ofast prot.cpp prot.cpp
clear:
	rm *.o
