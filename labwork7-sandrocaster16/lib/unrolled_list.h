#pragma once

#include <memory>
#include <iostream>

#include "node.h"
#include "iterator.h"

template<typename T, size_t NodeMaxSize = 10, typename Allocator = std::allocator<T>>
class unrolled_list{
private:
    using Node = base_node<T, NodeMaxSize>;
    using AllocTraits = std::allocator_traits<Allocator>;
    using NodeAllocator = typename AllocTraits::template rebind_alloc<Node>;
    using NodeAllocTraits = typename AllocTraits::template rebind_traits<Node>;

    Node* list_head_;
    Node* list_tail_;
    size_t list_size_;

    Allocator allocator_;
    NodeAllocator node_allocator_;

    void move_next(Node* node, size_t first = 0){
        if(node->node_size_ >= NodeMaxSize) return;

        for(size_t i = node->node_size_; i > first; --i){
            value_type* src = reinterpret_cast<value_type*>(node->node_data_+(i-1)*sizeof(value_type));
            value_type* dest = reinterpret_cast<value_type*>(node->node_data_+i*sizeof(value_type));
            
            AllocTraits::construct(allocator_, dest, *src);
            AllocTraits::destroy(allocator_, src);
        }
    }

    void move_prev(Node* node, size_t last = 0){
        if(node->node_size_ == 0) return;

        for(size_t i = last; i < node->node_size_-1; ++i){
            value_type* src = reinterpret_cast<value_type*>(node->node_data_+(i+1)*sizeof(value_type));
            value_type* dest = reinterpret_cast<value_type*>(node->node_data_+i*sizeof(value_type));
            
            AllocTraits::construct(allocator_, dest, *src);
            AllocTraits::destroy(allocator_, src);
        }
    }

    void steal_half(Node* out, Node* to){
        const size_t half_size = out->node_size_/2;
        const size_t remaining = out->node_size_-half_size;
        
        for(size_t i = 0; i < half_size; ++i){
            value_type* src = reinterpret_cast<value_type*>(out->node_data_+(remaining+i)*sizeof(value_type));
            value_type* dest = reinterpret_cast<value_type*>(to->node_data_+i*sizeof(value_type));
            
            AllocTraits::construct(allocator_, dest, std::move(*src));
            AllocTraits::destroy(allocator_, src);
        }
        
        out->node_size_ = remaining;
        to->node_size_ = half_size;
    }

    Node* create_node(){
        Node* new_node = NodeAllocTraits::allocate(node_allocator_, 1);
        
        try{
            NodeAllocTraits::construct(node_allocator_, new_node);

            new_node->node_size_ = 0;
        }
        catch(...){
            NodeAllocTraits::deallocate(node_allocator_, new_node, 1);
            throw;
        }

        return new_node;
    }

    void destroy_node(Node* node){
        if(node == list_head_ && node == list_tail_){
            list_head_ = nullptr;
            list_tail_ = nullptr;
        }
        else if(node == list_head_) list_head_ = node->next_;
        else if(node == list_tail_) list_tail_ = node->prev_;

        for(size_t i = 0; i < node->node_size_; ++i){
            AllocTraits::destroy(allocator_, reinterpret_cast<value_type*>(node->node_data_ + i * sizeof(value_type)));
        }

        NodeAllocTraits::destroy(node_allocator_, node);
        NodeAllocTraits::deallocate(node_allocator_, node, 1);
    }

public:

    // container's requirements
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    using allocator_type = Allocator;
    using iterator = base_iterator<T, NodeMaxSize, false>;
    using const_iterator = base_iterator<T, NodeMaxSize, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //def
    unrolled_list()
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
    {}

    //constructors
    unrolled_list(const Allocator& alloc)
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
        , allocator_(alloc)
    {}

    unrolled_list(std::initializer_list<value_type> init, const Allocator& alloc = Allocator())
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
        , allocator_(alloc) 
    {
        try{
            for(const auto& val : init){
                push_back(val);
            }
        }
        catch(std::exception& e){
            clear();
            throw e;
        }
    }

