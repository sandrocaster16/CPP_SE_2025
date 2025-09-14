#pragma once

#include <utility>
#include <type_traits>

template<typename... Types>
class MyTuple;

template<>
class MyTuple<>{};

template<typename First, typename... Rest>
class MyTuple<First, Rest...> : public MyTuple<Rest...>{
public:
	First first_;

	template<typename TFirst, typename... TRest>
	MyTuple(TFirst&& first, TRest&&... rest)
		: MyTuple<Rest...>(std::forward<TRest>(rest)...)
		, first_(std::forward<TFirst>(first))
	{}
};

template<size_t I, typename... Types>
auto get(MyTuple<Types...>& t);

template<size_t I, typename... Types>
decltype(auto) get(const MyTuple<Types...>& t);

template<size_t I, typename... Types>
decltype(auto) get(MyTuple<Types...>&& t);

template<size_t I, typename Head, typename... Tail>
auto get_impl_lvalue(MyTuple<Head, Tail...>& t, std::integral_constant<size_t, I>) {
	if constexpr (I == 0){
		return (t.first_);
	}
	else{
		return get<I - 1>(static_cast<MyTuple<Tail...>&>(t));
	}
}

template<size_t I, typename... Types>
auto get(MyTuple<Types...>& t) {
	return get_impl_lvalue(t, std::integral_constant<size_t, I>{});
}

template<size_t I, typename Head, typename... Tail>
auto get_impl_const_lvalue(const MyTuple<Head, Tail...>& t, std::integral_constant<size_t, I>){
	if constexpr (I == 0){
		return (t.first_);
	}
	else{
		return get<I - 1>(static_cast<const MyTuple<Tail...>&>(t));
	}
}

template<size_t I, typename... Types>
decltype(auto) get(const MyTuple<Types...>& t){
	return get_impl_const_lvalue(t, std::integral_constant<size_t, I>{});
}

template<size_t I, typename Head, typename... Tail>
decltype(auto) get_impl_rvalue(MyTuple<Head, Tail...>&& t, std::integral_constant<size_t, I>){
	if constexpr (I == 0){
		return std::forward<Head>(t.first_);
	}
	else{
		return get<I - 1>(static_cast<MyTuple<Tail...>&&>(t));
	}
}

template<size_t I, typename... Types>
decltype(auto) get(MyTuple<Types...>&& t) {
	return get_impl_rvalue(std::move(t), std::integral_constant<size_t, I>{});
}
