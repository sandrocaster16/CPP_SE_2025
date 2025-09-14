#pragma once

#include <optional>

template <typename Base, typename Joined>
struct JoinResult{
    Base base;
    std::optional<Joined> joined;

    JoinResult() = default;
    JoinResult(Base b, std::optional<Joined> j = std::nullopt)
        : base(std::move(b))
        , joined(std::move(j))
    {}

    bool operator==(const JoinResult& rhs) const{
        return (this->base == rhs.base && this->joined == rhs.joined);
    }
};