    unrolled_list(size_type count, const_reference value, const Allocator& alloc = Allocator())
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
        , allocator_(alloc) 
    {
        try{
            for(size_type i = 0; i < count; ++i){
                push_back(value);
            }
        }
        catch(std::exception& e){
            clear();
            throw e;
        }
    }

    unrolled_list(size_type count, const Allocator& alloc = Allocator())
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
        , allocator_(alloc)
    {
        try{
            for(size_type i = 0; i < count; ++i){
                push_back(value_type());
            }
        }
        catch(std::exception& e){
            clear();
            throw e;
        }
    }

    template<typename InputIterator>
    unrolled_list(
        InputIterator first,
        InputIterator last,
        std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>>* = 0
    ) 
        : list_size_(0) 
    {
        try{
            for(; first != last; ++first){
                push_back(*first);
            }
        } 
        catch(std::exception& e){
            clear();
            throw e;
        }
    }

    template<typename InputIterator>
    unrolled_list(
        InputIterator first,
        InputIterator last,
        const allocator_type& a,
        std::enable_if_t<std::is_convertible_v<typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>>* = 0
    )
        : list_size_(0)
        , node_allocator_(a)
    {
        try{
            for(; first != last; ++first){
                push_back(*first);
            }
        }
        catch(const std::exception& e){
            // std::cout<<"QWE\n";
            clear();
            throw e;
        }
    }

    //copy constructor
    unrolled_list(const unrolled_list& other)
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
        , allocator_(AllocTraits::select_on_container_copy_construction(other.allocator_))
        , node_allocator_(NodeAllocTraits::select_on_container_copy_construction(other.node_allocator_))
    {
        try{
            for(const auto& val : other){
                push_back(val);
            }
        }
        catch(std::exception& e){
            clear();
            throw e;
        }
    }

    unrolled_list(const unrolled_list& other, const Allocator& alloc)
        : list_head_(nullptr)
        , list_tail_(nullptr)
        , list_size_(0)
        , allocator_(AllocTraits::select_on_container_copy_construction(alloc))
        , node_allocator_(NodeAllocTraits::select_on_container_copy_construction(alloc))
    {
        try{
            for(const auto& val : other){
                push_back(val);
            }
        }
        catch(std::exception& e){
            clear();
            throw e;
        }
    }

    //copy oper=
    unrolled_list& operator=(const unrolled_list& other){
        if(this != &other){
            clear();

            allocator_ = other.allocator_;
            node_allocator_ = other.node_allocator_;

            try{
                for (const auto& val : other){
                    push_back(val);
                }
            }
            catch(std::exception& e){
                clear();
                throw e;
            }
        }

        return *this;
    }

    //destructor
    ~unrolled_list(){
        clear();
    }

