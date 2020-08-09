
myCalc: myCalc.o evaluation.o
	g++ myCalc.o evaluation.o -o myCalc
myCalc.o: myCalc.cpp
	g++ -c myCalc.cpp
evaluation.o: evaluation.cpp mycalc.h
	g++ -c evaluation.cpp

clean:
	rm *.o myCalc

