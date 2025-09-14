#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio> // для std::remove
#include <ctime>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Traveler{
private:
    std::string from_where;
    std::string to_where;
    std::string date;
    std::string megamegasecretAPI;

    std::vector<std::pair<std::string, std::string>> citys_code;
    std::vector<std::pair<std::string, std::string>> alphabet;

    std::string tolower(const std::string& str);

    bool townToCode(bool to);

    void parse_search(const json& search);

    void parse_segment(const json& segment);

    void parse_json(const json& j);


public:
    Traveler() : 
        from_where(""), 
        to_where(""),
        date(""),
        megamegasecretAPI("")
    {
        std::ifstream file("lib/codes.tsv");
        if(!file.is_open()){
            std::cerr<<"Ошибка: файл codes.tsv не удалось открыть!\n";
            exit(1);
        }
        else{
            std::string line;
            while(std::getline(file, line)){
                std::istringstream iss(line);
                std::string city_name, city_code;
                
                if(std::getline(iss, city_name, '\t') && std::getline(iss, city_code, '\t')){
                    citys_code.push_back({city_name, city_code});
                }
            }
        }
        file.close();

        file.open("lib/alphabet.tsv"); 
        if(!file.is_open()){
            std::cerr<<"Ошибка: файл alphabet.tsv не удалось открыть!\n";
            exit(1);
        }
        else{
            std::string line;
            while(std::getline(file, line)){
                std::istringstream iss(line);
                std::string high_letter, short_letter;
                
                if(std::getline(iss, high_letter, '\t') && std::getline(iss, short_letter, '\t')){
                    alphabet.push_back({high_letter, short_letter});
                }
            }
        }
        file.close();

        file.open("lib/megamegasecretAPI.txt"); 
        if(!file.is_open()){
            std::cerr<<"Ошибка: файл megamegasecretAPI.txt не удалось открыть!\n";
            exit(1);
        }
        else{
            std::getline(file, megamegasecretAPI);

            std::cout<<"Ваш API: "<<megamegasecretAPI<<'\n';
        }
        file.close();
    }

    void start();

    void start(char** argv, int argc);
};