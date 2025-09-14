#pragma once

#include <fstream>
#include "BaseAdapter.h"

template <typename Input>
class OpenFilesAdapter : public BaseAdapter<OpenFilesAdapter<Input>>{
private:
    Input input;

public:
    using value_type = std::ifstream;
    using Base = Input;

    class Iterator{
    private:
        typename Input::Iterator current;
        OpenFilesAdapter<Input>* parent;

    public:
        using iterator_category = std::input_iterator_tag;

        Iterator(){}

        Iterator(OpenFilesAdapter& parent, typename Input::Iterator it)
            : parent(&parent)
            , current(it)
        {}

        value_type operator*(){
            return std::ifstream(*current);
        }

        Iterator& operator++(){
            ++current;
            return *this;
        }

        Iterator operator++(int){
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator &lhs, const Iterator &rhs){
            return lhs.current == rhs.current;
        }

        friend bool operator!=(const Iterator &lhs, const Iterator &rhs){
            return !(rhs == lhs);
        }
    };

    OpenFilesAdapter(Input in)
        : input(std::move(in))
    {}

    Iterator begin(){
        return Iterator(*this, input.begin());
    }

    Iterator end(){
        return Iterator(*this, input.end());
    }
};

struct OpenFiles{};

template <typename InputType>
OpenFilesAdapter<InputType> operator|(InputType&& input, OpenFiles){
    return OpenFilesAdapter(std::forward<InputType>(input));
}