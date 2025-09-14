#pragma once

#include "BaseAdapter.h"

template <typename InputContainer>
class AsDataFlow : public BaseAdapter<typename InputContainer::iterator>{
    InputContainer* input;

public:
    using value_type = typename InputContainer::value_type;

    AsDataFlow() = delete;
    AsDataFlow(InputContainer& in)
        : input(&in)
    {}

    InputContainer::iterator begin(){
        return input->begin();
    }

    InputContainer::iterator end(){
        return input->end();
    }

    InputContainer::iterator begin() const{
        return input->begin();
    }

    InputContainer::iterator end() const{
        return input->end();
    }
};
