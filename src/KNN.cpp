#include "cppml/KNN.h"

#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/KDTree.h"
#include "cppml/BallTree.h"

#include <vector>
#include <unordered_map>
#include <stdexcept>

namespace cppml
{
	KNN::KNN(size_t neighbors, DataStructure data_structure, size_t leaf_size, long long norm) : neighbors_(neighbors), data_structure_(data_structure), leaf_size_(leaf_size), norm_(norm) {}

	void KNN::fit(Matrix const& X, std::vector<unsigned long long> const& y)
	{
		if (X.shape().first != y.size())
			throw std::invalid_argument("The number of matrix rows must match vector size.");

		if (data_structure_ == DataStructure::KDTree)
			kd_tree_ = KDTree(X, leaf_size_, norm_);
		else // data_structure_ == DataStructure::BallTree
			ball_tree_ = BallTree(X, leaf_size_, norm_);
		labels_ = y;
	}

	std::vector<unsigned long long> KNN::predict(Matrix const& X)
	{
		auto [n, m] = X.shape();
		std::vector<unsigned long long> res(n);
		Vector point(m, 0.0);
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
				point[j] = X(i, j);

			std::vector<size_t> indices;
			if (data_structure_ == DataStructure::KDTree)
				indices = kd_tree_.query(point, neighbors_);
			else
				indices = ball_tree_.query(point, neighbors_);

			std::unordered_map<unsigned long long, unsigned long long> um;
			for (size_t idx : indices)
				um[labels_[idx]]++;

			unsigned long long mx = 0;
			for (auto[label, cnt] : um)
				if (cnt > mx)
				{
					mx = cnt;
					res[i] = label;
				}
		}

		return res;
	}
}