    //iterators
    iterator begin(){
        return iterator(list_head_, 0);
    }
    iterator end(){
        return iterator(list_tail_,  list_tail_ ? list_tail_->node_size_ : 0);
    }
    const_iterator begin() const{
        return const_iterator(list_head_, 0);
    }
    const_iterator end() const{
        return const_iterator(list_tail_, list_tail_ ? list_tail_->node_size_ : 0);
    }
    const_iterator cbegin() const{
        return const_iterator(list_head_, 0);
    }
    const_iterator cend() const{
        return const_iterator(list_tail_, list_tail_ ? list_tail_->node_size_ : 0);
    }
    reverse_iterator rbegin(){
        return reverse_iterator(end()); 
    }
    reverse_iterator rend(){
        return reverse_iterator(begin());
    }
    const_reverse_iterator rbegin() const{
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const{
        return const_reverse_iterator(begin()); 
    }
    const_reverse_iterator crbegin() const{
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crend() const{
        return const_reverse_iterator(begin());
    }

    // ==
    bool operator==(const unrolled_list& other) const {
        if (list_size_ != other.list_size_) return false;
        
        auto it1 = begin();
        auto it2 = other.begin();
        auto end1 = end();
        auto end2 = other.end();
        
        while(it1 != end1 && it2 != end2){
            if(*it1 != *it2){
                return false;
            }
            ++it1;
            ++it2;
        }
        
        return true;
    }

    // !=
    bool operator!=(const unrolled_list& other) const{
        return !(*this == other);
    }

    // size|max_size|empty
    size_type size() const noexcept{
        return list_size_;
    }

    size_type max_size() const{
        return AllocTraits::max_size(allocator_);
    }

    bool empty() const noexcept{
        return list_size_==0;
    }

    // front
    reference front(){
        return *begin();
    }

    const_reference front() const{ 
        return *begin();
    }

    // back
    reference back(){
        return *--end(); 
    }
    const_reference back() const{ 
        return *--end();
    }

    // geters
    Allocator get_allocator() const noexcept{
        return allocator_;
    }

    NodeAllocator get_node_allocator() const noexcept{
        return node_allocator_;
    }

    // addition
    void push_back(const_reference data){
        if(empty()){
            list_head_ = create_node();
            list_tail_ = list_head_;
        }
        else if(list_tail_->node_size_ == NodeMaxSize){
            Node* new_node = create_node();
            new_node->prev_ = list_tail_;
            list_tail_->next_ = new_node;
            list_tail_ = new_node;
        }

        try{
            AllocTraits::construct(allocator_, reinterpret_cast<value_type*>(list_tail_->node_data_ + list_tail_->node_size_ * sizeof(value_type)), data);
        }
        catch(...){
            throw;
        }

        ++list_tail_->node_size_;
        ++list_size_;
    }

    void push_back(reference data){
        if(empty()){
            list_head_ = create_node();
            list_tail_ = list_head_;
        }
        else if(list_tail_->node_size_ == NodeMaxSize){
            Node* new_node = create_node();
            new_node->prev_ = list_tail_;
            list_tail_->next_ = new_node;
            list_tail_ = new_node;
        }

        try{
            AllocTraits::construct(allocator_, reinterpret_cast<value_type*>(list_tail_->node_data_ + list_tail_->node_size_ * sizeof(value_type)), data);
        }
        catch(...){
            throw;
        }

        ++list_tail_->node_size_;
        ++list_size_;
    }

    void push_front(const_reference data){
        if(empty()){
            list_head_ = create_node();
            list_tail_ = list_head_;
        }
        else if(list_head_->node_size_ == NodeMaxSize){
            Node* new_node = create_node();
            new_node->prev_ = list_head_;
            
            if(list_tail_ != list_head_){
                new_node->next_ = list_head_->next_;
                list_head_->next_->prev_ = new_node;
            }
            else{
                list_tail_ = new_node;
            }

            list_head_->next_ = new_node;

            steal_half(list_head_, new_node);
            // list_tail_ = new_node;
        }
        
        move_next(list_head_);    

        try{
            AllocTraits::construct(allocator_, reinterpret_cast<value_type*>(list_head_->node_data_), data);
        }
        catch(...){
            throw;
        }
        
        ++list_head_->node_size_;
        ++list_size_;
    }

    void push_front(reference data){
        if(empty()){
            list_head_ = create_node();
            list_tail_ = list_head_;
        }
        else if(list_head_->node_size_ == NodeMaxSize){
            Node* new_node = create_node();
            new_node->prev_ = list_head_;
            
            if(list_tail_ != list_head_){
                new_node->next_ = list_head_->next_;
                list_head_->next_->prev_ = new_node;
            }
            else{
                list_tail_ = new_node;
            }

            list_head_->next_ = new_node;

            steal_half(list_head_, new_node);
            // list_tail_ = new_node;
        }
        
        move_next(list_head_);    

        try{
            AllocTraits::construct(allocator_, reinterpret_cast<value_type*>(list_head_->node_data_), data);
        }
        catch(...){
            throw;
        }
        
        ++list_head_->node_size_;
        ++list_size_;
    }

    iterator insert(const_iterator pos, const_reference value){
        if(pos == cend()){
            push_back(value);
            return iterator(list_tail_, list_tail_->node_size_-1);
        }

        iterator pos_ = iterator(pos.node_, pos.index_);

        if(pos_.node_->node_size_ == NodeMaxSize){
            Node* new_node = create_node();
            new_node->next_ = pos_.node_->next_;

            if(new_node->next_){
                new_node->next_->prev_ = new_node;
            }

            new_node->prev_ = pos_.node_;
            pos_.node_->next_ = new_node;
            
            if(pos_.node_ == list_tail_){
                list_tail_ = new_node;
            }
            
            steal_half(pos_.node_, new_node);
            
            if(pos_.index_ >= pos_.node_->node_size_){
                pos_.index_ -= pos_.node_->node_size_;
                pos_.node_ = new_node;
            }
        }

        move_next(pos_.node_, pos_.index_);
        AllocTraits::construct(allocator_, reinterpret_cast<value_type*>(pos_.node_->node_data_ + pos_.index_*sizeof(value_type)), value);
        ++pos_.node_->node_size_;
        ++list_size_;

        return pos_;
    }

    iterator insert(const_iterator pos, size_t count, const_reference value){
        if(count == 0) return iterator(pos.node_, pos.index_);

        iterator it(pos.node_, pos.index_);

        for(size_t i = 0; i < count; ++i){
            it = insert(it, value);
            ++it;
        }

        return it;
    }

    iterator erase(const_iterator pos) noexcept{
        if(pos == cend() || empty()){
            return end();
        }

        iterator pos_ = iterator(pos.node_, pos.index_);
        
        AllocTraits::destroy(allocator_, reinterpret_cast<value_type*>(pos_.node_->node_data_ + pos_.index_*sizeof(value_type)));

        // std::cout<<"qwe: "<<pos_.node_->node_size_<<'\n';

        --pos_.node_->node_size_;
        --list_size_;

        if(pos_.node_->node_size_ == 0){
            // if(pos_.node_ == list_head_) list_head_ = pos_.node_->next_;

            if(pos_.node_->next_) pos_.node_->next_->prev_ = pos_.node_->prev_;
            if(pos_.node_->prev_) pos_.node_->prev_->next_ = pos_.node_->next_;
            
            destroy_node(pos_.node_);
        }
        else{
            move_prev(pos_.node_, pos_.index_);
        }

        return pos_;
    }

    iterator erase(const_iterator first, const_iterator last) noexcept{
        if(first == last) return iterator(last.node_, last.index_);

        iterator it(first.node_, first.index_);

        while(it != last){
            it = erase(it);
        }

        return it;
    }

    //clear
    void clear() noexcept{
        while(list_head_){
            Node* t = list_head_;
            list_head_ = list_head_->next_;
            destroy_node(t);
        }

        list_head_ = nullptr;
        list_tail_ = nullptr;
        list_size_ = 0;
    }

    //pop
    void pop_front() noexcept{
        if (empty()) return;
        erase(begin());
    }

    void pop_back() noexcept{
        if (empty()) return;
        erase(--end());
    }

    //swap
    void swap(unrolled_list& other) noexcept {
        std::swap(list_head_, other.list_head_);
        std::swap(list_tail_, other.list_tail_);
        std::swap(list_size_, other.list_size_);
        std::swap(allocator_, other.allocator_);
        std::swap(node_allocator_, other.node_allocator_);
    }

    // print log
    void print_info(){
        Node* t = list_head_;
        size_t count_nodes = 0;

        std::cout<<"-------print_info-------\nHead: "<<list_head_<<" Tail: "<<list_tail_<<" Size: "<<list_size_<<"\n\n";

        while(t){
            std::cout<<"Node: "<<t<<" next: "<<t->next_<<" prev: "<<t->prev_<<'\n';
            for(int i=0; i<t->node_size_; ++i){
                std::cout<<reinterpret_cast<value_type*>(t->node_data_+i*sizeof(value_type))<<' ';
            }
            std::cout<<'\n';
            t = t->next_;
            ++count_nodes;
        }
        std::cout<<"\nCount Nodes: "<<count_nodes<<'\n';
    }

    void print(){
        Node* t = list_head_;
        while(t){
            for(int i=0; i<t->node_size_; ++i){
                std::cout<<reinterpret_cast<value_type*>(t->node_data_+i*sizeof(value_type))<<' ';
            }
            t = t->next_;
        }
        std::cout<<'\n';
    }
};