all: AParser 

AParser: Main.o FileIO.o ExpressionEval.o CliDialog.o
	g++ -g -o AParser Main.o FileIO.o ExpressionEval.o CliDialog.o

Main.o: Main.cpp FileIO.o AParser.h
	g++ -g -c Main.cpp 

FileIO.o: FileIO.cpp ExpressionEval.o AParser.h
	g++ -g -c FileIO.cpp

ExpressionEval.o: ExpressionEval.cpp CliDialog.o AParser.h
	g++ -g -c ExpressionEval.cpp

CliDialog.o: CliDialog.cpp AParser.h
	g++ -g -c CliDialog.cpp

cleanObj:
	rm -f Main.o FileIO.o ExpressionEval.o CliDialog.o

clean:
	rm -f Main.o FileIO.o ExpressionEval.o CliDialog.o AParser