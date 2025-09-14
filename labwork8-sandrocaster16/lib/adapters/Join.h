#pragma once

#include <optional>
#include <functional>

#include "KV.h"
#include "JoinResult.h"
#include "BaseAdapter.h"

struct EmptyKey{};

template<typename T, typename KeyExtractor = EmptyKey>
struct KeyGetter{
    static auto get(const T& value, KeyExtractor extractor){
        return extractor(value);
    }
};

template<typename T>
struct KeyGetter<T, EmptyKey>{
    static auto get(const T& value, EmptyKey){
        if constexpr(requires { value.key; }){
            return value.key;
        }
        else if constexpr(requires { value.first; }){
            return value.first;
        }
        else{
            return value;
        }
    }
};

template<typename T>
struct ValueGetter{
    static auto get(const T& value){
        return value;
    }
};

template<typename K, typename V>
struct ValueGetter<KV<K, V>>{
    static V get(const KV<K, V>& kv){
        return kv.value;
    }
};

template<typename T, typename KeyExtractor = EmptyKey>
auto getKey(const T& value, KeyExtractor extractor = {}){
    return KeyGetter<T, KeyExtractor>::get(value, extractor);
}

template<typename T>
auto getValue(const T& value){
    return ValueGetter<T>::get(value);
}

template <typename LeftFlow, typename RightFlow, typename LeftKey = EmptyKey, typename RightKey = EmptyKey>
class JoinFlow : public BaseAdapter<JoinFlow<LeftFlow, RightFlow, LeftKey, RightKey>>{
public:
    using LeftType = std::remove_cvref_t<decltype(getValue(std::declval<typename LeftFlow::value_type>()))>;
    using RightType = std::remove_cvref_t<decltype(getValue(std::declval<typename RightFlow::value_type>()))>;
    using value_type = JoinResult<LeftType, RightType>;

private:
    LeftFlow leftFlow;
    RightFlow rightFlow;
    LeftKey leftKey;
    RightKey rightKey;

    using RightKeyType = decltype(getKey(std::declval<typename RightFlow::value_type>(), std::declval<RightKey>()));
    std::unordered_multimap<RightKeyType, RightType> rightMap;

public:
    JoinFlow(LeftFlow lf, RightFlow rf)
        : leftFlow(std::move(lf))
        , rightFlow(std::move(rf))
        // , leftKey()
        // , rightKey()
    {
        for(auto i : rightFlow){
            rightMap.emplace(getKey(i), getValue(i));
        }
    }

    JoinFlow(LeftFlow lf, RightFlow rf, LeftKey lk, RightKey rk)
        : leftFlow(std::move(lf))
        , rightFlow(std::move(rf))
        , leftKey(std::move(lk))
        , rightKey(std::move(rk))
    {
        for(auto i : rightFlow){
            rightMap.emplace(getKey(i, rightKey), getValue(i));
        }
    }

    class Iterator{
    private:
        using LeftIterator = typename LeftFlow::Iterator;
        using RightMap = std::unordered_multimap<RightKeyType, RightType>;

        LeftIterator leftIt;
        LeftIterator leftEnd;
        const RightMap* rightMap;
        LeftKey leftKey;
        RightKey rightKey;

        typename RightMap::const_iterator rightIt;
        typename RightMap::const_iterator rightEnd;
        value_type currentValue;
        bool hasMore;

        void findNextMatch(){
            if(leftIt == leftEnd){
                hasMore = false;
                return;
            }

            auto leftKeyValue = getKey(*leftIt, leftKey);
            auto range = rightMap->equal_range(leftKeyValue);

            if(range.first != range.second){
                rightIt = range.first;
                rightEnd = range.second;
                currentValue.base = getValue(*leftIt);
                currentValue.joined = std::make_optional(rightIt->second);
                hasMore = true;
            }
            else{
                currentValue.base = getValue(*leftIt);
                currentValue.joined = std::nullopt;
                hasMore = true;
            }
        }

    public:
        Iterator(LeftIterator it, LeftIterator end, const RightMap* map, LeftKey lk, RightKey rk)
                : leftIt(it)
                , leftEnd(end)
                , rightMap(map)
                , leftKey(lk)
                , rightKey(rk)
                , hasMore(false)
        {
            if(leftIt != leftEnd){
                findNextMatch();
            }
        }

        value_type operator*() const{
            return currentValue;
        }

        Iterator& operator++(){
            if(currentValue.joined.has_value() && rightIt != rightEnd){
                ++rightIt;

                if(rightIt != rightEnd){
                    currentValue.joined = std::make_optional(rightIt->second);
                }
                else{
                    ++leftIt;
                    if(leftIt != leftEnd){
                        findNextMatch();
                    }
                    else{
                        hasMore = false;
                    }
                }
            }
            else{
                ++leftIt;
                if(leftIt != leftEnd){
                    findNextMatch();
                }
                else{
                    hasMore = false;
                }
            }

            return *this;
        }

        bool operator!=(const Iterator& other) const{
            return hasMore != other.hasMore || leftIt != other.leftIt;
        }
    };

    Iterator begin(){
        return Iterator(leftFlow.begin(), leftFlow.end(), &rightMap, leftKey, rightKey);
    }

    Iterator end(){
        return Iterator(leftFlow.end(), leftFlow.end(), &rightMap, leftKey, rightKey);
    }
};

template <typename RightFlow, typename LeftKey = EmptyKey, typename RightKey = EmptyKey>
struct Join{
    RightFlow rightFlow;
    LeftKey leftKey;
    RightKey rightKey;

    Join(RightFlow rf, LeftKey lk, RightKey rk)
        : rightFlow(std::move(rf))
        , leftKey(std::move(lk))
        , rightKey(std::move(rk))
    {}

    Join(RightFlow rf)
        : rightFlow(std::move(rf))
    {}
};

template <typename LeftFlow, typename RightFlow, typename LeftKey, typename RightKey>
auto operator|(LeftFlow&& leftFlow, Join<RightFlow, LeftKey, RightKey> join){
    if constexpr(std::is_same_v<LeftKey, EmptyKey> || std::is_same_v<RightKey, EmptyKey>){
        return JoinFlow(std::forward<LeftFlow>(leftFlow), std::move(join.rightFlow));
    }
    else{
        return JoinFlow(std::forward<LeftFlow>(leftFlow), std::move(join.rightFlow), std::move(join.leftKey), std::move(join.rightKey));
    }
}