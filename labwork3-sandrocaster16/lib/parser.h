#pragma once

class parser{
private:
    const char* main_long_args[4] = {"input","output","max-iter","freq"};
    const char main_short_args[4] = {'i','o','m','f'};

public:
    char* _input;         // файл для чтения с начальными данными
    char* _output;        // директория для выгрузки картинок
    uint64_t _max_iter;    // максимальное количество итераций модели
    uint64_t _freq;        // частота, с которой должны сохранятся картинки

    bool _error_arg;      // если есть ошибка в аргументах

    parser() : _input(nullptr), _output(nullptr), _max_iter(10000000), _freq(0), _error_arg(false) {}

    void parse(int argc, char** argv){
        for(int i=1; i<argc; i++){
            int _len = len_char(argv[i]);

            if(argv[i][0] == '-'){
                if(argv[i][1] == '-'){

                    int y = -1;
                    for(int k=0; k<4; k++){
                        bool f = true;
                        for(int j=0; j<len_char(main_long_args[k]); j++){
                            if(main_long_args[k][j] != argv[i][j+2]){
                                f = false;
                                break;
                            }
                        }

                        if(f){
                            y = k;
                            break;
                        }
                    }

                    if(y == -1){
                        _error_arg = true;
                    }
                    else if(y == 0){
                        _input = argv[i]+8;
                    }
                    else if(y == 1){
                        _output = argv[i]+9;
                    }
                    else if(y == 2){
                        if(found_int(argv[i], 11, _len)){
                            _max_iter = char_to_int(argv[i], 11, _len);
                        }
                        else{
                            std::cerr<<"Error: No number found in argument --max-iter\n";
                        }
                    }
                    else if(y == 3){
                        if(found_int(argv[i], 7, _len)){
                            _freq = char_to_int(argv[i], 7, _len);
                        }
                        else{
                            std::cerr<<"Error: No number found in argument --freq\n";
                        }
                    }

                }
                else{

                    int y = -1;
                    for(int k=0; k<4; k++){
                        if(main_short_args[k] == argv[i][1]){
                            y = k;
                            break;
                        }
                    }

                    i++;

                    if(y == -1){
                        _error_arg = true;
                    }
                    else if(y == 0){
                        _input = argv[i];
                    }
                    else if(y == 1){
                        _output = argv[i];
                    }
                    else if(y == 2){
                        if(found_int(argv[i])){
                            _max_iter = char_to_int(argv[i]);
                        }
                        else{
                            std::cerr<<"Error: No number found in argument -m\n";
                        }
                    }
                    else if(y == 3){
                        if(found_int(argv[i])){
                            _freq = char_to_int(argv[i]);
                        }
                        else{
                            std::cerr<<"Error: No number found in argument -f\n";
                        }
                    }
                }
            }
            else{
                _error_arg = true;
                // std::cout<<"Непонятный аргумент "<<argv[i]<<'\n';
            }
        }
    }

    void print_all_var(){
        std::cout   <<"input: "<<_input<<'\n'
                    <<"output: "<<_output<<'\n'
                    <<"max-iter: "<<_max_iter<<'\n'
                    <<"freq: "<<_freq<<'\n';
    }
};
