# Arithemetic Parser (AParser)
> CLI tool for evaluating arithmetic expressions

C++ program to evaluate arithmetic expressions. This program also supports some File I/O
for rapid evaluation. Currently limited to CSV and untested boundaries of VRAM on your machine.

# NOTE

This program is developed for Plexsys Interface Products Inc.
as part of the interview process. Additionally, a majority of this program *may* be trivialized by
certain c++ libraries such as muparser, te_interp etc. It is my interpretation that the spirit of the 
assignment is to *not* utilize these libraries that can make maybe 1 function call to fully evaluate
expressions vs. practicing/ showing knowledge of data structures, recursion etc.

# Implementation
Sported the idea of S-expression tree of sorts, and ended doing a more traditional Binary tree that makes
use of the fact that we continually NULL out certain nodes to handle operator priority.

The tree is created from left to right on expression with only parenthesis operand operator operand
pairs being loaded at the bottonm first, as appropriate.

We then execute an Inorder traversal looking for operators with high priority (^, */, +-) etc. and evaluate those pairs 
**first**. We proceed through the rest of the tree for each level of precedence. 

Thus, the resulting output is the final nodal value that is moved along the Binary tree rather than a runnign total/ gloabl variable approach. 


## Installing / Getting started

A pre-compiled binary is included for the purpose of
providing quick testing for Plexsys.

To run:
Unix environment:

In local project directory:
```shell
AParser "./AParser"
```

Then follow prompts prompts delivered by program.

### Building
Unix:
No initial configuration is required. 
Dependecies are handled through makefile.
Simply run (in local project directory)
```shell
make clean
make
```

Windows:
You will need CyGwin, g++ compiler. (unix emulator)
In the future I will open this up with MsVisualStudio and configure it.

## Features
* Minimum features (met) dual operand entries supported for all following operations:
* Addition, Subtraction, Multiplication, division, and exponentiation are supported.
* Nested expressions with order of operations implemented
* Load csv file (comma delimmited) file to evaluate lots of expressions
* ~Some~ algorithm analysis and timing done
* Improved Binary tree method, in order traversal utilizes tall tree height to solve problems 

## Features in progress (some for fun)
* Parenthesis/ priority grouping, there are still errors with priority
* Ascii interpretation/ exception handling of operators
* "Potential" Library use


## Example usage

In order to utilize File IO pass a csv file as an argument to AParser.

#### File IO
```shell
"./AParser ./testFiles/test.csv"
"Expression step: 1+2: 3"
"..."
```
* BIG TEST ~warning.. around 4 million records. algorithm test
```shell
"./AParser ./testFiles/bigBinaryTest.csv"
"..Debug output"
"Expression step: 1+2: 3"
"..."
```
very rough benchmark: i7 intel cpu finishes ~4 million records 60 seconds
(the simplest expressions e.g. 2+2, 2-2 etc.)

In order to utilize traditional CLI expression input: run program without any arguments

#### Traditional CLI  
```shell
"./AParser"
"Enter an arithmetic expression to be evaluated followed by enter:"
"1+1"
"..Debug output"
"Output: 2"
```

