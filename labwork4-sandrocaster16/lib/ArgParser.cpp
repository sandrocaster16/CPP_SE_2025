#include "ArgParser.h"

//addstring
ArgumentParser::StringArg& ArgumentParser::ArgParser::AddStringArgument(const std::string& long_name){
    Args_.push_back(new StringArg(long_name));
    return *dynamic_cast<StringArg*>(Args_[Args_.size() - 1]);
}

ArgumentParser::StringArg& ArgumentParser::ArgParser::AddStringArgument(const std::string& long_name, const std::string& description){
    Args_.push_back(new StringArg(long_name, description));
    return *dynamic_cast<StringArg*>(Args_[Args_.size() - 1]);
}

ArgumentParser::StringArg& ArgumentParser::ArgParser::AddStringArgument(const char& short_name, const std::string& long_name){
    Args_.push_back(new StringArg(short_name, long_name));
    return *dynamic_cast<StringArg*>(Args_[Args_.size() - 1]);
}

ArgumentParser::StringArg& ArgumentParser::ArgParser::AddStringArgument(const char& short_name, const std::string& long_name, const std::string& description){
    Args_.push_back(new StringArg(short_name, long_name, description));
    return *dynamic_cast<StringArg*>(Args_[Args_.size() - 1]);
}



//addint
ArgumentParser::IntArg& ArgumentParser::ArgParser::AddIntArgument(const std::string& long_name){
    Args_.push_back(new IntArg(long_name));
    return *dynamic_cast<IntArg*>(Args_[Args_.size() - 1]);
}

ArgumentParser::IntArg& ArgumentParser::ArgParser::AddIntArgument(const std::string& long_name, const std::string& description){
    Args_.push_back(new IntArg(long_name, description));
    return *dynamic_cast<IntArg*>(Args_[Args_.size() - 1]);
}

ArgumentParser::IntArg& ArgumentParser::ArgParser::AddIntArgument(const char& short_name, const std::string& long_name){
    Args_.push_back(new IntArg(short_name, long_name));
    return *dynamic_cast<IntArg*>(Args_[Args_.size() - 1]);
}

ArgumentParser::IntArg& ArgumentParser::ArgParser::AddIntArgument(const char& short_name, const std::string& long_name, const std::string& description){
    Args_.push_back(new IntArg(short_name, long_name, description));
    return *dynamic_cast<IntArg*>(Args_[Args_.size() - 1]);
}


//addbool
ArgumentParser::BoolArg& ArgumentParser::ArgParser::AddFlag(const std::string& long_name){
    Flags_.push_back(new BoolArg(long_name));
    return *dynamic_cast<BoolArg*>(Flags_[Flags_.size() - 1]);
}

ArgumentParser::BoolArg& ArgumentParser::ArgParser::AddFlag(const std::string& long_name, const std::string& description){
    Flags_.push_back(new BoolArg(long_name, description));
    return *dynamic_cast<BoolArg*>(Flags_[Flags_.size() - 1]);
}

ArgumentParser::BoolArg& ArgumentParser::ArgParser::AddFlag(const char& short_name, const std::string& long_name){
    Flags_.push_back(new BoolArg(short_name, long_name));
    return *dynamic_cast<BoolArg*>(Flags_[Flags_.size() - 1]);
}

ArgumentParser::BoolArg& ArgumentParser::ArgParser::AddFlag(const char& short_name, const std::string& long_name, const std::string& description){
    Flags_.push_back(new BoolArg(short_name, long_name, description));
    return *dynamic_cast<BoolArg*>(Flags_[Flags_.size() - 1]);
}


