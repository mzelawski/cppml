#pragma once

#include "cppml/Vector.h"

#include <utility>
#include <vector>
#include <initializer_list>
#include <ostream>

namespace cppml
{
	class Matrix
	{
	private:
		size_t rows_, cols_;
		std::vector<double> data_;

	public:
		Matrix();
		Matrix(size_t rows, size_t cols, double val);
		Matrix(std::initializer_list<std::initializer_list<double>> list);
		std::pair<size_t, size_t> shape() const;
		friend bool operator==(Matrix const& lhs, Matrix const& rhs);
		friend bool operator!=(Matrix const& lhs, Matrix const& rhs);
		double& operator()(size_t a, size_t b);
		double const& operator()(size_t a, size_t b) const;
		Matrix& operator+=(Matrix const& other);
		Matrix& operator-=(Matrix const& other);
		Matrix& operator*=(Matrix const& other);
		Matrix& operator/=(Matrix const& other);
		Matrix transpose() const;
		Matrix insert_row(size_t idx, Vector vals) const;
		Matrix insert_column(size_t idx, Vector vals) const;
	};

	Matrix operator+(Matrix lhs, Matrix const& rhs);
	Matrix operator-(Matrix lhs, Matrix const& rhs);
	Matrix operator*(Matrix lhs, Matrix const& rhs);
	Matrix operator/(Matrix lhs, Matrix const& rhs);
	Matrix matmul(double lhs, Matrix const& rhs);
	Matrix matmul(Matrix const& lhs, double rhs);
	Matrix matmul(Matrix const& lhs, Matrix const& rhs);
	Vector matmul(Matrix const& lhs, Vector const& rhs);
	Vector matmul(Vector const& lhs, Matrix const& rhs);
	std::ostream& operator<<(std::ostream& os, Matrix const& x);
	Matrix identity_matrix(size_t n);
}