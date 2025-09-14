#include "./Traveler.hpp"

std::string Traveler::tolower(const std::string& str){
    std::string result;
    std::string q;
    bool f;

    for(int i=0; i<str.size(); ){
        int charLen = 1;
        if ((str[i] & 0b11000000) == 0b11000000) {
            charLen = 2;
        } else if ((str[i] & 0b11100000) == 0b11100000) {
            charLen = 3;
        } else if ((str[i] & 0b11110000) == 0b11110000) {
            charLen = 4;
        }

        q = str.substr(i, charLen);
        i += charLen;
        f = true;

        for(const auto& letter : alphabet){
            if(q == letter.first){
                result += letter.second;
                f = false;
                break;
            }
        }
        if(f){
            result += q;
        }
    }

    return result;
}  

bool Traveler::townToCode(bool to){
    if(to){

        for(int i=0; i<citys_code.size(); ++i){
            if(citys_code[i].first == tolower(to_where)){
                to_where = citys_code[i].second;
                std::cout<<to_where<<'\n';
                return true;
            }
        }

    }
    else{

        for(int i=0; i<citys_code.size(); ++i){
            if(citys_code[i].first == tolower(from_where)){
                from_where = citys_code[i].second;
                std::cout<<from_where<<'\n';
                return true;
            }
        }

    }

    return false;
}

void Traveler::parse_search(const json& search) {
    std::cout << "Отправление из: " << search["from"]["title"] << " (" << search["from"]["code"] << ")\n";
    std::cout << "Прибытие в: " << search["to"]["title"] << " (" << search["to"]["code"] << ")\n";
    std::cout << "Дата: " << search["date"] << "\n";
}

void Traveler::parse_segment(const json& segment) {
    std::cout << "Маршрут: " << segment["thread"]["title"] << "\n";
    std::cout << "Отправление: " << segment["from"]["title"] << " (" << segment["from"]["code"] << ")\n";
    std::cout << "Прибытие: " << segment["to"]["title"] << " (" << segment["to"]["code"] << ")\n";
    std::cout << "Время отправления: " << segment["departure"] << "\n";
    std::cout << "Время прибытия: " << segment["arrival"] << "\n";
    std::cout << "Длительность: " << segment["duration"] << " секунд\n";
    std::cout << "Есть пересадки: " << (segment["has_transfers"] ? "Да" : "Нет") << "\n";

    // Парсим информацию о билетах, если она есть
    std::cout << "-------------------------\n";
}

void Traveler::parse_json(const json& j) {
    try {
        // Парсим информацию о поиске
        parse_search(j["search"]);

        // Парсим сегменты маршрутов
        for (const auto& segment : j["segments"]) {
            // Проверяем количество пересадок
            if (segment.contains("transfers") && segment["transfers"].size() > 1) {
                std::cout << "Маршрут пропущен: количество пересадок больше 1.\n";
                continue; // Пропускаем маршрут, если пересадок больше 1
            }

            // Парсим сегмент, если пересадок не больше 1
            parse_segment(segment);
        }

        // Парсим информацию о пагинации
        std::cout << "Всего сегментов: " << j["pagination"]["total"] << "\n";
        std::cout << "Лимит: " << j["pagination"]["limit"] << "\n";
        std::cout << "Смещение: " << j["pagination"]["offset"] << "\n";

    } catch (const std::exception& e) {
        return;
    }
}

