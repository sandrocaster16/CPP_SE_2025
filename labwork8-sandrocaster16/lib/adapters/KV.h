#pragma once

template <typename Key, typename Value>
struct KV{
    using key_type = Key;
    using value_type = Value;

    Key key;
    Value value;

    KV() = default;
    KV(Key k, Value v)
        : key(std::move(k))
        , value(std::move(v))
    {}
};