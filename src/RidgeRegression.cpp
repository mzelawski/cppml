#include "cppml/RidgeRegression.h"

#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/math_utils.h"

namespace cppml
{
	RidgeRegression::RidgeRegression(double alpha) : beta_(0, 0.0), alpha_(alpha) {}

	void RidgeRegression::fit(Matrix X, Vector const& y)
	{
		X = X.insert_column(0, Vector(X.shape().first, 1.0));
		size_t m = X.shape().second;

		Matrix X_T = X.transpose();

		Matrix M = Matrix(m, m, 0.0);
		for (size_t i = 1; i < m; i++)
			M(i, i) = alpha_;

		beta_ = solve_cholesky(matmul(X_T, X) + M, matmul(X_T, y));
	}

	Vector RidgeRegression::predict(Matrix X)
	{
		X = X.insert_column(0, Vector(X.shape().first, 1.0));
		return matmul(X, beta_);
	}
}