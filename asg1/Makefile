ALLSRC = main.cpp string_set.cpp auxlib.cpp
GPP = g++ -std=gnu++17 -g -O0 -Wall -Wextra -Wold-style-cast
EXECBIN=oc

all:${EXECBIN}
	
${EXECBIN}: string_set.o auxlib.o main.o main.cpp
	${GPP} -o oc main.o string_set.o auxlib.o
string_set.o: string_set.h string_set.cpp
	${GPP} -c string_set.cpp
auxlib.o: auxlib.cpp auxlib.h
	${GPP} -c auxlib.cpp
main.o: main.cpp
	${GPP} -c main.cpp
ci:
	ci -u ${ALLSRC}
deps:
	g++ -MM -std=gnu++14  ${ALLSRC} > Makefile.deps
clean:
	rm -f *.o
spotless: clean
	rm -f oc *.o
god:
	rm -f *.log *.oc *.str *.err *.out
