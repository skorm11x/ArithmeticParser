#include <iostream>
#include <string>
#include "CliDialog.cpp"
#include "ArithmeticParser.h"
class ExpressionEval{
    CliDialog dialog;

    char validChars[20] = {'+','*','-','/','(',')','{','}','[',']','0','1','2','3','4','5','6','7','8','9'};
    char operatorChars[4] = {'+','*','-','/'};

    //global flag for valid or invalid expression, updated or not during parsing
    int isValid = 1; //expression valid by default


    void parseExpression(std::string expression){
        //evaluate expression for invalid characters

        int validCharMatch = 0;
        #ifdef PARSE_OUTPUT
            std::cout << "Recevied: "<< expression <<" with length of: "<< expression.length() <<"\n";
        #endif
        if(expression.length() >= 3){
            for(int i = 0; i< expression.length()-1; ++i){
                //check to see if character is on valid list,
                //Check to see if expression has adjacent operators
                for(int j = 0; j <(sizeof(validChars)/sizeof(validChars[0]))-1; ++j){
                    if(expression[i] == validChars[j]){
                        validCharMatch = 1; //flag that character matched one of the valid characters
                    }
                }
                if(!validCharMatch){
                    #ifdef PARSE_OUTPUT
                        std::cout << "invalid character: "<< expression[i] <<"\n";
                    #endif
                        isValid = 0;
                        return; //stop evaluating illegal input 
                }
                if(validCharMatch){
                    //Check to see if expression starts or ends with an operator,
                    //Check to see if operators are adjacent of each other
                    for(int j = 0; j< (sizeof(operatorChars)/sizeof(operatorChars[0]))-1; ++j){
                        if(expression[0] == operatorChars[j] || expression[expression.length()-1] == operatorChars[j]){
                            #ifdef PARSE_OUTPUT
                                std::cout << "Start or end invalid: "<< expression[0] << ", "<<expression[expression.length()-1] << "\n";
                            #endif
                                isValid = 0;
                                return; //stop evaluating illegal input
                        }
                        //check to see if current expression character is an operator
                        if(expression[i] == operatorChars[j]){
                            //look at the previous and next expression character,
                            //except in case of the first and last expression character
                            if(i == 0){
                                //for first char look at second character
                                if(expression[i+1] == operatorChars[j]){
                                    #ifdef PARSE_OUTPUT
                                        std::cout << "Two Adjacent operators: "<< expression[0] << ", "<<expression[1] << "\n";
                                    #endif
                                        isValid = 0;
                                        return; //stop evaluating illegal input
                                }
                            }
                            else if(i == expression.length()-1){
                                //for last char look at 2nd to last character
                                if(expression.length()-2 == operatorChars[j]){
                                    #ifdef PARSE_OUTPUT
                                        std::cout << "Two Adjacent operators: "<< expression[expression.length()-1] << ", "<<expression[expression.length()-2] << "\n";
                                    #endif
                                        isValid = 0;
                                        return; //stop evaluating illegal input
                                }
                            }
                            else{
                                //otherwise look at previous char and next char
                                if(expression[i-1] == operatorChars[j] || expression[i+1] == operatorChars[j]){
                                    #ifdef PARSE_OUTPUT
                                        std::cout << "Two Adjacent operators: "<< expression[i-1] << ", "<<expression[i] << ", "<< expression[i+1]<< "\n";
                                    #endif
                                        isValid = 0;
                                        return; //stop evaluating illegal input
                                }

                            }
                        } 
                    }
                    

                    validCharMatch = 0; // reset flag for checking input char validity
                }
            }
        }
        else{
            isValid = 0;
            return;
        }
        /*
        if function has reached this point the length is correct, there are no adjacent operators,
        the program does not start or end with an operator, and no invalid characters exist in
        the expression. The last thing we must do is partition the expression into operand and operator
        segments which will also determine validity.
        For example: 222 will pass all the above tests but boils down to a single
        operand entry and is not a valid expression (as defined by me in docs, some definitions say
        that 222 is a math expression!?). Our definition for minimum of math expression is 2 operands and
        one operator strictly input in infix notation.
        Additionally, all operators must be implicitly declared
        */


       //handles cases without any parenthesis entered at all first
       //self implemented limitation 



        #ifdef PARSE_OUTPUT
            std::cout << "isValid value: "<< isValid << "\n";
        #endif

    }
    

    public:

    void sayHello(){

        std::cout << "Hello from expression eval\n";
    }

    /*
    *  Function receive input expression and calls class functions
    *  to determine expression validity, parse expression, and calculate result
    */ 
    void receiveExpression(){
        std::string expression;
        do{
            expression = dialog.askForExpression(isValid);
            parseExpression(expression);

        }while(!isValid);   
        
    }
};
