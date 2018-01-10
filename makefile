OBJS = dpl.o lex.o parse.o eval.o
OPTS = -Wall -g -std=c++11

dplan : $(OBJS)
	g++ $(OPTS) $(OBJS) -o dplan -lm

dpl.o : dpl.cpp lex.h parse.h eval.h
	g++ $(OPTS) -c dpl.cpp

lex.o : lex.cpp lex.h
	g++ $(OPTS) -c lex.cpp

parse.o : parse.cpp parse.h
	g++ $(OPTS) -c parse.cpp

eval.o : eval.cpp eval.h
	g++ $(OPTS) -c eval.cpp

run :
	./dplan testprogram

error1 :
	cat error1_test

error1x :
	./dplan error1_test

error2 :
	cat error2_test

error2x :
	./dplan error2_test

error3 :
	cat error3_test

error3x :
	./dplan error3_test

error4 :
	cat error4_test

error4x :
	./dplan error4_test

error5 :
	cat error5_test

error5x :
	./dplan error5_test

conditionals :
	cat conditionals_test

conditionalsx :
	./dplan conditionals_test

recursion :
	cat recursion_test

recursionx :
	./dplan recursion_test

iteration :
	cat iteration_test

iterationx :
	./dplan iteration_test

clean :
	rm -f $(OBJS) dplan