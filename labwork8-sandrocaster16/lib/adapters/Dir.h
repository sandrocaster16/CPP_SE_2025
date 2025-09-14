#pragma once

#include <filesystem>
#include <string>

#include "BaseAdapter.h"

template <bool recursive>
using DirIterator =
        std::conditional_t<
            recursive,
            std::filesystem::recursive_directory_iterator,
            std::filesystem::directory_iterator
        >;

template <bool recursive = false>
class Dir : public BaseAdapter<DirIterator<recursive>>{
private:
    std::filesystem::path root_dir;

public:
    using Iterator = DirIterator<recursive>;
    using value_type = std::filesystem::path;

    Dir(const std::string& dirname)
        : root_dir(dirname)
    {}

    Iterator begin(){
        return Iterator(root_dir);
    }

    Iterator end(){
        return Iterator();
    }

    Iterator begin() const{
        return Iterator(root_dir);
    }

    Iterator end() const{
        return Iterator();
    }
};
