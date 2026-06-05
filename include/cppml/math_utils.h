#pragma once

#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/Random.h"

#include <unordered_set>
#include <utility>

namespace cppml
{
	Matrix cholesky_decomposition(Matrix const& A);
	Vector forward_substitution(Matrix const& L, Vector const& b);
	Vector backward_substitution(Matrix const& L, Vector const& b);
	Vector solve_cholesky(Matrix const& A, Vector const& b);

	size_t choose_random_element(Vector const& probabilities);
	std::unordered_set<long long> floyds_sampling_algorithm(long long l, long long r, size_t k);

	template<typename T>
	T quickselect(std::vector<T> arr, size_t k)
	{
		if (k >= arr.size())
			throw std::invalid_argument("k must be less than size of the array.");

		Random& random = get_random();
		size_t l = 0, r = arr.size() - 1;
		while (l < r)
		{
			size_t pivot_idx = random.uniform_int_distribution(l, r + 1);
			T pivot = arr[pivot_idx];
			std::swap(arr[pivot_idx], arr[r]);

			size_t i = l;
			for (size_t j = l; j < r; j++)
			{
				if (arr[j] < pivot)
				{
					std::swap(arr[i], arr[j]);
					i++;
				}
			}

			std::swap(arr[i], arr[r]);

			if (i == k)
				return arr[i];
			else if (k < i)
				r = i - 1;
			else // k > i
				l = i + 1;
		}

		return arr[l];
	}
}