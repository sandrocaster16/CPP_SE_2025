#pragma once

#include <ostream>

template <typename Input>
class OutAdapter{
private:
    Input input;
    std::ostream* stream;

public:
    using value_type = void;
    using Base = Input;
    using InputIterator = typename Input::Iterator;

    OutAdapter() = delete;

    OutAdapter(Input i, std::ostream& s)
        : input(std::move(i))
        , stream(&s)
    {
        InputIterator inputBegin = input.begin();
        InputIterator inputEnd = input.end();

        while(inputBegin != inputEnd){
            (*stream) << (*inputBegin);
            ++inputBegin;
        }
    }
};

struct Out{
    std::ostream* stream;

    Out() = delete;

    Out(std::ostream& s)
        : stream(&s)
    {}
};

template <typename InputType>
void operator|(InputType&& input, Out out){
    OutAdapter(std::forward<InputType>(input), *out.stream);
}
