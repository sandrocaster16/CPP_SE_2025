#include <iostream>

#include "../lib/unrolled_list.h"

#include <list>

class NodeTag {};

class SomeObj {
public:
    static inline int CopiesCount = 0;
    static inline int DestructorCalled = 0;

    SomeObj() = default;

    SomeObj(SomeObj&&) {}

    SomeObj(const SomeObj&) {
        ++CopiesCount;
        if (CopiesCount == 3) {
            throw std::runtime_error("");
        }
    }

    ~SomeObj() {
        ++DestructorCalled;
    }
};

struct Bad {};
struct Good {
    std::string Name;
};

struct BadOrGood {
    BadOrGood() = default;

    BadOrGood(Bad b) {
        throw std::runtime_error("");
    }

    BadOrGood(Good g)
        : Name(g.Name) {}

    std::string Name;
};

template<typename T>
class TestAllocator {
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;
    using is_always_equal = std::true_type;

    static inline int DeallocationCount = 0;
    static inline int ElementsDeallocated = 0;

    static inline int AllocationCount = 0;
    static inline int ElementsAllocated = 0;

    TestAllocator() = default;

    template<typename U>
    TestAllocator(const TestAllocator<U>& other) {
    }

    void deallocate(T* p, std::size_t n) {
        if constexpr (std::is_same_v<T, SomeObj>) {
            ++TestAllocator<SomeObj>::DeallocationCount;
            TestAllocator<SomeObj>::ElementsDeallocated += n;
        } else {
            ++TestAllocator<NodeTag>::DeallocationCount;
            TestAllocator<NodeTag>::ElementsDeallocated += n;
        }
        delete[] reinterpret_cast<char*>(p);
    }

    pointer allocate(size_type sz) {
        if constexpr (std::is_same_v<T, SomeObj>) {
            ++TestAllocator<SomeObj>::AllocationCount;
            TestAllocator<SomeObj>::ElementsAllocated += sz;
        } else {
            ++TestAllocator<NodeTag>::AllocationCount;
            TestAllocator<NodeTag>::ElementsAllocated += sz;
        }
        return reinterpret_cast<pointer>(new char[sz * sizeof(value_type)]);
    }

};

int main(int argc, char** argv){
    unrolled_list<int, 5> qwe;

    for(int i=0; i<20; ++i){
        qwe.push_back(i);
    }

    for(int i=0; i<20; ++i){
        std::cout<<i<<'\n';
        qwe.erase(qwe.cbegin());
    }

    return 0;
}
