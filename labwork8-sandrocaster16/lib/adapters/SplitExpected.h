#pragma once

#include "Transform.h"
#include "Filter.h"

struct SplitExpected{};

template<typename Flow>
auto operator|(Flow range, SplitExpected){
    return std::make_pair(
            FilteredDataFlow(range, [](const auto& ex){
                if(!ex.has_value()){
                    return true;
                }
                return false;
            })
            | Transform([](const auto& ex){
                return ex.error();
            }),

            FilteredDataFlow(range, [](const auto& ex){
                return ex.has_value();
            })
            | Transform([](const auto& ex){
                return ex.value();
            })
    );
}