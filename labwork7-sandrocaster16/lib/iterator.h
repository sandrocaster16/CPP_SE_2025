#pragma once

#include <iostream>

template<typename T, size_t NodeMaxSize, bool IsConst>
class base_iterator{
public:
    using Node = base_node<T, NodeMaxSize>;

    Node* node_;
    size_t index_;

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = typename std::conditional<IsConst, const T*, T*>::type;
    using reference = typename std::conditional<IsConst, const T&, T&>::type;
    using difference_type = std::ptrdiff_t;

    base_iterator(Node* node = nullptr, size_t index = 0)
        : node_(node)
        , index_(index)
    {}

    base_iterator& operator=(const base_iterator& other){
        index_ = other.index_;
        node_ = other.node_;

        return *this;
    }

    ~base_iterator(){
        node_ = nullptr;
    }

    // it -> const_it
    base_iterator(const base_iterator<T, NodeMaxSize, false>& other)
        : node_(other.node_)
        , index_(other.index_) 
    {}

    // const_it -> it
    base_iterator(const base_iterator<T, NodeMaxSize, true>& other)
        : index_(other.index_)
        , node_(other.node_)
    {}

    // *it
    reference operator*() const{
        return *reinterpret_cast<value_type*>(node_->node_data_+index_*sizeof(value_type));
    }

    // it->
    pointer operator->() const{
        return reinterpret_cast<value_type*>(node_->node_data_+index_*sizeof(value_type));
    }

    // ++it
    base_iterator& operator++(){
        if(index_+1 == node_->node_size_ && node_->next_){
            node_ = node_->next_;
            index_ = 0;
        }
        else if(index_ < node_->node_size_){
            ++index_;
        }

        return *this;
    }

    // it++
    base_iterator operator++(int){
        base_iterator temp = *this;
        ++(*this);
        return temp;
    }

    // --it
    base_iterator& operator--(){
        if(index_ == 0 && node_->prev_){
            node_ = node_->prev_;
            index_ = node_->node_size_-1;
        }
        else if(index_ > 0){
            --index_;
        }

        return *this;
    }

    // it--
    base_iterator operator--(int){
        base_iterator temp = *this;
        --(*this);
        return temp;
    }

    // it+it
    base_iterator operator+(int n) const{
        base_iterator t = *this;
        
        for(int i=0; i<n; ++i){
            ++t;
        }

        return t;
    }

    // it-it
    base_iterator operator-(int n) const{
        base_iterator t = *this;
        
        for(int i=0; i<n; ++i){
            --t;
        }

        return t;
    }


    // все операторы сравнения (вроде все)
    bool operator==(const base_iterator& other) const{
        return (node_ == other.node_ && index_ == other.index_);
    }

    bool operator!=(const base_iterator& other) const{
        return !(*this == other);
    }   
};