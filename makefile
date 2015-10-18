all: routing ssp
routing: trie.o
	g++ trie.o -o routing
	rm trie.o
ssp: ssp.o FibonacciHeap.o
	g++ ssp.o FibonacciHeap.o -o ssp
	rm ssp.o
	rm FibonacciHeap.o
trie.o: trie.cpp
	g++ -c trie.cpp
FibonacciHeap.o: FibonacciHeap.cpp FibonacciHeap.h
	g++ -c FibonacciHeap.cpp
ssp.o: ssp.cpp
	g++ -c ssp.cpp
clean:
	rm -f ssp *.o *~
