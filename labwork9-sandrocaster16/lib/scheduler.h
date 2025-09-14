#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <type_traits>
#include <stdexcept>

#include "Any.hpp"
#include "Function.hpp"
#include "GraphLogic.hpp"
#include "Tuple.hpp"
#include "Task.hpp"
#include "Invoke.hpp"

// TODO: Implement scheduler
class TTaskScheduler{
private:
	std::vector<std::unique_ptr<ITask>> tasks;
	Graph graphTask;

public:
	TTaskScheduler() = default;

	template <typename T>
	class FutureResult{
	private:
		ITask* ptr;

	public:
		FutureResult(ITask* ptr)
			: ptr(ptr)
		{}

		operator T() const{
			return any_cast<T>(ptr->GetResult());
		}
	};

	template<typename Func, typename... Args>
	size_t add(Func&& f, Args&&... args){
		using DecayedF = std::decay_t<Func>;
		using ResultType = std::invoke_result_t<DecayedF, Args...>;

		auto func_obj = Function<ResultType(Args...)>(std::forward<Func>(f));

		auto task = std::make_unique<Task<Function<ResultType(Args...)>, Args...>>(
			std::move(func_obj), std::forward<Args>(args)...
		);

		tasks.push_back(std::move(task));

		graphTask.add(tasks.size()-1);

		return tasks.size()-1;
	}

	template<typename T>
	FutureResult<T> getFutureResult(size_t id){
		if(id >= tasks.size()){
			throw std::out_of_range("Task ID out of bounds in getFutureResult");
		}

		graphTask.addEdge(tasks.size(), id);
		return FutureResult<T>(tasks[id].get());
	}

	template<typename T>
	T getResult(size_t id){
		if(!tasks[id]->IsDone()){
			std::vector<size_t> execute_these_tasks = graphTask.topSort();

			for(size_t other_id: execute_these_tasks){
				if(!tasks[other_id]->IsDone()){
					tasks[other_id]->Invoke();
				}
			}
		}

		return any_cast<T>(tasks[id]->GetResult());
	}

	void executeAll(){
		std::vector<size_t> sorted_tasks = graphTask.topSort();
		for(size_t task_id : sorted_tasks){
			if(task_id < tasks.size()){
				tasks[task_id]->Invoke();
			}
		}
	}
};