//string functions
ArgumentParser::StringArg& ArgumentParser::StringArg::Default(const std::string& data){
    default_ = new std::string(data);
    return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::StoreValue(std::string& value){
    store_value_ = &value;
    return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::StoreValues(std::vector<std::string>& value){
    store_value_vec_ = &value;
    return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::MultiValue(){
    if(data_){
        delete data_;
    }
    data_ = nullptr;

    multi_values_ = new std::vector<std::string>;

    return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::MultiValue(const size_t& size){
    if(data_){
        delete data_;
    }
    data_ = nullptr;

    multi_values_ = new std::vector<std::string>(size);
    multi_values_size_ = size;

    return *this;
}

ArgumentParser::StringArg& ArgumentParser::StringArg::Positional(){
    positional_ = true;
    return *this;
}

const std::string& ArgumentParser::StringArg::GetData(const int& index = -1){
    if(index == -1){
        if(data_){
            return *data_;
        }
        else if(default_){
            return *default_;
        }
        else{
            std::cerr<<"Error: data not founding\n";
            throw;
        }
    }
    else{
        if(multi_values_){
            if(index < multi_values_->size()){
                return (*multi_values_)[index];
            }
            else{
                std::cerr<<"Error: index is big\n";
                throw;
            }
        }
        else{
            std::cerr<<"Error: data not founding\n";
            throw;
        }
    }
}

ArgumentParser::StringArg& ArgumentParser::StringArg::SetData(const std::string& data){
    data_ = new std::string(data);
    if(store_value_){
        *store_value_ = data;
    }
    return *this;
}

std::string ArgumentParser::StringArg::debug(){
    std::string out;
    
    if(data_){
        out += *data_+' ';
    }
    else{
        out += "nonexistent ";
    }

    if(default_){
        out += *default_+' ';
    }
    else{
        out += "nonexistent ";
    }

    if(store_value_){
        out += "exist ";
    }
    else{
        out += "nonexistent ";
    }

    if(store_value_vec_){
        out += "exist ";
    }
    else{
        out += "nonexistent ";
    }

    if(multi_values_){
        for(std::string q : *multi_values_){
            out += q+' ';
        }
    }
    else{
        out += "nonexistent ";
    }

    out += std::to_string(multi_values_size_);

    return out;
}


//int functions
ArgumentParser::IntArg& ArgumentParser::IntArg::Default(const int& data){
    default_ = new int(data);
    return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::StoreValue(int& value){
    store_value_ = &value;
    return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::StoreValues(std::vector<int>& value){
    store_value_vec_ = &value;
    return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::MultiValue(){
    if(data_){
        delete data_;
    }
    data_ = nullptr;

    multi_values_ = new std::vector<int>;

    return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::MultiValue(const size_t& size){
    if(data_){
        delete data_;
    }
    data_ = nullptr;

    multi_values_ = new std::vector<int>;
    multi_values_size_ = size;

    return *this;
}

ArgumentParser::IntArg& ArgumentParser::IntArg::Positional(){
    positional_ = true;
    return *this;
}

const int& ArgumentParser::IntArg::GetData(const int& index = -1){
    if(index == -1){
        if(data_){
            return *data_;
        }
        else if(default_){
            return *default_;
        }
        else{
            std::cerr<<"Error: data not founding\n";
            throw;
        }
    }
    else{
        if(multi_values_){
            if(index < multi_values_->size()){
                return (*multi_values_)[index];
            }
            else{
                std::cerr<<"Error: index is big\n";
                throw;
            }
        }
        else{
            std::cerr<<"Error: data not founding\n";
            throw;
        }
    }
}

ArgumentParser::IntArg& ArgumentParser::IntArg::SetData(const int& data){
    data_ = new int(data);
    if(store_value_){
        *store_value_ = data;
    }
    return *this;
}

std::string ArgumentParser::IntArg::debug(){
    std::string out;
    
    if(data_){
        out += std::to_string(*data_)+' ';
    }
    else{
        out += "nonexistent ";
    }

    if(default_){
        out += std::to_string(*default_)+' ';
    }
    else{
        out += "nonexistent ";
    }

    if(store_value_){
        out += "exist ";
    }
    else{
        out += "nonexistent ";
    }

    if(store_value_vec_){
        out += "exist ";
    }
    else{
        out += "nonexistent ";
    }

    if(multi_values_){
        for(int q : *multi_values_){
            out += std::to_string(q)+' ';
        }
    }
    else{
        out += "nonexistent ";
    }

    out += std::to_string(multi_values_size_);

    return out;
}


//flag functions
ArgumentParser::BoolArg& ArgumentParser::BoolArg::Default(const bool& data){
    data_ = data;
    if(store_value_){
        *store_value_ = data;
    }
    return *this;
}

ArgumentParser::BoolArg& ArgumentParser::BoolArg::StoreValue(bool& value){
    store_value_ = &value;
    return *this;
}

ArgumentParser::BoolArg& ArgumentParser::BoolArg::Positional(){
    positional_ = true;
    return *this;
}
                       
const bool& ArgumentParser::BoolArg::GetData(){
    return data_;
}

std::string ArgumentParser::BoolArg::debug(){
    std::string out;
    
    out += std::to_string(data_)+' ';

    if(store_value_){
        out += "exist ";
    }
    else{
        out += "nonexistent ";
    }

    return out;
}


// other functions
bool ArgumentParser::ArgParser::Parse(std::vector<std::string> argv){
    Argument* temp_arg = nullptr;
    bool f_arg; //флаг аргументов
    
    std::string consol_arg;
    std::string arg_after_equal;
    size_t pos_equal;
    int i_next = 0;

    for(int i=1; i<argv.size(); ++i){
        // проверка на занятость temp_arg
        if(i_next != i){
            temp_arg = nullptr;
        }

        f_arg = true;
        consol_arg = argv[i];

        // если есть '='
        pos_equal = consol_arg.find('=');
        if(pos_equal != std::string::npos){
            std::string t = consol_arg;
            consol_arg = t.substr(0, pos_equal);
            arg_after_equal = t.substr(pos_equal + 1);
        }

        if(consol_arg[0] == '-' && consol_arg.size() != 1){
            consol_arg = consol_arg.substr(1);

            //if long_name
            if(consol_arg[0] == '-'  && consol_arg.size() != 1){
                consol_arg = consol_arg.substr(1);

                for(int j=0; j<Args_.size(); ++j){
                    if(Args_[j]->long_name_ && consol_arg == *Args_[j]->long_name_){
                        temp_arg = Args_[j];
                        f_arg = false;
                        if(pos_equal == std::string::npos){
                            i_next = i+1;
                        }
                        break;
                    }
                }

                for(int j=0; j<Flags_.size(); ++j){
                    if(Flags_[j]->long_name_ && consol_arg == *Flags_[j]->long_name_){
                        BoolArg* Arg = dynamic_cast<BoolArg*>(Flags_[j]);
                        Arg->data_ = true;
                        if(Arg->store_value_){
                            *Arg->store_value_ = true;
                        }
                        f_arg = false;
                        break;
                    }
                }

                // --help
                if(Help_ && consol_arg == *Help_->long_name_){
                    Help_->positional_ = true;
                    f_arg = false;
                    break;
                }
            }

            //if short_name
            for(int j=0; j<Args_.size(); ++j){
                if(Args_[j]->short_name_ && consol_arg == std::string(1, *Args_[j]->short_name_)){
                    temp_arg = Args_[j];
                    f_arg = false;
                    if(pos_equal == std::string::npos){
                        i_next = i+1;
                    }
                    break;
                }
            }

            if(f_arg){
                bool f;
                for(char ch : consol_arg){
                    f = true;
                    for(int j=0; j<Flags_.size(); ++j){
                        if(Flags_[j]->short_name_ && ch == *Flags_[j]->short_name_){
                            BoolArg* Arg = dynamic_cast<BoolArg*>(Flags_[j]);
                            Arg->data_ = true;
                            if(Arg->store_value_){
                                *Arg->store_value_ = true;
                            }
                            f_arg = false;
                            f = false;
                            break;
                        }
                    }
                    if(f){
                        return false;
                    }
                }
            }

            // -h
            if(Help_ && consol_arg == std::string(1, *Help_->short_name_)){
                Help_->positional_ = true;
                f_arg = false;
                break;
            }
        }

        // поиск positional
        if(!temp_arg && f_arg){
            for(int j=0; j<Args_.size(); ++j){
                //string
                if(Args_[j]->type_ == STRINGARG){
                    StringArg* Arg = dynamic_cast<StringArg*>(Args_[j]);
                    if(Arg->positional_){
                        temp_arg = Arg;
                        break;
                    }
                }
                //int
                else if(Args_[j]->type_ == INTARG){
                    IntArg* Arg = dynamic_cast<IntArg*>(Args_[j]);
                    if(Arg->positional_){
                        temp_arg = Arg;
                        break;
                    }
                }
            }
        }

        // если мне некуда закидывать
        if(!temp_arg && f_arg){
            std::cerr<<"Error: temp not founding\n";
            return false;
        }

        if(pos_equal != std::string::npos){
            consol_arg = arg_after_equal;
            f_arg = true;
        }

        // если аргумент не оказался флагом
        if(f_arg){
            //string
            if(temp_arg->type_ == STRINGARG){
                StringArg* Arg = dynamic_cast<StringArg*>(temp_arg);

                if(!Arg->multi_values_){
                    delete Arg->data_;
                    Arg->data_ = new std::string(consol_arg);
                    if(Arg->store_value_){
                        *Arg->store_value_ = consol_arg;
                    }
                }
                else{
                    Arg->multi_values_->push_back(consol_arg);
                    if(Arg->store_value_vec_){
                        Arg->store_value_vec_->push_back(consol_arg);
                    }
                }
            }
            //int
            else if(temp_arg->type_ == INTARG){
                IntArg* Arg = dynamic_cast<IntArg*>(temp_arg);
                int consol_arg_int = std::stoi(consol_arg);
            
                if(!Arg->multi_values_){
                    delete Arg->data_;
                    Arg->data_ = new int(consol_arg_int);
                    if(Arg->store_value_){
                        *Arg->store_value_ = consol_arg_int;
                    }
                }
                else{
                    Arg->multi_values_->push_back(consol_arg_int);
                    if(Arg->store_value_vec_){
                        Arg->store_value_vec_->push_back(consol_arg_int);
                    }
                }
            }
        }
    }

    if(Help()){
        return true;
    }

    // проверки
    for(int j=0; j<Args_.size(); ++j){
        //string
        if(Args_[j]->type_ == STRINGARG){
            StringArg* Arg = dynamic_cast<StringArg*>(Args_[j]);
            if(Arg->multi_values_size_ != -1 && Arg->multi_values_size_ > Arg->multi_values_->size()){
                return false;
            }
            if(!Arg->data_ && !Arg->default_ && !Arg->multi_values_){
                return false;
            }
        }
        //int
        else if(Args_[j]->type_ == INTARG){
            IntArg* Arg = dynamic_cast<IntArg*>(Args_[j]);
            if(Arg->multi_values_size_ != -1 && Arg->multi_values_size_ > Arg->multi_values_->size()){
                return false;
            }
            if(!Arg->data_ && !Arg->default_ && !Arg->multi_values_){
                return false;
            }
        }
    }
    
    return true;
}

bool ArgumentParser::ArgParser::Parse(int argc, char** argv){
    std::vector<std::string> args;

    for(int i=0; i<argc; ++i){
        args.push_back(argv[i]);
    }

    return Parse(args);
}

void ArgumentParser::ArgParser::debugger(){
    std::cout<<"Program name: "<<program_name_<<"\n\n";

    std::cout<<"TYPE short_name long_name description positional data default store_value_ store_value_vec_ multi_values multi_values_size\n";
    std::cout<<"size Args_: "<<Args_.size()<<'\n';

    for(int i=0; i<Args_.size(); ++i){
        std::cout<< Args_[i]->type_         <<' '
                <<  (Args_[i]->short_name_ ? Args_[i]->short_name_ : "nonexistent") <<' '
                <<  (Args_[i]->long_name_ ? *Args_[i]->long_name_ : "nonexistent")   <<' '
                <<  (Args_[i]->description_ ? *Args_[i]->description_ : "nonexistent")  <<' '
                <<  Args_[i]->positional_   <<' ';

        if(Args_[i]->type_ == STRINGARG){
            StringArg* Arg = dynamic_cast<StringArg*>(Args_[i]);
            std::cout<< Arg->debug() <<'\n';
        }
        else if(Args_[i]->type_ == INTARG){
            IntArg* Arg = dynamic_cast<IntArg*>(Args_[i]);
            std::cout<< Arg->debug() <<'\n';
        }
    }

    std::cout<<"\nsize Flag_: "<<Flags_.size()<<'\n';
    for(int i=0; i<Flags_.size(); ++i){
        std::cout<< Flags_[i]->type_         <<' '
                <<  (Flags_[i]->short_name_ ? Flags_[i]->short_name_ : "nonexistent") <<' '
                <<  (Flags_[i]->long_name_ ? *Flags_[i]->long_name_ : "nonexistent")   <<' '
                <<  (Flags_[i]->description_ ? *Flags_[i]->description_ : "nonexistent")  <<' '
                <<  Flags_[i]->positional_   <<' ';

        BoolArg* Arg = dynamic_cast<BoolArg*>(Flags_[i]);
        std::cout<< Arg->debug() <<'\n';
    }
}

const std::string& ArgumentParser::ArgParser::GetStringValue(const std::string& long_name){
    for(int j=0; j<Args_.size(); ++j){
        if(Args_[j]->long_name_ && Args_[j]->type_ == STRINGARG && long_name == *Args_[j]->long_name_){
            StringArg* Arg = dynamic_cast<StringArg*>(Args_[j]);
            return Arg->GetData();
        }
    }
    return nullptr;
}

const std::string& ArgumentParser::ArgParser::GetStringValue(const std::string& long_name, const int& index){
    for(int j=0; j<Args_.size(); ++j){
        if(Args_[j]->long_name_ && Args_[j]->type_ == STRINGARG && long_name == *Args_[j]->long_name_){
            StringArg* Arg = dynamic_cast<StringArg*>(Args_[j]);
            return Arg->GetData(index);
        }
    }
    return nullptr;
}

const int& ArgumentParser::ArgParser::GetIntValue(const std::string& long_name){
    for(int j=0; j<Args_.size(); ++j){
        if(Args_[j]->long_name_ && Args_[j]->type_ == INTARG && long_name == *Args_[j]->long_name_){
            IntArg* Arg = dynamic_cast<IntArg*>(Args_[j]);
            return Arg->GetData();
        }
    }
    return 0;
}

const int& ArgumentParser::ArgParser::GetIntValue(const std::string& long_name, const int& index){
    for(int j=0; j<Args_.size(); ++j){
        if(Args_[j]->long_name_ && Args_[j]->type_ == INTARG && long_name == *Args_[j]->long_name_){
            IntArg* Arg = dynamic_cast<IntArg*>(Args_[j]);
            return Arg->GetData(index);
        }
    }
    return 0;
}
    
const bool& ArgumentParser::ArgParser::GetFlag(const char& short_name){
    for(int j=0; j<Flags_.size(); ++j){
        if(Flags_[j]->short_name_ && short_name == *Flags_[j]->short_name_){
            BoolArg* Arg = dynamic_cast<BoolArg*>(Flags_[j]);
            return Arg->GetData();
        }
    }
    return 0;
}

const bool& ArgumentParser::ArgParser::GetFlag(const std::string& long_name){
    for(int j=0; j<Flags_.size(); ++j){
        if(Flags_[j]->long_name_ && long_name == *Flags_[j]->long_name_){
            BoolArg* Arg = dynamic_cast<BoolArg*>(Flags_[j]);
            return Arg->GetData();
        }
    }
    return 0;
}

void ArgumentParser::ArgParser::AddHelp(const char& short_name, const std::string& long_name, const std::string& description){
    Help_ = new Argument(HELPARG, short_name, long_name, description);
}

bool ArgumentParser::ArgParser::Help(){
    if(Help_){
        return Help_->positional_;
    }
    return false;
}

std::string ArgumentParser::ArgParser::HelpDescription(){
    std::string out = "test help";
    return out;
}