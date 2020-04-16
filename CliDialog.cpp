/*
 *****************************************************************
 *                         CliDialog.cpp                         *
 * Explanation: Perform CLi dialog with user outside of debug    *
 * Author: Christopher J. Kosik  16-Apr-2019 (current)           *          
 * See included Readme.md file for details.                      *
 *****************************************************************
*/

#include <iostream>
#include <string>
class CliDialog{


    public:

    /*
    * Function asks CLI user for arithmetic expression and returns user input string
    * Only allows a single expression to evaluated per program run atm.
    */
    std::string askForExpression(int isValid){
        std::string expressionInput;
        if(isValid){
            std::cout << "Enter an arithmetic expression to be evaluated followed by enter: \n";
            std::getline(std::cin, expressionInput);
            return expressionInput;
        }
        else{
            std::cout << "Invalid expression entered. Please enter a valid infix notation arithmetic expression: \n";
            std::getline(std::cin, expressionInput);
            return expressionInput;
        }
    }

    /*
    * Informs CLI user of file input process.
    */ 
    void informUserFileMode(char * filename){
        std::cout <<"Beginning File IO mode. \n";
        std::cout <<"Opening file located at: "<<filename<<"\n";
    }
    
};