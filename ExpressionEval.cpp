#include <iostream>
#include <string>
#include <cctype>
#include <bits/stdc++.h> 
#include "CliDialog.cpp"
#include "ArithmeticParser.h"
class ExpressionEval{
    CliDialog dialog;

    // An expression tree node 
    struct expressTree 
    { 
        double value; //either operator or numeric value
        bool isLeaf;  //only values can be leaf nodes! 
        expressTree* left, *right;  //left and right pointers to nodes, null for leaf..
    }; 

    char validChars[22] = {'+','*','-','/','(',')','{','}','[',']','0','1','2','3','4','5','6','7','8','9','.',','};
    char priorityChars[6] = {'(',')','{','}','[',']'};
    char operatorChars[4] = {'+','*','-','/'};

    //global flag for valid or invalid expression, updated or not during parsing
    int isValid = 1; //expression valid by default

    //Knew I wanted to build some tree and keep local evaluations built in a stack
    //Source of part of idea: https://www.geeksforgeeks.org/expression-tree/

    // A utility function to create a new node 
    expressTree* newNode(double v) 
    { 
        expressTree *temp = new expressTree; 
        temp->left = temp->right = NULL; 
        temp->value = v; 
        return temp; 
    };

    int sanitizeExpression(std::string expression){
        //evaluate expression for invalid characters, maximum number of around 22*n overhead
        // sanitizing input requires overhead cost but eases parsing section
        int isClean = 1;
        int validCharMatch = 0;
        #ifdef PARSE_OUTPUT
            std::cout << "Recevied: "<< expression <<" with length of: "<< expression.length() <<"\n";
        #endif
        if(expression.length() >= 3){
            for(int i = 0; i< expression.length(); ++i){
                //check to see if character is on valid list,
                //Check to see if expression has adjacent operators
                for(int j = 0; j <(sizeof(validChars)/sizeof(validChars[0])); ++j){
                    //sanitize each character in the expression string, ignore spaces
                    if(expression[i] == validChars[j] || isspace(expression[i])){
                        validCharMatch = 1; //flag that character matched one of the valid characters
                    }
                }
                if(!validCharMatch){
                    #ifdef PARSE_OUTPUT
                        std::cout << "invalid character: "<< expression[i] <<"\n";
                    #endif
                        isClean = 0;
                        return isClean; //stop evaluating illegal input 
                }
                if(validCharMatch){
                    //Check to see if expression starts or ends with an operator,
                    //Check to see if operators are adjacent of each other
                    for(int j = 0; j< (sizeof(operatorChars)/sizeof(operatorChars[0])); ++j){
                        if(expression[0] == operatorChars[j] || expression[expression.length()-1] == operatorChars[j]){
                            #ifdef PARSE_OUTPUT
                                std::cout << "Start or end invalid: "<< expression[0] << ", "<<expression[expression.length()-1] << "\n";
                            #endif
                            isClean = 0;
                            return isClean; //stop evaluating illegal input
                        }
                        //check to see if current expression character is an operator
                        if(expression[i] == operatorChars[j]){
                            if(expression[i-1] == operatorChars[0] || expression[i+1] == operatorChars[0]){
                                #ifdef PARSE_OUTPUT
                                    std::cout << "Adjacent with + operators: "<< expression[i-1] << ", "<<expression[i] << ", "<< expression[i+1]<< "\n";
                                #endif
                                    isClean = 0;
                                    return isClean; //stop evaluating illegal input
                            }
                            else if (expression[i-1] == operatorChars[1] || expression[i+1] == operatorChars[1]){
                                #ifdef PARSE_OUTPUT
                                    std::cout << "Adjacent with * operators: "<< expression[i-1] << ", "<<expression[i] << ", "<< expression[i+1]<< "\n";
                                #endif
                                    isClean = 0;
                                    return isClean; //stop evaluating illegal input
                            }
                            else if (expression[i-1] == operatorChars[2] || expression[i+1] == operatorChars[2]){
                                #ifdef PARSE_OUTPUT
                                    std::cout << "Adjacent with - operators: "<< expression[i-1] << ", "<<expression[i] << ", "<< expression[i+1]<< "\n";
                                #endif
                                    isClean = 0;
                                    return isClean; //stop evaluating illegal input
                            }
                            else if (expression[i-1] == operatorChars[3] || expression[i+1] == operatorChars[3]){
                                #ifdef PARSE_OUTPUT
                                    std::cout << "Adjacent with / operators: "<< expression[i-1] << ", "<<expression[i] << ", "<< expression[i+1]<< "\n";
                                #endif
                                    isClean = 0;
                                    return isClean; //stop evaluating illegal input
                            }

                        } 
                    }
                    

                    validCharMatch = 0; // reset flag for checking input char validity
                }
            }
        }
        else{
            //impossible for expression to not contain at lest characters by definition
            return 0;
        }

        return isClean; //if program made it to this point did not return early and should be value 1
    }

