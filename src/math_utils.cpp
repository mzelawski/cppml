#include "cppml/math_utils.h"
#include "cppml/Matrix.h"
#include "cppml/Random.h"

#include <stdexcept>
#include <cmath>
#include <utility>
#include <unordered_set>

namespace cppml
{
	Matrix cholesky_decomposition(Matrix const& A)
	{
		size_t n = A.shape().first;
		if (n != A.shape().second)
			throw std::invalid_argument("The matrix must be square.");

		Matrix L(n, n, 0.0);
		for (size_t i = 0; i < n; i++)
			for (size_t k = 0; k <= i; k++)
			{
				double sum_k = 0.0;
				for (size_t j = 0; j < k; j++)
					sum_k += L(i, j) * L(k, j);

				if (i == k)
				{
					double val = std::max(1e-12, A(i, i) - sum_k);
					L(i, k) = sqrt(val);
				}
				else
					L(i, k) = (A(i, k) - sum_k) / L(k, k);
			}

		return L;
	}

	Vector forward_substitution(Matrix const& L, Vector const& b)
	{
		size_t n = b.size();
		Vector y(n, 0.0);

		for (size_t i = 0; i < n; i++)
		{
			double sum = 0.0;
			for (size_t j = 0; j < i; j++)
				sum += L(i, j) * y[j];
			y[i] = (b[i] - sum) / L(i, i);
		}

		return y;
	}

	Vector backward_substitution(Matrix const& L, Vector const& y)
	{
		size_t n = y.size();
		Vector beta(n, 0.0);
		
		for (long long i = n - 1; i >= 0; i--)
		{
			double sum = 0.0;
			for (size_t j = i + 1; j < n; j++)
				sum += L(j, i) * beta[j];
			beta[i] = (y[i] - sum) / L(i, i);
		}

		return beta;
	}

	Vector solve_cholesky(Matrix const& A, Vector const& b)
	{
		Matrix L = cholesky_decomposition(A);
		Vector y = forward_substitution(L, b);
		return backward_substitution(L, y);
	}

	// sum(probabilities) should equal 1, though the function does not verify that
	size_t choose_random_element(Vector const& probabilities)
	{
		if (probabilities.size() == 0)
			throw std::invalid_argument("The vector can't be empty.");

		Random& random = get_random();
		double val = random.uniform_real_distribution(0.0, 1.0);

		double sum = 0.0;
		for (size_t i = 0; i < probabilities.size(); i++)
		{
			sum += probabilities[i];
			if (sum >= val)
				return i;
		}

		return probabilities.size() - 1;
	}

	// [)
	std::unordered_set<long long> floyds_sampling_algorithm(long long l, long long r, size_t k)
	{
		if (r - l < (long long)k)
			throw std::invalid_argument("Length of the range must be greater than or equal to number of sampled elements.");

		Random& random = get_random();

		std::unordered_set<long long> s;
		s.reserve(k);

		for (long long i = r - k; i < r; i++)
		{
			long long x = random.uniform_int_distribution(l, i + 1);
			if (s.count(x))
				s.insert(i);
			else
				s.insert(x);
		}

		return s;
	}
}