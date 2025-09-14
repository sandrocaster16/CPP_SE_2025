#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

enum State{
	UNVISITED,
	VISITING,
	VISITED
};

class Graph{
private:
	std::vector<std::vector<size_t>> graph;

public:
	Graph() = default;

	bool dfs(size_t v, std::vector<State>& states, std::vector<size_t>& sortedGraph) const{
		states[v] = State::VISITING;

		for(size_t u : graph[v]){
			if(states[u] == State::UNVISITED){
				if(!dfs(u, states, sortedGraph)){
					return false;
				}
			}
			else if(states[u] == State::VISITING){
				return false;
			}
		}

		states[v] = State::VISITED;
		sortedGraph.push_back(v);
		return true;
	}

	std::vector<size_t> topSort() const{
		std::vector<State> states(graph.size(), State::UNVISITED);
		std::vector<size_t> sortedGraph;

		for(size_t v=0; v < graph.size(); ++v){
			if(states[v] == State::UNVISITED){
				if(!dfs(v, states, sortedGraph)){
					throw std::runtime_error("Graph have a loop");
				}
			}
		}

		std::reverse(sortedGraph.begin(), sortedGraph.end());
		return sortedGraph;
	}

	void add(size_t taskID){
		if(taskID >= graph.size()){
			graph.resize(taskID+1);
		}
	}

	void addEdge(size_t taskID, size_t parent){
		size_t neededSize = 0;

		if(graph.empty()){
			neededSize = std::max(taskID, parent) + 1;
		}
		else{
			neededSize = std::max({graph.size(), taskID + 1, parent + 1});
		}

		if(neededSize > graph.size()){
			graph.resize(neededSize);
		}

		if(taskID == parent){
			throw std::runtime_error("Loop detected: cannot add edge from " + std::to_string(parent) + " to " + std::to_string(taskID));
		}

		graph[parent].push_back(taskID);
	}

	size_t size() const{
		return graph.size();
	}

	const std::vector<std::vector<size_t>>& getGraph() const{
		return graph;
	}
};
