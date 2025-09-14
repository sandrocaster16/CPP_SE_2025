#include <fstream>
#include <iostream>

struct S{
    char* _input = NULL;            // файл для чтения
    char* _output = NULL;           // файл для записи
    bool _print = false;            // вывод в cmd
    unsigned int _stats = 10;       // n частых запросов
    unsigned int _window = 0;       //непон
    unsigned int _from = 0;         //
    unsigned int _to = 4294967295;  //

    bool _error_arg = false;     // если есть ошибка в аргументах
};


// swap
void swap(int& a,int& b){
    int temp = a;
    a = b;
    b = temp;
}

// длина строки
int len_char(char* q){
    int len = 0;
    while(*q != '\0'){
        q++;
        len++;
    }
    return len;
}

// перевод чаров в инт
int char_to_int(char* q,int start=0,int end=0){
    int u=0,n=1;
    for(int i=end-1;i>start-1;i--){
        u += (q[i]-'0') * n;
        n*=10;
    }
    return u;
}

// является ли char числом
bool found_int(char* q,int start=0,int end=0){
    for(int i=start;i<end;i++){
        if(!(q[i]-'0' >= 0 && q[i]-'0' <= 9)){
            return false;
        }
    }
    return true;
}



// обрезание лога на время и ошибку
void cut_log(char* q, int& _timestamp, int& _error){
    int ii=1;

    while(q[ii-1] != '['){
        ii++;
    }

    // 01/Jul/1995:00:00:13 -0400
    // ^ii
    // day/month/year:hour:minute:second -4 hour

    // пример/тест
    // 01/Jul/1995:00:01:55 -0400 =>    804542515

    char month[12][4] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    int month_day[21] = {31,28,31,30,31,30,31,31,30,31,30,31},
        _month,
        _days = (char_to_int(q,ii+7,ii+10)-1970) * 365 + 
                (char_to_int(q,ii+7,ii+10)-1968) / 4;
                
    for(int i=0;i<12;i++){
        if(q[ii+3] == month[i][0] && q[ii+4] == month[i][1] && q[ii+5] == month[i][2]){
            _month = i;
        }
    }
    for(int i=0; i<_month; i++){
        _days += month_day[i];
    }
    
    _timestamp  =   86400 *     _days +
                    3600 *      char_to_int(q,ii+12,ii+13) +
                    60 *        char_to_int(q,ii+15,ii+16) +
                                char_to_int(q,ii+18,ii+19)
                    - 14400; // -4 часа пояс

    ii = 254;
    while(q[ii+1] != ' '){
        ii--;
    }
    if(q[ii] != '-'){
        _error = char_to_int(q,ii-2,ii+1);
    }
}

//анализ логов
void analyze_logs(S& argss){
    // открытие файлов
    std::ifstream file_read(argss._input);
    std::ofstream file_write(argss._output);

    if(!file_read.is_open()){
        std::cerr<<"Error: file is dont open";
        return;
    }

    char q;
    int ii=0, _error, _timestamp, stats_error[100], stats_error_1[100];
    char qwe[256];

    for(int i=0; i<100; i++){
        stats_error[i] = 0;
    }

    while(file_read.get(q)){
        qwe[ii] = q;
        ii++;

        if(q == '\n'){
            // строчка (log) записана в qwe
            cut_log(qwe, _timestamp, _error); //функция обработки лога до ошибки и времени
        
            // начало анализа по аргументам

            if((_error >= 500 && _error <= 599) && (_timestamp >= argss._from && _timestamp <= argss._to)){
                // --stats
                if(argss._stats != 0){
                    stats_error[_error-500]++;
                }

                // --print
                if(argss._print){
                    std::cout<<qwe;
                }

                file_write << qwe; //запись
            }


            //вывод тест
            /*
            for(int i=0; i<ii; i++){
                std::cout<<qwe[i];
            }
            std::cout<<_error<<' '<<_timestamp<<'\n';
            std::cout<<'\n';
            */

            // анулирование qwe и ii
            ii=0;
            for(int i=0; i<256; i++){
                qwe[i] = '\0';
            }
        }
        
    }

    // --stats
    if(argss._stats != 0){
        for(int i=0; i<100; i++){
            stats_error_1[i] = i+500;
        }

        // сортировка
        for(int i=1;i<100;i++){
            for(int j=i;j>0;j--){
                if(stats_error[j]>stats_error[j-1]){
                    swap(stats_error[j],stats_error[j-1]);
                    swap(stats_error_1[j],stats_error_1[j-1]);
                }
                else{
                    break;
                }
            }
        }

        // вывод
        std::cout<<"--stats:\n";
        for(int i=0; i<argss._stats; i++){
            std::cout<<" - "<<i+1<<") "<<stats_error_1[i]<<" -> "<<stats_error[i]<<'\n';
        }
    }

    // закрытие файлов
    file_read.close();
    file_write.close();
}



