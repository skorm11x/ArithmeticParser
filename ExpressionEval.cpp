/*
 ********************************************************************
 *                         ExpressionEval.cpp                       *
 * Explanation: sanitize, parse, build expression tree, and compute *
 * Author: Christopher J. Kosik  1-May-2020 (current)              *          
 * See included Readme.md file for details.                         *
 *********************************************************** *********
*/

#include <iostream>
#include <stdio.h>
#include <string>
#include <cctype>
#include <bits/stdc++.h> 
#include <math.h>
#include <chrono>  
#include <cmath>
#include "CliDialog.cpp"
#include "AParser.h"

class ExpressionEval{
    CliDialog dialog;

    // An expression tree node 
    struct expressTree 
    { 
        double value; //either operator or numeric value
        bool isLeaf;  //only values can be leaf nodes! 
        expressTree* left, *right;  //left and right pointers to nodes, null for leaf..
    }; 

    char validChars[23] = {'+','*','-','/','(',')','{','}','[',']','0','1','2','3','4','5','6','7','8','9','.','^'};
    char priorityChars[6] = {'(',')','{','}','[',']'};
    char operatorChars[5] = {'+','-','*','/','^'}; 

    //global flag for valid or invalid expression, updated or not during parsing
    int isValid = 1; //expression valid by default

    //Knew I wanted to build some tree and keep local evaluations built in a stack
    //Source of part of idea: https://www.geeksforgeeks.org/expression-tree/

