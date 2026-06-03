#pragma once

#include "cppml/Vector.h"
#include "cppml/Matrix.h"

namespace cppml
{
	class KMeans
	{
	public:
		enum class InitMethod
		{
			Random,
			KMeansPlusPlus
		};

	private:
		unsigned long long n_clusters_;
		InitMethod init_method_;
		unsigned long long max_iters_;
		Matrix centroid_centers_;

	public:
		KMeans(unsigned long long n_clusters, InitMethod init_method, unsigned long long max_iters);
		std::vector<unsigned long long> fit_predict(Matrix const& X);
	};
}