// функция для int аргументов
void check_func_int(unsigned int& u,bool& err,char* arg,char* prov,char* arg_next=nullptr,int* ii=nullptr){
    int arg_lenght=len_char(arg), prov_lenght=len_char(prov);
    
    if(prov_lenght <= arg_lenght){
        for(int i=0; i<prov_lenght; i++){
            if(arg[i] != prov[i]){
                break;
            }
            if(i==prov_lenght-1 && arg[i+1] != '='){
                // если все верно
                char* _a;
                int _b, _c;
                if(arg[i] != '='){
                    _a = arg_next;
                    _b = 0;
                    _c = len_char(arg_next);
                }
                else{
                    _a = arg;
                    _b = prov_lenght;
                    _c = arg_lenght;
                }

                // проверка на число
                if(found_int(_a,_b,_c)){
                    u = char_to_int(_a,_b,_c);
                    if(arg_next!=nullptr){
                        *ii+=1;
                    }
                    err = false;
                }
            }
        }
    }
}

// функция для output file
void check_func_output(char*& u,bool& err,char* arg,char* prov,char* arg_next=nullptr,int* ii=nullptr){
    int arg_lenght=len_char(arg), prov_lenght=len_char(prov);
    
    if(prov_lenght <= arg_lenght){
        for(int i=0; i<prov_lenght; i++){
            if(arg[i] != prov[i]){
                break;
            }
            if(i==prov_lenght-1){
                // если все верно
                if(arg[i] == '='){
                    // char y[256];
                    // for(int j=0; j<256; j++){
                    //     y[j] = '\0';
                    // }
                    // for(int j=0; j<arg_lenght-prov_lenght; j++){
                    //     y[j] = arg[prov_lenght+j];
                    // }
                    // std::cout<<"== "<<y<<'\n';
                    // u = y;
                    // std::cout<<"== "<<u<<'\n';

                    u = "result.txt";
                    // тут баг
                }
                else{
                    u = arg_next;
                    *ii+=1;
                }
            }
        }
    }
}

// функция для print
void check_func_print(bool& u,bool& err,char* arg,char* prov){
    int arg_lenght=len_char(arg), prov_lenght=len_char(prov);
    
    if(prov_lenght <= arg_lenght){
        for(int i=0; i<prov_lenght; i++){
            if(arg[i] != prov[i]){
                break;
            }
            if(i==prov_lenght-1){
                // если все верно
                u = true;
                err = false;
            }
        }
    }
}

// функция для всех аргументов 
void check_args(S& argss,char** argv,int argc){
    for(int i = 1;i<argc;i++){

        bool wrong_arg = true; // переменная для отсева фигни

        // длинные int аргументы с =
        check_func_int(argss._stats,  wrong_arg, argv[i],"--stats=");
        check_func_int(argss._window, wrong_arg, argv[i],"--window=");
        check_func_int(argss._from,   wrong_arg, argv[i],"--from=");
        check_func_int(argss._to,     wrong_arg, argv[i],"--to=");

        // длинные int аргументы без =
        check_func_int(argss._stats,  wrong_arg, argv[i],"--stats",  argv[i+1], &i);
        check_func_int(argss._window, wrong_arg, argv[i],"--window", argv[i+1], &i);
        check_func_int(argss._from,   wrong_arg, argv[i],"--from",   argv[i+1], &i);
        check_func_int(argss._to,     wrong_arg, argv[i],"--to",     argv[i+1], &i);
        
        // короткие int аргументы
        check_func_int(argss._stats,  wrong_arg, argv[i],"-s", argv[i+1], &i);
        check_func_int(argss._window, wrong_arg, argv[i],"-w", argv[i+1], &i);
        check_func_int(argss._from,   wrong_arg, argv[i],"-f", argv[i+1], &i);
        check_func_int(argss._to,     wrong_arg, argv[i],"-e", argv[i+1], &i);

        // input
        int y = len_char(argv[i]);
        if(argv[i][y-4] == '.' && argv[i][y-3] == 'l' && argv[i][y-2] == 'o' && argv[i][y-1] == 'g'){
            argss._input = argv[i];
        }

        // output
        check_func_output(argss._output, wrong_arg, argv[i],"--output=");
        // check_func_output(argss._output, wrong_arg, argv[i],"--output", argv[i+1], &i);
        check_func_output(argss._output, wrong_arg, argv[i],"-o",       argv[i+1], &i);

        // print
        check_func_print(argss._print, wrong_arg, argv[i],"--print");
        check_func_print(argss._print, wrong_arg, argv[i],"-p");
    }
}



int main(int argc, char** argv){
    S argss;
    check_args(argss,argv,argc);

    // если ошибки в аргументах
    if(argss._input == NULL){
        std::cerr<<"Error: log file not found";
    }
    else if(argss._output == NULL){
        std::cerr<<"Error: output file not found";
    }
    else if(argss._error_arg){
        std::cerr<<"Error: wrong arguments";
    }
    else{
        // прога
        std::cout<<"All done\n";
        analyze_logs(argss);
    }

    //test
    std::cout<<'\n'<<argss._input<<' '<<argss._output<<' '<<argss._print<<" "<<argss._stats<<" "<<argss._window<<" "<<argss._from<<" "<<argss._to<<std::endl;
}