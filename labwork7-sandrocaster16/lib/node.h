#pragma once

#include <iostream>

template<typename T, size_t NodeMaxSize>
struct base_node{
    uint8_t node_data_[NodeMaxSize*sizeof(T)];
    size_t node_size_;

    base_node* prev_;
    base_node* next_;

    base_node()
        : prev_(nullptr)
        , next_(nullptr)
        , node_size_(0)
    {}

    ~base_node(){
        prev_ = nullptr;
        next_ = nullptr;
    }
};