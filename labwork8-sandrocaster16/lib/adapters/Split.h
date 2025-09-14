#pragma once

#include "BaseAdapter.h"

template <typename Input>
class SplittedDataFlow : public BaseAdapter<SplittedDataFlow<Input>>{
    Input input;
    std::string splitters;

public:
    using value_type = std::string;

    class Iterator;

    SplittedDataFlow(Input in, std::string pred)
        : input(std::move(in))
        , splitters(std::move(pred))
    {}

    Iterator begin(){
        return Iterator(*this, input.begin());
    }

    Iterator end(){
        return Iterator();
    }
};

template <typename Input>
class SplittedDataFlow<Input>::Iterator{
private:
    SplittedDataFlow<Input>* parent = nullptr;
    typename Input::Iterator current;
    std::string current_chunk;
    size_t current_pos = 0;
    bool reached_end = false;
    std::string current_value;

    void fillBuffer(){
        if(current == parent->input.end()){
            reached_end = true;
            return;
        }

        std::stringstream ss;
        ss << (*current).rdbuf();
        current_chunk = ss.str();
        current_pos = 0;
        ++current;
    }

    std::string readNext(){
        if(reached_end)
            return "";

        while(true){
            if(current_pos >= current_chunk.size()){
                fillBuffer();

                if(reached_end)
                    return "";
                
                continue;
            }

            size_t split_pos = current_chunk.find_first_of(parent->splitters, current_pos);

            if(split_pos == std::string::npos){
                std::string result = current_chunk.substr(current_pos);
                current_pos = current_chunk.size();
                return result;
            }

            if(split_pos > current_pos){
                std::string result = current_chunk.substr(current_pos, split_pos - current_pos);
                current_pos = split_pos + 1;
                return result;
            }

            current_pos = split_pos + 1;
        }
    }

public:
    using return_type = typename SplittedDataFlow<Input>::value_type;
    using value_type = return_type;
    using iterator_category = std::input_iterator_tag;

    Iterator()
        : reached_end(true)
    {}

    Iterator(SplittedDataFlow& p, typename Input::Iterator c)
        : parent(&p)
        , current(c)
    {
        ++(*this);
    }

    return_type& operator*(){
        return current_value;
    }

    Iterator& operator++(){
        current_value = readNext();
        return *this;
    }

    friend bool operator==(const Iterator &lhs, const Iterator &rhs){
        return (lhs.reached_end && rhs.reached_end || (!lhs.reached_end && !rhs.reached_end && lhs.current == rhs.current && lhs.current_pos == rhs.current_pos));
    }

    friend bool operator!=(const Iterator &lhs, const Iterator &rhs){
        return !(lhs == rhs);
    }
};

struct Split{
    std::string predicate;

    Split(std::string pred)
        : predicate(std::move(pred))
    {}
};

template <typename InputType>
SplittedDataFlow<InputType> operator|(InputType&& input, Split split){
    return SplittedDataFlow<InputType>(std::forward<InputType>(input), std::move(split.predicate));
}