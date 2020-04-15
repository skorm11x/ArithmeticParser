#include <iostream>
#include <cstdlib> // for EXIT_SUCCESS and EXIT_FAILURE status codes
#include "ExpressionEval.cpp"
#include "ArithmeticParser.h"
/*
 * @author: Christopher J. Kosik 
 * 
 * 
 */



/*
 * Entry point of c++ application 
 */ 
int main(int argc, char *argv[]){
    ExpressionEval eval;

    eval.receiveExpression();
    return 0;
}

/*
 * Function selects whether to run off of CLI or file IO mode
 * paramaters passed by value copy
 */ 
void selectMode(){

}

