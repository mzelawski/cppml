#pragma once

#include "cppml/Vector.h"
#include "cppml/Matrix.h"

namespace cppml
{
	class RidgeRegression
	{
	private:
		Vector beta_;
		double alpha_;

	public:
		RidgeRegression(double alpha);
		void fit(Matrix X, Vector const& y);
		Vector predict(Matrix X);
	};
}