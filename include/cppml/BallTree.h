#pragma once

#include "cppml/Vector.h"
#include "cppml/math_utils.h"

#include <vector>
#include <queue>
#include <utility>

namespace cppml
{
	class BallTree
	{
	public:
		struct Node
		{
			Node* left = nullptr, * right = nullptr;
			std::vector<size_t> indices;
			size_t left_pivot_idx = 0, right_pivot_idx = 0;
			double radius = 0.0;
		};

	private:
		size_t leaf_size_;
		long long norm_;
		Node* root_;
		std::vector<Vector> points_;

	private:
		size_t farthest(Vector const& point, std::vector<size_t> const& indices);
		void build(Node* v, std::vector<size_t> const& indices);
		void copy(Node* v, Node* u);
		void free(Node* v);
		void query_node(Node* v, Vector const& point, std::priority_queue<std::pair<double, size_t>>& pq, size_t neighbors) const;

	public:
		BallTree();
		BallTree(Matrix const& X, size_t leaf_size, long long norm);
		BallTree(BallTree const& other);
		BallTree& operator=(BallTree const& other);
		~BallTree();
		std::vector<size_t> query(Vector const& point, size_t neighbors) const;
	};
}