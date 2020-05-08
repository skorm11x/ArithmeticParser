/*
 *****************************************************************
 *                         Main.cpp                              *
 * Explanation: launch program in selected mode of operation     *
 * Author: Christopher J. Kosik  1-May-2020 (current)           *          
 * See included Readme.md file for details.                      *
 *****************************************************************
*/

#include <iostream>
#include <cstdlib> // for EXIT_SUCCESS and EXIT_FAILURE status codes
#include <string>
// #include "ExpressionEval.cpp"
#include "FileIO.cpp"
#include <stdlib.h>
#include "AParser.h"



/*
 * Entry point of c++ application, receive cli arguments to select mode of operation 
 */ 
int main(int argc, char *argv[]){
    ExpressionEval eval;
    FileIO fileio;
    CliDialog dialog;

    char * filename; //capute the filename passed as cli arg

    if(argc >= 2) {
        //filename is second argument in char array
        filename = argv[1];
        dialog.informUserFileMode(filename);
        if(fileio.processFile(filename)){
            //in future asks for another input/ help user/ file selector
            std::cout <<"Program currently only supports csv files! (soft check): Please re-run with csv test file."<<"\n";
            exit(0);
        }
    }
    else if(argc < 2){
        //only command argument was passed to shell. Start program in traditional mode
        eval.receiveCliExpression(); //ask for expression
    }
    return 0;
}

