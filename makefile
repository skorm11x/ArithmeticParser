all: AParser 

#target requirements for Arithmetic Parser, normal binary + windows exe
AParser: Main.cpp CliDialog.cpp ExpressionEval.cpp FileIO.cpp
	g++ -o AParser Main.cpp CliDialog.cpp ExpressionEval.cpp FileIO.cpp 
	g++ -o AParser.exe Main.cpp CliDialog.cpp ExpressionEval.cpp FileIO.cpp 	 

#remove the binary file
#TODO: generate seperate object files
clean:
	rm -f AParser AParser.exe