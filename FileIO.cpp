/*
 *********************************************************************
 *                         FileIO.cpp                                *
 * Explanation: Open specified csv file, obtain expression from line *
 * Author: Christopher J. Kosik  1-May-2020 (current)               *          
 * See included Readme.md file for details.                          *
 *********************************************************************
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono> 
#include "ExpressionEval.cpp"
#include "AParser.h"

class FileIO{
    std::string fileInput;
    
    
    public:

        /* 
         * Open the file, check if it matches file extension properties, and then read line by line delimmiting
         * expressions on comma to feed into Expression Evaluator. 
         */ 
        int processFile(char *filename){
            ExpressionEval eval;
            std::ifstream infile(filename);
            std::size_t found = std::string(filename).find(FILE_EXT);
            if(found != std::string::npos){
                if(infile.is_open()){
                    //file is open, read line by line for evaluation
                    //processFile(infile);
                    std::cout << "Processing file...\n";
                    auto start = std::chrono::high_resolution_clock::now();
                    for(std::string lineData; getline(infile, lineData);){
                        //received line must be comma delimmited and expression sent to ExpressionEval for evaluation
                        //std::cout <<"File Line data: "<<lineData<<" \n";
                        std::stringstream ss(lineData);
                        while(ss.good()){
                            std::vector<std::string> result;
                            std::string expression;
                            getline(ss, expression,',');
                            result.push_back(expression);
                            eval.receiveFileExpression(expression);
                        }
                
                        
                    }
                    auto stop = std::chrono::high_resolution_clock::now(); 
                    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
                    #ifdef TIME_CLI_OUTPUT
                        std::cout<< "Total File processing time take: "<<duration.count()<<" uS \n";
                    #endif
                    std::cout << "File processing complete for "<<filename<<"\n";
                }
                else{
                    std::cout << filename<<" File could not be located! Please reload.\n";
                    exit(EXIT_FAILURE);
                }
            }
            else if(found == std::string::npos){
                std::cout << filename<<" File is not csv extension.\n";
                exit(EXIT_FAILURE);
            }

            return 0;
        }
};
