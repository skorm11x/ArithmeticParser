/*
 *********************************************************************
 *                         FileIO.cpp                                *
 * Explanation: Open specified csv file, obtain expression from line *
 * Author: Christopher J. Kosik  16-Apr-2019 (current)               *          
 * See included Readme.md file for details.                          *
 *********************************************************************
*/

#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#include "AParser.h"
 int processFile(char *);

class FileIO{
    std::string fileInput;
    
    
    public:

        /* 
         * Open the file, check if it matches file extension properties, and then read line by line delimmiting
         * expressions on comma to feed into Expression Evaluator. 
         */ 
        int processFile(char *filename){
            std::ifstream infile(filename);
            std::size_t found = std::string(filename).find(FILE_EXT);
            if(found == std::string::npos){
                if(infile.is_open()){
                    //file is open, read line by line for evaluation
                    //processFile(infile);
                    for(std::string lineData; getline(infile, lineData);){
                        //received line must be comma delimmited and expression sent to ExpressionEval for evaluation
                        std::cout <<"Line data: "<<lineData<<" \n";
                        //take vector approach for delimiting file
                        
                    }
                }
                else{
                    std::cout << filename<<" File could not be located! Please reload.\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if(!found){
                std::cout << filename<<" File is not csv extension.\n";
                exit(EXIT_FAILURE);
            }

            return 0;
        }
};
