#pragma once

#include <iostream>
#include <string>
#include <vector>
// #include <utility>
// #include <limits>

namespace ArgumentParser {

enum Type{
    STRINGARG,
    INTARG,
    BOOLARG,
    HELPARG
};

class Argument{
public:
    Type type_;

    char*        short_name_;
    std::string* long_name_;
    std::string* description_;

    bool positional_;
    
    Argument(Type type, const std::string& long_name) : 
        type_(type),
        short_name_(nullptr), 
        long_name_(new std::string(long_name)), 
        description_(nullptr),
        positional_(false) 
    {}

    Argument(Type type, const std::string& long_name, const std::string& description) : 
        type_(type),
        short_name_(nullptr), 
        long_name_(new std::string(long_name)), 
        description_(new std::string(description)),
        positional_(false) 
    {}

    Argument(Type type, const char& short_name, const std::string& long_name) : 
        type_(type),
        short_name_(new char(short_name)), 
        long_name_(new std::string(long_name)), 
        description_(nullptr),
        positional_(false) 
    {}

    Argument(Type type, const char& short_name, const std::string& long_name, const std::string& description) : 
        type_(type),
        short_name_(new char(short_name)), 
        long_name_(new std::string(long_name)), 
        description_(new std::string(description)),
        positional_(false) 
    {}

    virtual ~Argument(){
        delete short_name_;
        delete long_name_;
        delete description_;
    }
};


class StringArg : public Argument{
public:
    std::string* data_;
    std::string* default_;

    std::string* store_value_;
    std::vector<std::string>* store_value_vec_;

    std::vector<std::string>* multi_values_;
    int multi_values_size_;

    StringArg(const std::string& long_name) :
        Argument(STRINGARG, long_name),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    StringArg(const std::string& long_name, const std::string& description) :
        Argument(STRINGARG, long_name, description),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    StringArg(const char& short_name, const std::string& long_name) :
        Argument(STRINGARG, short_name, long_name),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    StringArg(const char& short_name, const std::string& long_name, const std::string& description) :
        Argument(STRINGARG, short_name, long_name, description),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    ~StringArg(){
        delete data_;
        delete default_;
        delete store_value_;
        delete store_value_vec_;
        delete multi_values_;
    }

    StringArg& Default(const std::string& data);            //true
    StringArg& StoreValue(std::string& value);              //true
    StringArg& StoreValues(std::vector<std::string>& value); //false     //true
    StringArg& MultiValue();                                //true
    StringArg& MultiValue(const size_t& size);              //true
    StringArg& Positional();                                //true

    const std::string& GetData(const int& index);
    StringArg& SetData(const std::string& data);
    std::string debug();
};

class IntArg : public Argument{
public:
    int* data_;
    int* default_;

    int* store_value_;
    std::vector<int>* store_value_vec_;

    std::vector<int>* multi_values_;
    int multi_values_size_;

    IntArg(const std::string& long_name) :
        Argument(INTARG, long_name),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    IntArg(const std::string& long_name, const std::string& description) :
        Argument(INTARG, long_name, description),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    IntArg(const char& short_name, const std::string& long_name) :
        Argument(INTARG, short_name, long_name),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    IntArg(const char& short_name, const std::string& long_name, const std::string& description) :
        Argument(INTARG, short_name, long_name, description),
        data_(nullptr),
        default_(nullptr),
        store_value_(nullptr),
        store_value_vec_(nullptr),
        multi_values_(nullptr),
        multi_values_size_(-1)
    {}

    ~IntArg(){
        delete data_;
        delete default_;
        delete store_value_;
        delete store_value_vec_;
        delete multi_values_;
    }

    IntArg& Default(const int& data);            //true
    IntArg& StoreValue(int& value);              //true
    IntArg& StoreValues(std::vector<int>& value); //true
    IntArg& MultiValue();                        //true
    IntArg& MultiValue(const size_t& size);      //true
    IntArg& Positional();                        //true

    const int& GetData(const int& index);
    IntArg& SetData(const int& data);
    std::string debug();
};

class BoolArg : public Argument {
public:
    bool data_;
    bool* store_value_;

    BoolArg(const std::string& long_name) :
        Argument(BOOLARG, long_name),
        data_(false),
        store_value_(nullptr)
    {}

    BoolArg(const std::string& long_name, const std::string& description) :
        Argument(BOOLARG, long_name, description),
        data_(false),
        store_value_(nullptr)
    {}

    BoolArg(const char& short_name, const std::string& long_name) :
        Argument(BOOLARG, short_name, long_name),
        data_(false),
        store_value_(nullptr)
    {}

    BoolArg(const char& short_name, const std::string& long_name, const std::string& description) :
        Argument(BOOLARG, short_name, long_name, description),
        data_(false),
        store_value_(nullptr)
    {}

    ~BoolArg(){
        delete store_value_;
    }

    BoolArg& Default(const bool& data);            //falsae
    BoolArg& StoreValue(bool& value);              //false
    BoolArg& Positional();                        //false

    const bool& GetData();
    std::string debug();
};


class ArgParser {
private:
    std::vector<Argument*> Args_;
    std::vector<Argument*> Flags_;

    Argument* Help_;

    std::string program_name_;

public:
    ArgParser(std::string name): Help_(nullptr), program_name_(name) {};

    StringArg& AddStringArgument(const std::string& long_name);
    StringArg& AddStringArgument(const std::string& long_name, const std::string& description);
    StringArg& AddStringArgument(const char& short_name, const std::string& long_name);
    StringArg& AddStringArgument(const char& short_name, const std::string& long_name, const std::string& description);

    IntArg& AddIntArgument(const std::string& long_name);
    IntArg& AddIntArgument(const std::string& long_name, const std::string& description);
    IntArg& AddIntArgument(const char& short_name, const std::string& long_name);
    IntArg& AddIntArgument(const char& short_name, const std::string& long_name, const std::string& description);
   
    BoolArg& AddFlag(const std::string& long_name);
    BoolArg& AddFlag(const std::string& long_name, const std::string& description);
    BoolArg& AddFlag(const char& short_name, const std::string& long_name);
    BoolArg& AddFlag(const char& short_name, const std::string& long_name, const std::string& description);

    void AddHelp(const char& short_name, const std::string& long_name, const std::string& description);
    bool Help();
    std::string HelpDescription();

    const std::string& GetStringValue(const std::string& long_name);
    const std::string& GetStringValue(const std::string& long_name, const int& index);

    const int& GetIntValue(const std::string& long_name);
    const int& GetIntValue(const std::string& long_name, const int& index);

    const bool& GetFlag(const char& short_name);
    const bool& GetFlag(const std::string& long_name);

    bool Parse(std::vector<std::string>);
    bool Parse(int argc, char** argv);

    void debugger();
};
  // cmake --build . --target argparser_tests && ctest -V
} // namespace ArgumentParser