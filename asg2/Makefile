ALLSRC = main.cpp string_set.cpp auxlib.cpp lyutils.cpp astree.cpp
GPP = g++ -std=gnu++14 -g -O0 -Wall -Wextra
EXECBIN=oc

all:${EXECBIN}
	
${EXECBIN}: string_set.o auxlib.o astree.o lyutils.o main.o main.cpp
	${GPP} -o oc main.o string_set.o auxlib.o
string_set.o: string_set.h string_set.cpp
	${GPP} -c string_set.cpp
auxlib.o: auxlib.cpp auxlib.h
	${GPP} -c auxlib.cpp
astree.o: astree.h astree.cpp
	${GPP} -c astree.cpp
lyutils.o: lytils.h lyutils.cpp
	${GPP} -c lyutils.cpp
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
