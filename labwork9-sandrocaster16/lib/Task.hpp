#pragma once

#include "Tuple.hpp"
#include "Any.hpp"
#include "Invoke.hpp"

struct ITask{
	virtual ~ITask() = default;
	virtual const Any &GetResult() = 0;
	virtual bool IsDone() = 0;
	virtual void Invoke() = 0;
};

template<typename Functor, typename... Args>
class Task : public ITask{
private:
	MyTuple<Args...> args_;
	Functor functor_;
	Any result_;
	bool done_;

public:
	Task(const Functor &functor, Args&&... args)
		: functor_(std::move(functor))
		, args_(std::forward<Args>(args)...)
		, done_(false)
	{}

	const Any& GetResult() override{
		return result_;
	}

	bool IsDone() override{
		return done_;
	}

	void Invoke() override{
		if(!done_){
			done_ = true;
			result_ = myInvoke(std::move(functor_), std::move(args_));
		}
	}
};