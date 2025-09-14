#pragma once

#include <ostream>

template <typename Input>
class WriteAdapter{
private:
    Input input;
    std::string delimiter;
    std::ostream* stream;

public:
    using value_type = void;
    using Base = Input;
    using InputIterator = typename Input::Iterator;

    WriteAdapter() = delete;

    WriteAdapter(Input i, std::ostream& s, std::string d)
        : input(std::move(i))
        , stream(&s)
        , delimiter(d)
    {
        InputIterator inputBegin = input.begin();
        InputIterator inputEnd = input.end();

        while(inputBegin != inputEnd){
            (*stream) << (*inputBegin) << delimiter;
            ++inputBegin;
        }
    }
};

struct Write{
    std::ostream* stream;
    std::string delimiter;

    Write() = delete;

    Write(std::ostream& s, std::string d)
        : stream(&s)
        , delimiter(std::move(d))
    {}

    Write(std::ostream& s, char d)
        : stream(&s)
    {
        delimiter.resize(1);
        delimiter[0] = d;
    }
};

template <typename InputType>
void operator|(InputType&& input, Write write){
    WriteAdapter(std::forward<InputType>(input), *write.stream, write.delimiter);
}
