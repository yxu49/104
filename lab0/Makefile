# $Id: Makefile,v 1.1 2018-04-02 15:31:00-07 - - $

CPP    = g++ -g -O0 -std=gnu++17 -Wall -Wextra -Wold-style-cast
CPPYY  = g++ -g -O0 -std=gnu++17 -Wno-register
OBJS   = main.o yyparse.o yylex.o
GENS   = yyparse.h yyparse.cpp yylex.cpp
SRCS   = extern.h lexer.l parser.y main.cpp Makefile

all : rpn

rpn : ${OBJS}
	${CPP} -o rpn ${OBJS}

%.o : %.cpp
	checksource $<
	cpplint.py.perl $<
	${CPP} -c $<

yyparse.cpp yyparse.h : parser.y
	bison --defines=yyparse.h --output=yyparse.cpp parser.y

yyparse.o : yyparse.cpp
	${CPPYY} -c yyparse.cpp

yylex.cpp : yyparse.h lexer.l
	flex --outfile=yylex.cpp lexer.l

yylex.o : yylex.cpp
	${CPPYY} -Wno-register -c yylex.cpp

spotless : clean
	- rm rpn Makefile.deps Listing.{ps,pdf}

clean :
	- rm ${OBJS} ${GENS}

ci : 
	- cid + ${SRCS}

lis : ${SRCS}
	mkpspdf Listing.ps ${SRCS}

Makefile.deps : yyparse.cpp yylex.cpp
	${CPP} -MM yyparse.cpp yylex.cpp main.cpp

include Makefile.deps

