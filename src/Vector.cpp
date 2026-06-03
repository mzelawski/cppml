#include "cppml/Vector.h"

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <ostream>

namespace cppml
{
	Vector::Vector() {}

	Vector::Vector(size_t size, double val) : data_(size, val) {}

	Vector::Vector(std::initializer_list<double> list) : data_(list) {}

	size_t Vector::size() const
	{
		return data_.size();
	}

	bool operator==(Vector const& lhs, Vector const& rhs)
	{
		return lhs.data_ == rhs.data_;
	}

	bool operator!=(Vector const& lhs, Vector const& rhs)
	{
		return lhs.data_ != rhs.data_;
	}

	double& Vector::operator[](size_t idx)
	{
		return data_[idx];
	}

	double const& Vector::operator[](size_t idx) const
	{
		return data_[idx];
	}

	Vector& Vector::operator+=(Vector const& other)
	{
		if (size() != other.size())
			throw std::invalid_argument("Vector sizes must match.");

		for (size_t i = 0; i < other.size(); i++)
			data_[i] += other[i];

		return *this;
	}

	Vector& Vector::operator-=(Vector const& other)
	{
		if (size() != other.size())
			throw std::invalid_argument("Vector sizes must match.");

		for (size_t i = 0; i < other.size(); i++)
			data_[i] -= other[i];

		return *this;
	}

	Vector& Vector::operator*=(Vector const& other)
	{
		if (size() != other.size())
			throw std::invalid_argument("Vector sizes must match.");

		for (size_t i = 0; i < other.size(); i++)
			data_[i] *= other[i];

		return *this;
	}

	Vector& Vector::operator/=(Vector const& other)
	{
		if (size() != other.size())
			throw std::invalid_argument("Vector sizes must match.");

		for (size_t i = 0; i < other.size(); i++)
			data_[i] /= other[i];

		return *this;
	}

	Vector Vector::insert(size_t idx, double val)
	{
		if (idx > size())
			throw std::invalid_argument("The index must be less than or equal to the size.");
		
		Vector res(size() + 1, 0.0);
		for (size_t i = 0; i < res.size(); i++)
		{
			if (i < idx)
				res[i] = data_[i];
			else if (i == idx)
				res[i] = val;
			else // i > idx
				res[i] = data_[i - 1];
		}

		return res;
	}

	Vector operator+(Vector lhs, Vector const& rhs)
	{
		return lhs += rhs;
	}

	Vector operator-(Vector lhs, Vector const& rhs)
	{
		return lhs -= rhs;
	}

	Vector operator*(Vector lhs, Vector const& rhs)
	{
		return lhs *= rhs;
	}

	Vector operator/(Vector lhs, Vector const& rhs)
	{
		return lhs /= rhs;
	}

	double matmul(Vector const& lhs, Vector const& rhs)
	{
		if (lhs.size() != rhs.size())
			throw std::invalid_argument("Vector sizes must match.");

		double res = 0.0;
		for (size_t i = 0; i < lhs.size(); i++)
			res += lhs[i] * rhs[i];

		return res;
	}

	std::ostream& operator<<(std::ostream& os, Vector const& v)
	{
		for (size_t i = 0; i < v.size(); i++)
			os << v[i] << ' ';
		os << '\n';
		return os;
	}
}