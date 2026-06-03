#pragma once

#include "cppml/Vector.h"
#include "cppml/Matrix.h"

namespace cppml
{
	class LinearRegression
	{
	private:
		Vector beta_;

	public:
		LinearRegression();
		void fit(Matrix X, Vector const& y);
		Vector predict(Matrix X);
	};
}