    void parseExpression(std::string expression){

       //Evaluate expression from left to right
        isValid = sanitizeExpression(expression);
        #ifdef PARSE_OUTPUT
            std::cout << "isValid value: "<< isValid << "\n";
        #endif
        std::stack <expressTree *> operand_stack;
        expressTree *op1Node, *op2Node, *operatorNode, *rootNode; //right: 1, left: 2
        //parseExpression proceeds once input has been sanitizied
        if(isValid){
            //input has been sanitized, only validChars are present
            //either characters create operands, indicate priority, or are operators

            /*
                Inefficient algorithm explanation:
                work like a human and perform a linear search looking for any priority indicators,
                parenthesis, brackets etc.

                Then, apply order of operations priority
                Shunting algorithm utilizing 2 stacks runs O(1) time really is "best" way to do this
            */

           int inPriority = 0;// flag to indicate whether we are building expression inside parenthesis etc.
           int state = 0; //track num of operand/operator
           for(int i = 0; i< expression.length(); ++i){
        
               for(int j = 0; j<(sizeof(priorityChars)/sizeof(priorityChars[0])); ++j){
                    if(expression[i] == priorityChars[j]){
                        //we found a bracket, parenthesis, brace etc.

                        //because we are reading from left to right we expect (require) the 
                        //priority indicator to be an open: (, {, [
                        //valid characters and operators but incorrect priority sequence otherwise
                        if(expression[i] == '{' || expression[i] == '(' || expression[i] == '['){
                            //beginning of a priority segment found
                            //guaranteed correct chars + operator sequencing so recursivly create smaller problem, add to stack
                            #ifdef PARSE_OUTPUT
                                    std::cout << "Start Priority char found: "<< expression[i] << "\n";
                            #endif

                            //
                        }
                        else{
                            //end of a priority segment found
                            #ifdef PARSE_OUTPUT
                                    std::cout << "End Priority char found: "<< expression[i] << "\n";
                            #endif
                        }
                    }


               }
               //character is not a priority indicator (parenthesis)
               if(isdigit(expression[i]) || expression[i] == ',' || expression[i] == '.'){

                   //the character is not an operator, so we need to build the operand 
                   double buildOperand = 0;
                   int decimalFlag  = 0; //flag set for handling values after decimal place
                   double valueToAdd = 0; // hold a value that will divided by each time
                   double divisorValue = 1;// hold a value that will divided by each time

                   while(i < expression.length() && (isdigit(expression[i]) || expression[i] == ',' || expression[i] == '.')){
                       //**ONLY ACCURATE DOWN TO 1000TH PLACE DECIMAL**

                       //just ignore comma's although input may be input that way
                       //decimals will indicate building double value
                       //because this is in base 10 (assumption) every value entered will be multiple of 10 difference/ shift
                       if(isdigit(expression[i]) && decimalFlag == 0){
                            buildOperand = (buildOperand*10) + (expression[i] - '0'); //subtract 0 from end to convert to integer
                       }
                       else if(isdigit(expression[i]) && decimalFlag == 1){
                           divisorValue = divisorValue*10;
                           valueToAdd = ((double)expression[i] - '0')/divisorValue;
                           buildOperand = buildOperand + (valueToAdd);
                       }
                       else if(expression[i] == '.'){
                           //when decimal point is encountered now data is essentially growing in the opposite direction, with decimal point as origin
                           //buildOperand = buildOperand + 0.0; //
                           decimalFlag = 1;
                       }

                       ++i;
                   }
                    #ifdef PARSE_OUTPUT
                            std::cout << "Build Operand value: "<< buildOperand << "\n";
                    #endif
                    //we have succesffully built an operand, push to stack, build new node
                    if(state == 2){
                        //we have found the second operand, and are ready to build the tree
                        //operator at 1 position, op1 at position 0
                        #ifdef STACK_OUTPUT
                            std::cout << "Op2 check Stack size: "<< operand_stack.size() <<", ";
                        #endif
                        operatorNode = operand_stack.top();
                        #ifdef STACK_OUTPUT
                            std::cout << "Operator Node: " << (char)operatorNode->value <<", ";
                        #endif
                        operand_stack.pop();
                        op1Node = operand_stack.top();
                        #ifdef STACK_OUTPUT
                            std::cout << "op1 Node: " << op1Node->value <<", ";
                        #endif
                        operand_stack.pop();
                        op2Node = newNode(buildOperand);
                        #ifdef STACK_OUTPUT
                            std::cout << "op2 Node: "<< op2Node->value <<"\n";
                        #endif
                        #ifdef STACK_OUTPUT
                            std::cout << "Stack size: "<< operand_stack.size() <<"\n ";
                        #endif
                        //we now have handle of all 3 nodes for valid expression, create tree 
                        operatorNode->right = op1Node;
                        operatorNode->left = op2Node;

                        //add local tree to stack 
                        operand_stack.push(operatorNode);
                        state = 0;
                    }
                    else if (state == 0){
                        state = 1;
                        op1Node = newNode(buildOperand);
                        #ifdef STACK_OUTPUT
                            std::cout << "op1 Node state 0: " << op1Node->value <<"\n";
                        #endif
                        operand_stack.push(op1Node);
                    }
                    //after finding the first operand, we should find an operator before 2nd operand

                   
               }
               if((expression[i] == operatorChars[0] || expression[i] == operatorChars[1] ||
                  expression[i] == operatorChars[2] || expression[i] == operatorChars[3]) && state == 1){
                      //we found an operator, pop the two operands on stack and build expression tree
                      state = 2;
                    #ifdef STACK_OUTPUT
                        std::cout << "stack size : "<< operand_stack.size() <<"\n";
                    #endif
                    operatorNode = newNode(expression[i]); //create node with value of operator
                    operand_stack.push(operatorNode); //char pushed
                    //get the values of the other operator nodes from the stack
                    // rightNode = operand_stack.top(); //get operand 2 for right node
                    // operand_stack.pop(); //remove from stack
                    // leftNode = operand_stack.top(); //get operand 1 for left node
                    // operand_stack.pop(); //remove from stack


                  }
               
           }

            rootNode = operand_stack.top();
            operand_stack.pop();
            op1Node = rootNode->right;
            op2Node = rootNode->left;
            
            #ifdef EVAL_OUTPUT
                std::cout << "rootNode value "<<(char)rootNode->value<<" \n";
                std::cout << "op1 value "<<op1Node->value<<" \n";
                std::cout << "op2 value "<<op2Node->value<<" \n";
            #endif

            double result  = performOperation(op1Node->value,op2Node->value,(char)rootNode->value);
            std::cout << "RESULT: "<<result<<"\n";
            
        }
        

    }

    

    public:

    int performOperation(int operandOne, int operandTwo, char operatorSelect){
        int result = 0;
        switch(operatorSelect){
            case '+':
                result = (operandOne + operandTwo);
            break;
            case '*':
                result = (operandOne * operandTwo);
            break;
            case '-':
                result = (operandOne - operandTwo);
            break;
            case '/':
                result = (operandOne / operandTwo);
            break;
        }

        return result;
        
    }

    double performOperation(double operandOne, double operandTwo, char operatorSelect){
        double result = 0;
        switch(operatorSelect){
            case '+':
                result = (operandOne + operandTwo);
            break;
            case '*':
                result = (operandOne * operandTwo);
            break;
            case '-':
                result = (operandOne - operandTwo);
            break;
            case '/':
                result = (operandOne / operandTwo);
            break;
        }
        return result;
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
