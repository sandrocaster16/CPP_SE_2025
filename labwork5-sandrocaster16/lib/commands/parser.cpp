#include "parser.hpp"

Parser::Parser(Console& console, LogicCommands& LogicCommands)
    : console_(&console)
    , LogicCommands_(&LogicCommands)
    , argument(nullptr)
{}

Parser::~Parser() {
    if (argument) delete argument;
}

bool Parser::isNumber(const std::string& str) {
    if (str.empty()) return false;

    size_t start = 0;
    bool hasDecimal = false;

    for (size_t i = start; i < str.size(); ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }

    return true;
}

std::vector<std::string> Parser::cut_line(const std::string& line) {
    std::vector<std::string> result;
    std::string arg;

    for (char q : line) {
        if (q == ' ') {
            if (arg.size() != 0) {
                result.push_back(arg);
                arg.clear();
            }
        } else {
            arg += q;
        }
    }
    if (line.size() != 0) {
        result.push_back(arg);
    }

    return result;
}

void Parser::parse(const std::string& line) {
    argument = new Arguments;
    std::vector<std::string> args = cut_line(line);

    if (args.size() == 0) {
        argument->error_ = true;
    } else if (args[0] == "ping") {
        argument->com_ = ping;
    } else if (args[0] == "exit") {
        argument->com_ = exitt;
    } else if (args[0] == "create" && args.size() == 2) {
        argument->com_ = create;
        argument->string_args_.push_back(args[1]);
    } else if (args[0] == "start") {
        argument->com_ = start;
    } else if (args[0] == "stop") {
        argument->com_ = stop;
    } else if (args[0] == "set" && args.size() > 2) {
        argument->com_ = set;
        argument->string_args_.push_back(args[1]);

        if (args[1] == "count" && (args[2] >= "1" && args[2] <= "4") && isNumber(args[3])) {
            argument->int_args_.push_back(std::stoi(args[2]));
            argument->int_args_.push_back(std::stoi(args[3]));
        } else if ((args[1] == "height" || args[1] == "width") && isNumber(args[2])) {
            argument->int_args_.push_back(std::stoi(args[2]));
        } else if (args[1] == "strategy" || args[1] == "result") {
            argument->string_args_.push_back(args[2]);
        } else {
            argument->error_ = true;
        }
    } else if (args[0] == "get" && args.size() > 1) {
        argument->com_ = get;
        argument->string_args_.push_back(args[1]);

        if (args[1] == "count" && (args[2] >= "1" && args[2] <= "4")) {
            argument->int_args_.push_back(std::stoi(args[2]));
        } else if (args[1] != "height" && args[1] != "width") {
            argument->error_ = true;
        }
    } else if (args[0] == "shot") {
        if (args.size() == 1) {
            argument->com_ = shot;
        } else if (args.size() == 3) {
            argument->com_ = shot;
            argument->int_args_.push_back(std::stoi(args[1]));
            argument->int_args_.push_back(std::stoi(args[2]));
        } else {
            argument->error_ = true;
        }
    } else if (args[0] == "finished") {
        argument->com_ = finished;
    } else if (args[0] == "win") {
        argument->com_ = win;
    } else if (args[0] == "lose") {
        argument->com_ = lose;
    } else if (args[0] == "dump" && args.size() == 2) {
        argument->com_ = dump;
        argument->string_args_.push_back(args[1]);
    } else if (args[0] == "load" && args.size() == 2) {
        argument->com_ = load;
        argument->string_args_.push_back(args[1]);
    } else {
        argument->error_ = true;
    }

    LogicCommands_->doCommand(argument);
}