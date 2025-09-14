#pragma once

#include "Filter.h"

struct DropNullopt{};

template <typename Flow>
auto operator|(Flow flow, DropNullopt){
    return FilteredDataFlow(flow, [](decltype(*std::declval<decltype(std::declval<Flow&>().begin())&>()) drop){
        return drop.has_value();
    });
}