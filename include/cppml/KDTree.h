#pragma once

#include "cppml/Vector.h"
#include "cppml/math_utils.h"

#include <vector>

namespace cppml
{
	class KDTree
	{
	public:
		struct Node
		{
			Node* left = nullptr, * right = nullptr;
			std::vector<size_t> indices;
			size_t d = 0;
			double pivot = 0.0;
		};

	private:
		size_t leaf_size_;
		Node* root_;
		std::vector<Vector> points_;

	private:
		void build(Node* v, std::vector<size_t> const& indices);
		void copy(Node* v, Node* u);
		void free(Node* v);

	public:
		KDTree(Matrix const& X, size_t leaf_size);
		KDTree(KDTree const& other);
		KDTree& operator=(KDTree const& other);
		~KDTree();
	};
}