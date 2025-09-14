#pragma once

#include <stdexcept>
#include <typeinfo>

class Any{
private:
	struct Base{
		virtual ~Base() {}
		virtual Base *getCopy() const = 0;
		virtual const std::type_info &type() const = 0;
	};

	template<typename T>
	struct Derived : public Base{
		T value;

		Derived(const T &value)
			: value(value) {}

		Base *getCopy() const override{
			return new Derived(value);
		}

		const std::type_info &type() const override{
			return typeid(T);
		}
	};

	Base *data = nullptr;

public:
	Any() noexcept = default;

	template<typename T>
	Any(const T& other)
		: data(new Derived<T>(other))
	{}

	Any(const Any& other)
		: data(other.data ? other.data->getCopy() : nullptr)
	{}

	Any(Any&& other) noexcept
		: data(other.data)
	{
		other.data = nullptr;
	}

	~Any(){
		delete data;
	}

	Any& operator=(const Any &other){
		if(this != &other){
			Base* newData = (other.data ? other.data->getCopy() : nullptr);
			delete data;
			data = newData;
		}

		return *this;
	}

	template<typename T>
	Any& operator=(const T &other) noexcept{
		delete data;
		data = new Derived<T>(other);

		return *this;
	}

	Any& operator=(Any&& other){
		if(this != &other){
			delete data;
			data = other.data;
			other.data = nullptr;
		}
		return *this;
	}

	const std::type_info& type() const noexcept{
		return (data ? data->type() : typeid(void));
	}

	template<typename T>
	friend T& any_cast(Any& any);
	template<typename T>
	friend const T& any_cast(const Any& any);
	template<typename T>
	friend T* any_cast(Any* any);
};

template<typename T>
T& any_cast(Any& any){
	if(any.type() != typeid(T)){
		throw std::runtime_error("Bad any_cast");
	}

	return static_cast<Any::Derived<T>*>(any.data)->value;
}

template<typename T>
const T& any_cast(const Any& any){
	if(any.type() != typeid(T)){
		throw std::runtime_error("Bad any_cast");
	}

	return static_cast<Any::Derived<T>*>(any.data)->value;
}

template<typename T>
T* any_cast(Any* any){
	if(!any){
		return nullptr;
	}

	if(any->type() != typeid(T)){
		throw std::runtime_error("Bad any_cast");
	}

	return &static_cast<Any::Derived<T>*>(any->data)->value;
}