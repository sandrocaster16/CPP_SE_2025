#pragma once

#include <functional>
#include "BaseAdapter.h"

template <typename Input, typename Function>
class TransformDataFlow : public BaseAdapter<TransformDataFlow<Input, Function>>{
private:
    std::remove_cvref_t<Input> input;
    Function func;

public:
    using InputElementType = decltype(*std::declval<typename Input::Iterator>());
    using value_type = std::invoke_result_t<Function, InputElementType>;

    TransformDataFlow(Input i, Function f)
        : input(std::move(i))
        , func(std::move(f))
    {}

    class Iterator{
    private:
        TransformDataFlow<Input, Function>* parent;
        typename Input::Iterator current;

    public:
        using return_type = typename TransformDataFlow<Input, Function>::value_type;
        using value_type = return_type;
        using iterator_category = std::input_iterator_tag;

        Iterator() = default;

        Iterator(TransformDataFlow& p, typename Input::Iterator c)
            : parent(&p)
            , current(c)
        {}

         Iterator(const TransformDataFlow& p, typename Input::Iterator c)
            : parent(&p)
            , current(c)
        {}

        return_type operator*(){
            return std::invoke(parent->func, *current);
        }

        Iterator& operator++(){
            ++current;
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
        return Iterator(*this, inputBegin);
    }

    Iterator end(){
        return Iterator(*this, input.end());
    }

    Iterator begin() const{
        typename Input::Iterator inputBegin = input.begin();
        return Iterator(*this, inputBegin);
    }

    Iterator end() const{
        return Iterator(*this, input.end());
    }
};

template <typename Function>
struct Transform{
    Function func;

    Transform(Function func)
        : func(std::move(func))
    {}
};

template <typename InputType, typename Function>
TransformDataFlow<std::remove_cvref_t<InputType>, Function> operator|(InputType&& input, Transform<Function> transform){
    return TransformDataFlow<std::remove_cvref_t<InputType>, Function>(std::forward<InputType>(input), std::move(transform.func));
}
