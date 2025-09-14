#pragma once

#include <unordered_map>
#include "BaseAdapter.h"

template<typename Input, typename RawKeyType, typename Aggregator, typename KeyFunction>
class AggregateFlow : public BaseAdapter<AggregateFlow<Input, RawKeyType, Aggregator, KeyFunction>>{
private:
    Input input;
    RawKeyType startValue; // start count literaly
    Aggregator aggregator; // функция как обновлять счетчик startValue
    KeyFunction keyFunction; // и функция, которая return key

    using input_type = std::remove_cvref_t<decltype(*std::declval<decltype(std::declval<Input&>().begin())&>())>;
    using key_type = std::invoke_result_t<KeyFunction, input_type>;

    std::unordered_map<key_type, RawKeyType> goodKeys; // результаты

public:
    using value_type = std::pair<key_type, RawKeyType>;

    class Iterator{
    private:
        using map_iterator = typename std::unordered_map<key_type, RawKeyType>::iterator;
        map_iterator it;

    public:
        explicit Iterator(map_iterator it)
            : it(it)
        {}

        value_type operator*() const{
            return {it->first, it->second};
        }

        Iterator& operator++(){
            ++it;
            return *this;
        }

        bool operator==(const Iterator& other) const{
            return this->it == other.it;
        }
    };

    AggregateFlow(Input i, RawKeyType rk, Aggregator a, KeyFunction kf)
        : input(std::move(i))
        , startValue(std::move(rk))
        , aggregator(std::move(a))
        , keyFunction(std::move(kf))
    {
        for(const auto& item : input){
            key_type key = keyFunction(item);

            if(goodKeys.find(key) == goodKeys.end()){
                goodKeys[key] = startValue;
            }
            
            aggregator(item, goodKeys[key]);
        }
    }

    Iterator begin(){
        return Iterator(goodKeys.begin());
    }

    Iterator end(){
        return Iterator(goodKeys.end());
    }
};

template<typename RawKeyType, typename Aggregator, typename KeyFunction>
struct AggregateByKey{
    RawKeyType startValue;
    Aggregator aggregator;
    KeyFunction keyFunction;

    AggregateByKey(RawKeyType rk, Aggregator a, KeyFunction kf)
        : startValue(std::move(rk))
        , aggregator(std::move(a))
        , keyFunction(std::move(kf))
    {}
};

template<typename InputType, typename RawKeyType, typename Aggregator, typename KeyFunction>
AggregateFlow<InputType, RawKeyType, Aggregator, KeyFunction> operator|(InputType&& input, AggregateByKey<RawKeyType, Aggregator, KeyFunction> aggregateByKey){
    return AggregateFlow<InputType, RawKeyType, Aggregator, KeyFunction>(std::forward<InputType>(input), std::move(aggregateByKey.startValue), std::move(aggregateByKey.aggregator), std::move(aggregateByKey.keyFunction));
}
