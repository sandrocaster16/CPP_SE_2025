#include "console.hpp"

std::string Console::GetConsole(){
    std::cout << "> ";
    std::getline(std::cin, line);
    return line;
}

void Console::SendConsole(std::string message){
    std::cout<<message<<'\n';
}

void Console::SendConsole(uint64_t message){
    std::cout<<message<<'\n';
}

void Console::SendError(std::string error_message){
    std::cerr<<"Error: "+error_message+'\n';
}