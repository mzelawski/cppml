#include "cppml/KMeans.h"
#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/math_utils.h"
#include "cppml/Random.h"

#include <unordered_set>
#include <climits>

namespace cppml
{
	// Either cppml::KMeans::InitMethod::Random or cppml::KMeans::InitMethod::KMeansPlusPlus
	KMeans::KMeans(unsigned long long n_clusters, InitMethod init_method, unsigned long long max_iters) : n_clusters_(n_clusters), init_method_(init_method), max_iters_(max_iters) {}

	std::vector<unsigned long long> KMeans::fit_predict(Matrix const& X)
	{
		Random& random = get_random();
		auto [n, d] = X.shape();
		centroid_centers_ = Matrix(n_clusters_, d, 0.0);

		if (init_method_ == InitMethod::Random)
		{
			std::unordered_set<long long> rnd = floyds_sampling_algorithm(0, n, n_clusters_);
			size_t i = 0;
			for (long long rnd_i : rnd)
			{
				for (size_t j = 0; j < d; j++)
					centroid_centers_(i, j) = X(rnd_i, j);
				i++;
			}
		}
		else // init_method == InitMethod::KMeansPlusPlus
		{
			size_t first = random.uniform_int_distribution(0, n);
			for (size_t i = 0; i < d; i++)
				centroid_centers_(0, i) = X(first, i);

			Vector dists(n, 1e300);

			for (size_t i = 1; i < n_clusters_; i++)
			{
				double sum = 0.0;
				for (size_t j = 0; j < n; j++)
				{
					double cur_dist = 0.0;
					for (size_t k = 0; k < d; k++)
					{
						double val = X(j, k) - centroid_centers_(i - 1, k);
						cur_dist += val * val;
					}
					dists[j] = std::min(dists[j], cur_dist);
					sum += dists[j];
				}

				Vector probabilities(n, 0.0);
				for (size_t j = 0; j < n; j++)
					probabilities[j] = dists[j] / sum;

				size_t k = choose_random_element(probabilities);
				for (size_t j = 0; j < d; j++)
					centroid_centers_(i, j) = X(k, j);
			}
		}

		std::vector<unsigned long long> assignments(n, ULLONG_MAX);
		for (unsigned long long i = 0; i < max_iters_; i++)
		{
			bool end = true;

			for (size_t j = 0; j < n; j++)
			{
				double min_dist = 1e300;
				unsigned long long best_assignment = ULLONG_MAX;
				for (size_t k = 0; k < n_clusters_; k++)
				{
					double cur_dist = 0.0;
					for (size_t l = 0; l < d; l++)
					{
						double val = X(j, l) - centroid_centers_(k, l);
						cur_dist += val * val;
					}

					if (cur_dist < min_dist)
					{
						min_dist = cur_dist;
						best_assignment = k;
					}
				}

				if (best_assignment != assignments[j])
					end = false;
				assignments[j] = best_assignment;
			}

			if (end)
				break;

			for (size_t j = 0; j < n_clusters_; j++)
				for (size_t k = 0; k < d; k++)
					centroid_centers_(j, k) = 0.0;

			std::vector<size_t> cnts(n_clusters_);
			for (size_t j = 0; j < n; j++)
			{
				size_t idx = assignments[j];
				cnts[idx]++;
				for (size_t k = 0; k < d; k++)
					centroid_centers_(idx, k) += X(j, k);
			}

			for (size_t j = 0; j < n_clusters_; j++)
			{
				if (cnts[j] > 0)
				{
					for (size_t k = 0; k < d; k++)
						centroid_centers_(j, k) /= (double)cnts[j];
				}
				else
				{
					size_t rnd_idx = random.uniform_int_distribution(0, n);
					for (size_t k = 0; k < d; k++)
						centroid_centers_(j, k) = X(rnd_idx, k);
				}
			}
		}

		return assignments;
	}
}