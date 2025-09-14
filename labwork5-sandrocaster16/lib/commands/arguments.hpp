#pragma once

#include <iostream>
#include <vector>
#include "commands.hpp"

struct Arguments{
    Commands com_;
    bool error_;

    std::vector<std::string> string_args_;
    std::vector<uint64_t> int_args_;

    Arguments()
        : error_(false)
    {}
};
