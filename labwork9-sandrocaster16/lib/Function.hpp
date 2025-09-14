#pragma once

#include <memory>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <functional>

template<typename>
class Function;

template<typename R, typename... Args>
class Function<R(Args...)>{
private:
	struct ICallable{
		virtual ~ICallable() = default;
		virtual R invoke(Args... args) = 0;
		virtual std::unique_ptr<ICallable> clone() const = 0;
	};

	template<typename F_Storage>
	struct callable : ICallable{
		F_Storage f_member;

		template<typename F_CtorArg>
		callable(F_CtorArg&& f_val)
			: f_member(std::forward<F_CtorArg>(f_val))
		{}

		template<typename T> struct is_reference_wrapper_helper : std::false_type {};
		template<typename U> struct is_reference_wrapper_helper<std::reference_wrapper<U>> : std::true_type {};
		template<typename T> static constexpr bool is_reference_wrapper_v = is_reference_wrapper_helper<std::decay_t<T>>::value;

		template <typename MemberFuncPtr, typename ObjectArg, typename... MemberFuncCallArgs>
		static R call_member_func_helper(MemberFuncPtr mfp, ObjectArg&& obj, MemberFuncCallArgs&&... mf_args){
			if constexpr (std::is_pointer_v<std::decay_t<ObjectArg>>){
				if(!obj){
					throw std::runtime_error("Attempt to call a member function through a null pointer.");
				}
				return (obj->*mfp)(std::forward<MemberFuncCallArgs>(mf_args)...);
			}
			else if constexpr (is_reference_wrapper_v<ObjectArg>){
				return (obj.get().*mfp)(std::forward<MemberFuncCallArgs>(mf_args)...);
			}
			else{
				return (std::forward<ObjectArg>(obj).*mfp)(std::forward<MemberFuncCallArgs>(mf_args)...);
			}
		}

		R invoke(Args... args) override{
			if constexpr (std::is_member_function_pointer_v<std::decay_t<F_Storage>>){
				return call_member_func_helper(f_member, std::forward<Args>(args)...);
			}
			else{
				return f_member(std::forward<Args>(args)...);
			}
		}

		std::unique_ptr<ICallable> clone() const override{
			return std::make_unique<callable<F_Storage>>(f_member);
		}
	};

	std::unique_ptr<ICallable> callable_;

public:
	Function() = default;

	Function(std::nullptr_t)
		: callable_(nullptr)
	{}

	template<typename F_param>
	Function(F_param f_val)
		: callable_(std::make_unique<callable<std::decay_t<F_param>>>(std::forward<F_param>(f_val)))
	{}

	Function(const Function& other)
		: callable_(other.callable_ ? other.callable_->clone() : nullptr)
	{}

	Function(Function&& other) noexcept = default;

	Function& operator=(const Function& other){
		if(this != &other){
			callable_ = other.callable_ ? other.callable_->clone() : nullptr;
		}
		return *this;
	}

	Function& operator=(Function&& other) noexcept = default;

	Function& operator=(std::nullptr_t){
		callable_ = nullptr;
		return *this;
	}

	template<typename F_param>
	Function& operator=(F_param&& f_val){
		callable_ = std::make_unique<callable<std::decay_t<F_param>>>(std::forward<F_param>(f_val));
		return *this;
	}

	R operator()(Args... args) const{
		if (!callable_){
			throw std::runtime_error("bad_function_call");
		}
		return callable_->invoke(std::forward<Args>(args)...);
	}

	explicit operator bool() const noexcept{
		return callable_ != nullptr;
	}
};