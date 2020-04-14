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
    // //<< is the insertio operator part of std::cout which lives in iostream library
    // // double x{0};
    // // double y{0};
    // // double z{0};
    // // double g{0};
    // CliDialog dialog;
    // int x{0};
    // int y{0};
    // int z{0};
    // int g{0};
    // std::cout << "Input first operand to continue: \n";
    // std::cin >> x;
    // std::cout << "Input second operand to continue: \n";
    // std::cin >> y;
    // z = addOperands(x,y);
    // g = multOperands(x,y);
    // std::cout << "Z value: " << z << "\n";
    // std::cout << "G value: " << g << "\n";
    // dialog.sayHello();
    CliDialog dialog;
    ExpressionEval eval;
    eval.receiveExpression();
    //std::cout << dialog.askForExpression() << "\n";
    eval.sayHello();


    return 0;
}

/*
 * Function selects whether to run off of CLI or file IO mode
 * paramaters passed by value copy
 */ 
void selectMode(){

}

/*
 * Function accepts two operands and returns the addition result 
 * paramaters passed by value copy
 */
int addOperands(int operandOne, int operandTwo){
    int additionResult = (operandOne+operandTwo);
    return additionResult;
}

/*
 * Function overload of addOperands type double
 * paramaters passed by value copy
 */ 
double addOperands(double operandOne, double operandTwo){
    double additionResult = (operandOne+operandTwo);
    return additionResult;
}

/*
 * Function accepts two operands and returns the multiplication result
 * paramaters passed by value copy  
 */ 
int multOperands(int operandOne, int operandTwo){
    int multResult = (operandOne*operandTwo);
    return multResult;
}

/*
 * Function overload of multOperands type double
 * paramaters passed by value copy  
 */ 
double multOperands(double operandOne, double operandTwo){
    double multResult = (operandOne*operandTwo);
    return multResult;
}

int subtOperands(int operandOne, int operandTwo){
    int subtResult = (operandOne-operandTwo);
    return subtResult;
}

double subtOperands(double operandOne, double operandTwo){
    double subtResult = (operandOne-operandTwo);
    return subtResult;
}
