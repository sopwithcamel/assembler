HEADERS = assembler.h

default: assembler

assembler.o: assembler.cpp $(HEADERS)
	g++ -c -g assembler.cpp -o assembler.o

assembler: assembler.o
	g++ assembler.o -o assembler

clean:
	rm -f assembler.o
	rm -f assembler
