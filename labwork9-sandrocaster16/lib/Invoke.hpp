#pragma once

#include "Tuple.hpp"
#include "Any.hpp"
#include <utility>

template<typename Functor, typename Tuple, size_t... Is>
auto my_apply_impl(Functor&& f, Tuple&& t, std::index_sequence<Is...>){
	return std::forward<Functor>(f)(get<Is>(std::forward<Tuple>(t))...);
}

template<typename Functor, typename... Args>
auto myInvoke(Functor&& f, MyTuple<Args...>&& t){
	return my_apply_impl(std::forward<Functor>(f), std::move(t), std::make_index_sequence<sizeof...(Args)>{});
}