    // A utility function to create a new node 
    expressTree* newNode(double v) 
    { 
        expressTree *temp = new expressTree; 
        temp->left = temp->right = NULL; 
        temp->isLeaf = false; 
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
                            else if (expression[i-1] == operatorChars[4] || expression[i+1] == operatorChars[4]){
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
        expressTree *op1Node, *op2Node, *operatorNode, *rootNode, *addNode; //right: 1, left: 2
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

                Priorities:
                4: priorityChars[6] = {'(',')','{','}','[',']'};
                3: ^ (exponetioation)
                2: * ,/,(mod)( multiply or modulo/divide, though currently unsupported)
                1: +, -

                Concept: we painstakingly go through each expression and find our priorities,
                where we want the highest priority statements to be all the way at the bottom of our tree to be evaluated
                first by our inorder traversal and evaluate

                So, we must find all rank 5 priority statements and place at bottom, and then 4 and so on.  
            */

           int inPriority = 0;// flag to indicate if we are currently inside priority (,{,[
           int parathesisCnt = 0;
           int state = 0; //track num of operand/operator
            std::cout<<"Expression to evaluate: "<<expression<<"\n";
            //add all remaining parts
            for(int i = 0; i< expression.length(); ++i){
                   for(int j = 0; j<(sizeof(priorityChars)/sizeof(priorityChars[0])); ++j){
                        if(expression[i] == priorityChars[j]){
                            //we found a bracket, parenthesis, brace etc.

                            //because we are reading from left to right we expect (require) the 
                            //priority indicator to be an open: (, {, [
                            //valid characters and operators but incorrect priority sequence otherwise
                            if(expression[i] == '{' || expression[i] == '(' || expression[i] == '['){
                                //Ignore everything after, incrementing count for every new one encountered
                                ++parathesisCnt;
                                //look for (parathesisCnt # of }, ), or ] to "escape"
                                //ex (2+23) : 1
                                //((2+2)*5), ((2+2)1*5) where the second one has implicit multiplication!!
                                //for purposes of program, ((2+2)1*5) is considered ILLEGAL. all operators must be explicit!
                                //e.g ((2+2)*5) is valid and has 2 items where we always expect operators between nested
                                //parenthesis (2+2+(3*4)) (redundant/ useless parenthesis but still)

                                                        

                            }
                            else{
                                --parathesisCnt;
                                //we encountered the end of parenthesis, we need to add this **valid** expression
                                // to our tree
                            }


                        }
                   }
            if(parathesisCnt <1){
                //if count is greater than 1 we are inside a parenth, ignore.
                //character is not a priority indicator (parenthesis)/ operator
                if(isdigit(expression[i]) || expression[i] == '.'||
                    !(expression[i] == operatorChars[0] || expression[i] == operatorChars[1] ||
                    expression[i] == operatorChars[2] || expression[i] == operatorChars[3] ||
                    expression[i] == operatorChars[4]))
                    {
                    //the character is not an operator, so we need to build the operand 
                    double buildOperand = 0;
                    int decimalFlag  = 0; //flag set for handling values after decimal place
                    double valueToAdd = 0; // hold a value that will divided by each time
                    double divisorValue = 1;// hold a value that will divided by each time

                    //move the character forward to capture all the numbers to create operand
                    while(i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')){
                        //**ONLY ACCURATE DOWN TO 1000TH PLACE DECIMAL**
                        /*
                        just ignore comma's although input may be input that way
                        decimals will indicate building double value
                        because this is in base 10 (assumption) every value entered will be multiple of 10 difference/ shift
                        */
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
                            /*
                                If we are in state 2, then we have 2 items in our stack, which due to sanitization,
                                must be operand and operator.
                                Thus, update the left and right children for tree creation
                            */
                            #ifdef STACK_OUTPUT
                                std::cout << "Op2 check Stack size: "<< operand_stack.size() <<", ";
                            #endif
                            operatorNode = operand_stack.top(); //the last value pushed was an operator
                            #ifdef STACK_OUTPUT
                                std::cout << (char)operatorNode->value <<", ";
                            #endif
                            operand_stack.pop();
                            op1Node = operand_stack.top(); //the second value pushed was an operand
                            //op1Node->isLeaf = true;
                            if(op1Node->value == operatorChars[0] || op1Node->value == operatorChars[1] ||
                                op1Node->value == operatorChars[2] || op1Node->value == operatorChars[3] ||
                                op1Node->value == operatorChars[4]){
                                    //we MUST check whether the left or the right are * to set isLeaf
                                    op1Node->isLeaf = false;
                                }
                            else{
                                    op1Node->isLeaf = true;
                            }
                            #ifdef STACK_OUTPUT
                                std::cout << op1Node->value <<", ";
                            #endif
                            operand_stack.pop();
                            op2Node = newNode(buildOperand); //capture the current operand
                            //op2Node->isLeaf = true;
                            if(op2Node->value == operatorChars[0] || op2Node->value == operatorChars[1] ||
                                op2Node->value == operatorChars[2] || op2Node->value == operatorChars[3] ||
                                op2Node->value == operatorChars[4]){
                                    //we MUST check whether the left or the right are * to set isLeaf
                                    op2Node->isLeaf = false;
                                }
                            else{
                                    op2Node->isLeaf = true;
                            }
                            #ifdef STACK_OUTPUT
                                std::cout << op2Node->value <<"\n";
                            #endif
                            #ifdef STACK_OUTPUT
                                std::cout << "Stack size: "<< operand_stack.size() <<"\n ";
                            #endif
                            //we now have handle of all 3 nodes for valid expression, create tree 
                            operatorNode->left = op1Node;
                            operatorNode->right = op2Node;
                            operatorNode->isLeaf = false;

                            //add local tree to stack 
                            operand_stack.push(operatorNode);
                            state = 0;
                        }
                        else if (state == 0){
                            //we are either building the first binary expression e.g. 1+1, or we have encountered
                            //another operator and need to "add onto" the exisiting tree 
                            // 1+1+1: 
                            //       1
                            //    +     1
                            // 1     1
                            //
                                op1Node = newNode(buildOperand);
                                #ifdef STACK_OUTPUT
                                    std::cout << "op1 Node state 0: " << op1Node->value <<"\n";
                                #endif
                                operand_stack.push(op1Node);
                                state = 1;
                        }
                        //after finding the first operand, we should find an operator before 2nd operand

                    
                }
                if(state == 1 || state == 0){
                    /*
                        * First case: we found an operator following an operand and we are 
                        * building a non chained expression e.g. 1+1
                        */ 
                        if(expression[i] == operatorChars[0] || expression[i] == operatorChars[1] ||
                        expression[i] == operatorChars[2] || expression[i] == operatorChars[3] ||
                        expression[i] == operatorChars[4]){
                                state = 2; //look for an operand
                                #ifdef STACK_OUTPUT
                                    std::cout << "stack size : "<< operand_stack.size() <<"\n";
                                #endif
                                operatorNode = newNode(expression[i]); //create node with value of operator
                                operand_stack.push(operatorNode); //char pushed

                        }


                    }

            }       
                
            }
            if(parathesisCnt >0){
                //format error/ mismatched paraenth, throw error and exit. Annoying but necessary.
                std::cout<<"Critical error. Mismatch on number of parenthesis/brackets remaining: "<<parathesisCnt<<"\n";
                exit(0);
            }
            rootNode = operand_stack.top();
            operand_stack.pop();
            inorderEvaluate(rootNode, 3); //pop the pointer to the root node
            inorderEvaluate(rootNode, 2);
            inorderEvaluate(rootNode, 1);
        }
        

    }

