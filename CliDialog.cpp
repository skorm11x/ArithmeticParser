#include <iostream>
#include <string>
class CliDialog{


    public:

    /*
    * Function asks CLI user for arithmetic expression and returns user input string
    */
    std::string askForExpression(int isValid){
        std::string expressionInput;
        if(isValid){
            std::cout << "Enter an arithmetic expression to be evaluated followed by enter: \n";
            std::getline(std::cin, expressionInput);
            //need to ignore when doing multiple input
            //std::cin.ignore(32767, '\n'); // remove \n from input stream
            return expressionInput;
        }
        else{
            std::cout << "Invalid expression entered. Please enter a valid infix notation arithmetic expression: \n";
            std::getline(std::cin, expressionInput);
            //need to ignore when doing multiple input
            //std::cin.ignore(32767, '\n'); // remove \n from input stream
            return expressionInput;
        }
    }
    
};