#pragma once

#include "cppml/Vector.h"
#include "cppml/Matrix.h"

#include <unordered_set>

namespace cppml
{
	Matrix cholesky_decomposition(Matrix const& A);
	Vector forward_substitution(Matrix const& L, Vector const& b);
	Vector backward_substitution(Matrix const& L, Vector const& b);
	Vector solve_cholesky(Matrix const& A, Vector const& b);

	size_t choose_random_element(Vector const& probabilities);
	std::unordered_set<long long> floyds_sampling_algorithm(long long l, long long r, size_t k);
}