    /**
     * Function receives pointer to root node.
     */ 
    void inorderEvaluate(expressTree * rootNode, int currPriority){
        //Each time we evaluate a node pair:
        //    +
        //  1   1
        // We must update the root node value, e.g. new tree should be
        //
        //    2
        // DC   DC
        // Where DC stands for don't care, or we should NULL out
        //
        //As we traverse in order, we are looking for priority order of ops, and performing them first and modifying
        // values. 
        //


        if(rootNode != NULL){

            //Inorder traverse all the way to the bottom of the left child of tree
            inorderEvaluate(rootNode->left, currPriority);
            #ifdef EVAL_OUTPUT
                std::cout<<"Node val: "<<rootNode->value<<"\n";
                std::cout<<"IsLeaf? : "<<rootNode->isLeaf<<"\n";
                if(rootNode->left != NULL){
                    std::cout<<"LEFT: "<<rootNode->left->value<<"\n";
                }
                else if(rootNode->left == NULL){
                    std::cout<<"LEFT is null\n";
                }
                if(rootNode->right != NULL){
                    std::cout<<"RIGHT: "<<rootNode->right->value<<"\n";
                }
                else if(rootNode->right == NULL){
                    std::cout<<"RIGHT is null\n";
                } 
            #endif
            //we are at the bottom of Binary tree, start checking operators
            if(!rootNode->isLeaf){
                if(currPriority == 3){
                    //handle exponents
                    if(rootNode->value == '^'){
                        if(rootNode->left->value == operatorChars[0] || rootNode->left->value == operatorChars[1] ||
                        rootNode->left->value == operatorChars[2] || rootNode->left->value == operatorChars[3] ||
                        rootNode->left->value == operatorChars[4]){
                            if(rootNode->left->right == NULL){
                                //we already used value, go further down left
                                //TODO: change the way tree is built / fix the depth problem of the tree!!
                                //only one step to solve "simple" inputs...
                                double op1Node = rootNode->left->left->value;
                                double op2Node = rootNode->right->value;
                                double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                                //std::cout << "RESULT: "<<result<<"\n";
                                std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                                rootNode->left->left->value = result;   
                            }else{
                                double op1Node = rootNode->left->right->value;
                                double op2Node = rootNode->right->value;
                                double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                                //std::cout << "RESULT: "<<result<<"\n";
                                std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                                rootNode->left->right->value = result;
                                rootNode->right = NULL; 
                            }  
                        }
                        else{
                            double op1Node = rootNode->left->value;
                            double op2Node = rootNode->right->value;
                            double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                            //std::cout << "RESULT: "<<result<<"\n";
                            std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                            rootNode->value = result;
                        }
                    }
                }
                if(currPriority == 2){
                    //handle mult, div, modulo
                    if(rootNode->value == '*' || rootNode->value == '/'){
                        if(rootNode->left->value == operatorChars[0] || rootNode->left->value == operatorChars[1] ||
                        rootNode->left->value == operatorChars[2] || rootNode->left->value == operatorChars[3] ||
                        rootNode->left->value == operatorChars[4]){
                            if(rootNode->left->right == NULL){
                                //we already used value, go further down left
                                //TODO: change the way tree is built / fix the depth problem of the tree!!
                                //only one step to solve "simple" inputs...
                                double op1Node = rootNode->left->left->value;
                                double op2Node = rootNode->right->value;
                                double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                                //std::cout << "RESULT: "<<result<<"\n";
                                std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                                rootNode->left->left->value = result;

                            }else{
                                double op1Node = rootNode->left->right->value;
                                double op2Node = rootNode->right->value;
                                double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                                //std::cout << "RESULT: "<<result<<"\n";
                                std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                                rootNode->left->right->value = result; 
                                rootNode->right = NULL; 
                            } 
                        }
                        else{
                            double op1Node = rootNode->left->value;
                            double op2Node = rootNode->right->value;
                            double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                            //std::cout << "RESULT: "<<result<<"\n";
                            std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                            rootNode->value = result;
                        }
                    }
                }
                if(currPriority == 1){
                    //handle +, -
                    if(rootNode->value == '+' || rootNode->value == '-'){
                        if(rootNode->left->value == operatorChars[0] || rootNode->left->value == operatorChars[1] ||
                        rootNode->left->value == operatorChars[2] || rootNode->left->value == operatorChars[3] ||
                        rootNode->left->value == operatorChars[4]){
                            if(rootNode->left->right == NULL){
                                //we already used value, go further down left
                                //TODO: change the way tree is built / fix the depth problem of the tree!!
                                //only one step to solve "simple" inputs...
                                double op1Node = rootNode->left->left->value;
                                double op2Node = rootNode->right->value;
                                double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                                //std::cout << "RESULT: "<<result<<"\n";
                                std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                                rootNode->left->left->value = result; 

                            }else{
                                double op1Node = rootNode->left->right->value;
                                double op2Node = rootNode->right->value;
                                double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                                //std::cout << "RESULT: "<<result<<"\n";
                                std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                                rootNode->left->right->value = result;
                                rootNode->right = NULL;  
                            }  
                        }
                        else{
                            double op1Node = rootNode->left->value;
                            double op2Node = rootNode->right->value;
                            double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                            //std::cout << "RESULT: "<<result<<"\n";
                            std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                            rootNode->value = result;
                        }
                    }
                }

                
                // double op1Node = rootNode->left->value;
                // double op2Node = rootNode->right->value;
                // double result  = performOperation(op1Node,op2Node,(char)rootNode->value);
                // //std::cout << "RESULT: "<<result<<"\n";
                // std::cout << "Expresion step:"<<op1Node<<" "<<(char)rootNode->value<<" "<<op2Node<<" RESULT: "<<result<<"\n";
                // rootNode->value = result;
            }
            inorderEvaluate(rootNode->right, currPriority);
            //we have reached the end of inorder traversal.
        };

    }

    

    public:

    /**
     * Group of overloaded functions containing, and supporting, all of our operators
     */ 
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
            case '^':
                result = (pow(operandOne,operandTwo));    
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
            case '^':
                result = (pow(operandOne,operandTwo));  
            break;
        }
        return result;
    }

    long double performOperation(long double operandOne, long double operandTwo, char operatorSelect){
        long double result = 0;
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
            case '^':
                result = (pow(operandOne,operandTwo));      
            break;
        }
        return result;
    }

    /*
    *  Function receive input expression and calls class functions
    *  to determine expression validity, parse expression, and calculate result
    */ 
    void receiveCliExpression(){
        std::string expression;
        do{
            expression = dialog.askForExpression(isValid);
            parseExpression(expression);

        }while(!isValid);   
        
    }

    /**
     * Function receives line by line from csv file to sanitize and then parse.
     * 
     */ 
    void receiveFileExpression(std::string expression){
        auto start = std::chrono::high_resolution_clock::now(); 
        parseExpression(expression);
        auto stop = std::chrono::high_resolution_clock::now(); 
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        #ifdef TIME_CLI_OUTPUT
            std::cout<< "Time take single expression: "<<duration.count()<<" uS \n";
        #endif
    }
};