void Traveler::start(){
    std::cout<<"Traveler приветствует вас!\n";
    std::cout<<"Для выхода введите \"exit\"\n";

    while(true){
        while(true){
            std::cout<<"Введите город откуда вы хотите уехать:\n";
            std::cin>>from_where;
            if(from_where == "exit"){
                std::cout<<"Завершение программы. До свидания!\n";
                return;
            }

            if(townToCode(false)){
                std::cout<<"Хорошо\n";
                break;
            }
            else{
                std::cout<<"Неверное название города. Попробуйте ещё раз.\n";
            }
        }

        while(true){
            std::cout<<"Введите город куда вы хотите поехать:\n";
            std::cin>>to_where;
            if(to_where == "exit"){
                std::cout<<"Завершение программы. До свидания!\n";
                return;
            }

            if(townToCode(true)){
                std::cout<<"Хорошо\n";
                break;
            }
            else{
                std::cout<<"Неверное название города. Попробуйте ещё раз.\n";
            }
        }

        std::cout<<"Введите дату в формате (YYYY-MM-DD):\n";
        std::cin>>date;
        if(date == "exit"){
            std::cout<<"Завершение программы. До свидания!\n";
            return;
        }
        std::cout<<"Хорошо\n";

        //code
        std::ifstream time_live_read("lib/time_live.tsv");
        std::vector<std::string> lines;
        if(time_live_read.is_open()){
            std::string line;
            while(std::getline(time_live_read, line)){
                std::istringstream iss(line);
                std::string city_from_, city_to_, date_, time_;
                
                if(std::getline(iss, city_from_, '\t') && std::getline(iss, city_to_, '\t') && std::getline(iss, date_, '\t') && std::getline(iss, time_, '\t')){
                    // std::cout<<"time: "<<std::time(nullptr)-std::stoi(time_)<<'\n';
                    if(std::time(nullptr)-std::stoi(time_) >= 3600){
                        //delete file
                        std::filesystem::remove("history/"+city_from_+"-"+city_to_+"-"+date_+".json");
                    }
                    else{
                        lines.push_back(line);
                    }
                }
            }
        }
        time_live_read.close();

        //перезаписываем без удаляемых строчек
        std::ofstream time_live_write("lib/time_live.tsv");
        for(std::string line : lines){
            time_live_write << line << '\n';
        }
        time_live_write.close();


        json json_request;
        std::ifstream history_file("history/"+from_where+"-"+to_where+"-"+date+".json");
        if(!history_file.is_open()){
            std::cout<<"Делается запрос\n";
            cpr::Response travel = cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/?apikey="+megamegasecretAPI+"&format=json&from="+from_where+"&to="+to_where+"&lang=ru_RU&date="+date+"&transfers=true"});
            json_request = travel.text;

            //сохраняю history
            std::ofstream history_file_write("history/"+from_where+"-"+to_where+"-"+date+".json");
            history_file_write<<travel.text;
            history_file_write.close();
            
            //сохраняю время запроса
            std::ofstream time_live("lib/time_live.tsv", std::ios::app);
            time_live << from_where<<'\t'<<to_where<<'\t'<<date<<'\t'<<std::time(nullptr)<<'\n';
            time_live.close();

            history_file.open("history/"+from_where+"-"+to_where+"-"+date+".json");
        }
        else{
            std::cout<<"Такой запрос уже был. Читаю файл в истории\n";
        }
        history_file >> json_request;

        parse_json(json_request);
        // parse_segments(json_request);
    }
}

void Traveler::start(char** argv, int argc){
    if(argc != 4){
        std::cerr << "Ошибкас\n";
        return;
    }

    std::cout<<"Traveler приветствует вас!\n";

    from_where = argv[1];
    if(!townToCode(false)){
        std::cout<<"Неверное название города. Попробуйте ещё раз.\n";
        return;
    }

    to_where = argv[2];
    if(!townToCode(true)){
        std::cout<<"Неверное название города. Попробуйте ещё раз.\n";
        return;
    }

    date = argv[3];

    //code
    std::ifstream time_live_read("lib/time_live.tsv");
    std::vector<std::string> lines;
    if(time_live_read.is_open()){
        std::string line;
        while(std::getline(time_live_read, line)){
            std::istringstream iss(line);
            std::string city_from_, city_to_, date_, time_;
            
            if(std::getline(iss, city_from_, '\t') && std::getline(iss, city_to_, '\t') && std::getline(iss, date_, '\t') && std::getline(iss, time_, '\t')){
                // std::cout<<"time: "<<std::time(nullptr)-std::stoi(time_)<<'\n';
                if(std::time(nullptr)-std::stoi(time_) >= 3600){
                    //delete file
                    std::filesystem::remove("history/"+city_from_+"-"+city_to_+"-"+date_+".json");
                }
                else{
                    lines.push_back(line);
                }
            }
        }
    }
    time_live_read.close();

    //перезаписываем без удаляемых строчек
    std::ofstream time_live_write("lib/time_live.tsv");
    for(std::string line : lines){
        time_live_write << line << '\n';
    }
    time_live_write.close();


    json json_request;
    std::ifstream history_file("history/"+from_where+"-"+to_where+"-"+date+".json");
    if(!history_file.is_open()){
        std::cout<<"Делается запрос\n";
        cpr::Response travel = cpr::Get(cpr::Url{"https://api.rasp.yandex.net/v3.0/search/?apikey="+megamegasecretAPI+"&format=json&from="+from_where+"&to="+to_where+"&lang=ru_RU&date="+date+"&transfers=true"});
        json_request = travel.text;

        //сохраняю history
        std::ofstream history_file_write("history/"+from_where+"-"+to_where+"-"+date+".json");
        history_file_write<<travel.text;
        history_file_write.close();
        
        //сохраняю время запроса
        std::ofstream time_live("lib/time_live.tsv", std::ios::app);
        time_live << from_where<<'\t'<<to_where<<'\t'<<date<<'\t'<<std::time(nullptr)<<'\n';
        time_live.close();

        history_file.open("history/"+from_where+"-"+to_where+"-"+date+".json");
    }
    else{
        std::cout<<"Такой запрос уже был. Читаю файл в истории\n";
    }
    history_file >> json_request;

    parse_json(json_request);
    // parse_segments(json_request);
}