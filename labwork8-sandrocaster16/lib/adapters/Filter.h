#pragma once

#include "BaseAdapter.h"

template <typename Input, typename Function>
class FilteredDataFlow : public BaseAdapter<FilteredDataFlow<Input, Function>>{
private:
    Input input;
    Function func;

public:
    using value_type = Input::value_type;

    FilteredDataFlow(Input i, Function p)
        : input(std::move(i))
        , func(std::move(p))
    {}

    class Iterator{
    private:
        FilteredDataFlow<Input, Function>* parent;
        typename Input::Iterator current;

        void skip(){
            while(current != parent->input.end() && !(parent->func(*current))){
                ++current;
            }
        }

    public:
        using return_type = typename FilteredDataFlow<Input, Function>::value_type;
        using value_type = return_type;
        using iterator_category = std::input_iterator_tag;

        Iterator() = default;
        Iterator(FilteredDataFlow& p, typename Input::Iterator c)
            : parent(&p)
            , current(c)
        {}

        return_type operator*(){
            return *current;
        }

        Iterator& operator++(){
            ++current;
            skip();
            return *this;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs){
            return lhs.current == rhs.current;
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs){
            return !(rhs == lhs);
        }
    };

    Iterator begin(){
        typename Input::Iterator inputBegin = input.begin();

        while(inputBegin != input.end() && !func(*inputBegin)){
            ++inputBegin;
        }

        return Iterator(*this, inputBegin);
    }

    Iterator end(){
        return Iterator(*this, input.end());
    }
};

template <typename Function>
struct Filter{
    Function func;

    Filter(Function pred)
        : func(std::move(pred))
    {}
};

template <typename InputType, typename Function>
FilteredDataFlow<InputType, Function> operator|(InputType&& input, Filter<Function> filter){
    return FilteredDataFlow(std::forward<InputType>(input), std::move(filter.func)); 
}