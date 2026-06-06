#pragma once

#include "cppml/KDTree.h"
#include "cppml/BallTree.h"

namespace cppml
{
	class KNN
	{
	public:
		enum class DataStructure
		{
			KDTree,
			BallTree
		};

	private:
		size_t neighbors_;
		DataStructure data_structure_;
		size_t leaf_size_;
		long long norm_;
		KDTree kd_tree_;
		BallTree ball_tree_;
		std::vector<unsigned long long> labels_;

	public:
		KNN(size_t neighbors, DataStructure data_structure, size_t leaf_size, long long norm);
		void fit(Matrix const& X, std::vector<unsigned long long> const& y);
		std::vector<unsigned long long> predict(Matrix const& X);
	};
}