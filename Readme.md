# Arithemetic Parser (AParser)
> CLI tool for evaluating arithmetic expressions

C++ program to evaluate arithmetic expressions. This program also supports some File I/O
for rapid evaluation. Currently limited to CSV and untested boundaries of VRAM on your machine.

## Installing / Getting started

A pre-compiled binary + windows executable is included for the purpose of
providing quick testing for Plexsys.

To run:
Unix environment:

In local project directory:
```shell
AParser "./AParser"
```

Windows environment:

In local project directory:
Powershell (as administrator to be safe)
```shell
AParser "./AParser.exe"
```

Then follow prompts prompts delivered by program.

### Building

No initial configuration is required. 
Dependecies are handled through makefile.
Simply run
```shell
make clean
make
```
in local project directory

## Features

* Addition, Subtraction, Multiplication, division, and exponentiation are supported.
* Load csv file (comma delimmited) file to evaluate lots of expressions
* ~Some~ algorithm analysis and timing done 

## Example usage

In order to utilize File IO pass a csv file as an argument to AParser.

#### File IO
```shell
"./AParser ./testFiles/test.csv"
"..Debug output"
"Output: 2"
"Output: 3"
```

In order to utilize traditional CLI expression input: run program without any arguments

#### Traditional CLI  
```shell
"./AParser"
"Enter an arithmetic expression to be evaluated followed by enter:"
"1+1"
"..Debug output"
"Output: 2"
```

## Note

This program is developed for Plexsys Interface Products Inc.
as part of the interview process. 

