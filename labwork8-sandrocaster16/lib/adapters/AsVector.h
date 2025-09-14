#pragma once

template <typename Input>
class AsVectorFlow{
private:
    Input input;

public:
    AsVectorFlow(Input i)
        : input(std::move(i))
    {}

    using return_type = std::vector<std::remove_cvref_t<decltype(*std::declval<decltype(std::declval<Input&>().begin())&>())>>;

    return_type getVector(){
        return_type result;

        typename Input::Iterator current = input.begin();

        while(current != input.end()){
            result.push_back(*current);
            ++current;
        }

        return result;
    }
};

struct AsVector{};

template<typename InputType>
std::vector<typename std::remove_cvref_t<InputType>::value_type> operator|(InputType&& input, AsVector){
    return AsVectorFlow(std::forward<InputType>(input)).getVector();
}