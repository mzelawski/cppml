#include "cppml/Matrix.h"

#include "cppml/Vector.h"

#include <utility>
#include <vector>
#include <initializer_list>
#include <stdexcept>

namespace cppml
{
	Matrix::Matrix() : rows_(0), cols_(0) {}

	Matrix::Matrix(size_t rows, size_t cols, double val) : rows_(rows), cols_(cols), data_(rows*cols, val) {}

	Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list)
	{
		size_t rows = list.size();
		if (rows == 0)
		{
			rows_ = 0;
			cols_ = 0;
			return;
		}
		size_t cols = list.begin()->size();

		rows_ = rows;
		cols_ = cols;
		data_.resize(rows * cols);

		size_t i = 0;
		for (auto list_i : list)
		{
			if (list_i.size() != cols_)
				throw std::invalid_argument("Second dimension must have a constant size.");

			size_t j = 0;
			for (auto list_ij : list_i)
			{
				(*this)(i, j) = list_ij;
				j++;
			}

			i++;
		}
	}

	std::pair<size_t, size_t> Matrix::shape() const
	{
		return std::make_pair(rows_, cols_);
	}

	bool operator==(Matrix const& lhs, Matrix const& rhs)
	{
		return lhs.rows_ == rhs.rows_ && lhs.cols_ == rhs.cols_ && lhs.data_ == rhs.data_;
	}

	bool operator!=(Matrix const& lhs, Matrix const& rhs)
	{
		return !(lhs.rows_ == rhs.rows_ && lhs.cols_ == rhs.cols_ && lhs.data_ == rhs.data_);
	}

	double& Matrix::operator()(size_t a, size_t b)
	{
		return data_[a * cols_ + b];
	}

	double const& Matrix::operator()(size_t a, size_t b) const
	{
		return data_[a * cols_ + b];
	}

	Matrix& Matrix::operator+=(Matrix const& other)
	{
		if (shape() != other.shape())
			throw std::invalid_argument("Dimensions of both matrices must match.");

		for (size_t i = 0; i < data_.size(); i++)
			data_[i] += other.data_[i];
		
		return *this;
	}

	Matrix& Matrix::operator-=(Matrix const& other)
	{
		if (shape() != other.shape())
			throw std::invalid_argument("Dimensions of both matrices must match.");

		for (size_t i = 0; i < data_.size(); i++)
			data_[i] -= other.data_[i];

		return *this;
	}

	Matrix& Matrix::operator*=(Matrix const& other)
	{
		if (shape() != other.shape())
			throw std::invalid_argument("Dimensions of both matrices must match.");

		for (size_t i = 0; i < data_.size(); i++)
			data_[i] *= other.data_[i];

		return *this;
	}

	Matrix& Matrix::operator/=(Matrix const& other)
	{
		if (shape() != other.shape())
			throw std::invalid_argument("Dimensions of both matrices must match.");

		for (size_t i = 0; i < data_.size(); i++)
			data_[i] /= other.data_[i];

		return *this;
	}

	Matrix Matrix::transpose() const
	{
		Matrix res(cols_, rows_, 0.0);
		for (size_t i = 0; i < rows_; i++)
			for (size_t j = 0; j < cols_; j++)
				res(j, i) = (*this)(i, j);

		return res;
	}

	Matrix Matrix::insert_row(size_t idx, Vector vals)
	{
		if (idx > rows_)
			throw std::invalid_argument("The index must be less than or equal to the number of rows");
		if (vals.size() != cols_)
			throw std::invalid_argument("Size of the vector must be equal to the number of columns.");

		Matrix res(rows_ + 1, cols_, 0.0);
		for (size_t i = 0; i < rows_ + 1; i++)
			for (size_t j = 0; j < cols_; j++)
			{
				if (i < idx)
					res(i, j) = (*this)(i, j);
				else if (i == idx)
					res(i, j) = vals[j];
				else // i > idx
					res(i, j) = (*this)(i - 1, j);
			}

		return res;
	}

	Matrix Matrix::insert_column(size_t idx, Vector vals)
	{
		if (idx > cols_)
			throw std::invalid_argument("The index must be less than or equal to the number of columns");
		if (vals.size() != rows_)
			throw std::invalid_argument("Size of the vector must be equal to the number of rows.");

		Matrix res(rows_, cols_ + 1, 0.0);
		for (size_t i = 0; i < rows_; i++)
			for (size_t j = 0; j < cols_ + 1; j++)
			{
				if (j < idx)
					res(i, j) = (*this)(i, j);
				else if (j == idx)
					res(i, j) = vals[i];
				else // j > idx
					res(i, j) = (*this)(i, j - 1);
			}

		return res;
	}

	Matrix operator+(Matrix lhs, Matrix const& rhs)
	{
		return lhs += rhs;
	}

	Matrix operator-(Matrix lhs, Matrix const& rhs)
	{
		return lhs -= rhs;
	}

	Matrix operator*(Matrix lhs, Matrix const& rhs)
	{
		return lhs *= rhs;
	}

	Matrix operator/(Matrix lhs, Matrix const& rhs)
	{
		return lhs /= rhs;
	}

	Matrix matmul(Matrix const& lhs, Matrix const& rhs)
	{
		auto [n, l] = lhs.shape();
		size_t m = rhs.shape().second;
		if (l != rhs.shape().first)
			throw std::invalid_argument("The number of columns of the left matrix must be equal to the number of rows of the right one.");

		Matrix res(n, m, 0.0);
		for (size_t i = 0; i < n; i++)
			for (size_t k = 0; k < l; k++)
			{
				double lhs_ik = lhs(i, k);
				for (size_t j = 0; j < m; j++)
					res(i, j) += lhs_ik * rhs(k, j);
			}


		return res;
	}

	Vector matmul(Matrix const& lhs, Vector const& rhs)
	{
		auto [n, m] = lhs.shape();
		if (m != rhs.size())
			throw std::invalid_argument("The number of columns of the matrix must be equal to the size of the vector.");

		Vector res(n, 0.0);
		for (size_t i = 0; i < n; i++)
		{
			double& res_i = res[i];
			for (size_t j = 0; j < m; j++)
				res_i += lhs(i, j) * rhs[j];
		}

		return res;
	}

	Vector matmul(Vector const& lhs, Matrix const& rhs)
	{
		auto [n, m] = rhs.shape();
		if (n != lhs.size())
			throw std::invalid_argument("The number of rows of the matrix must be equal to the size of the vector.");

		Vector res(m, 0.0);
		for (size_t i = 0; i < n; i++)
		{
			double lhs_i = lhs[i];
			for (size_t j = 0; j < m; j++)
				res[j] += lhs_i * rhs(i, j);
		}

		return res;
	}

	std::ostream& operator<<(std::ostream& os, Matrix const& x)
	{
		auto [n, m] = x.shape();
		for (size_t i = 0; i < n; i++)
		{
			for (size_t j = 0; j < m; j++)
				os << x(i, j) << ' ';
			os << '\n';
		}

		return os;
	}
}