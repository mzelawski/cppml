#include "cppml/LinearRegression.h"

#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/math_utils.h"

namespace cppml
{
	LinearRegression::LinearRegression() : beta_(0, 0.0) {}

	void LinearRegression::fit(Matrix X, Vector const& y)
	{
		X = X.insert_column(0, Vector(X.shape().first, 1.0));
		Matrix X_T = X.transpose();
		beta_ = solve_cholesky(matmul(X_T, X), matmul(X_T, y));
	}

	Vector LinearRegression::predict(Matrix X)
	{
		X = X.insert_column(0, Vector(X.shape().first, 1.0));
		return matmul(X, beta_);
	}
}