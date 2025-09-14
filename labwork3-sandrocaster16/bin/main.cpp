#include <iostream>

#include "../lib/support_functions.h"
#include "../lib/files_worker.h"
#include "../lib/parser.h"
#include "../lib/game.h"

const char* help_description(){
    return "help: read README.rd pls";
}

int main(int argc, char** argv){
    parser my_parser;
    files_worker my_files;
    game my_game;

    my_parser.parse(argc, argv);

    // если ошибки в аргументах
    if(my_parser._error_arg){
        std::cerr<<"Error: wrong arguments\n";
        std::cerr<<help_description()<<'\n';
    }
    else if(my_parser._input == nullptr){
        std::cerr<<"Error: wrong input file\n";
        std::cerr<<help_description()<<'\n';
    }
    else if(my_parser._output == nullptr){
        std::cerr<<"Error: direction doesnt exist\n";
        std::cerr<<help_description()<<'\n';
    }
    else{
        std::cout<<"All done\n";

        my_game.play(my_parser, my_files);
    }
 
    return 0;
}