//
// Created by Ben Baysinger  on 1/11/24.
//
#define CATCH_CONFIG_RUNNER
#include "cmdline.hpp"
#include <iostream>



void use_arguments( int argc, char **argv) {
    bool hasSeen = false;

    for( int i = 1; i < argc; i++ ) {
        if (std::strcmp(argv[i], "--help") ==0) {
            std::cout <<"Arguments allowed are:\n " << "--help: shows allowed arguments with explanation of what they do\n"
            << "--test: checks if test has passed \n";
            exit(0);
        }
        else if ( std::strcmp(argv[i], "--test") ==0) {

            if (!hasSeen) {
                if ( Catch::Session().run(1, argv) != 0) {
                    exit(1);
                }
                hasSeen = true;
            }
            else {
                std::cerr << "Error test has already been seen \n";
                exit(1);
            }
        }
        else{
            std::cerr<<"Invalid argument \n";
            exit(1);
        }
    }
}

