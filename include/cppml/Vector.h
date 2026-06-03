#pragma once

#include <vector>
#include <initializer_list>
#include <ostream>

namespace cppml
{
	class Vector
	{
	private:
		std::vector<double> data_;

	public:
		Vector();
		Vector(size_t size, double val);
		Vector(std::initializer_list<double> list);
		size_t size() const;
		friend bool operator==(Vector const& lhs, Vector const& rhs);
		friend bool operator!=(Vector const& lhs, Vector const& rhs);
		double& operator[](size_t idx);
		double const& operator[](size_t idx) const;
		Vector& operator+=(Vector const& other);
		Vector& operator-=(Vector const& other);
		Vector& operator*=(Vector const& other);
		Vector& operator/=(Vector const& other);
		Vector insert(size_t idx, double val);
	};

	Vector operator+(Vector lhs, Vector const& rhs);
	Vector operator-(Vector lhs, Vector const& rhs);
	Vector operator*(Vector lhs, Vector const& rhs);
	Vector operator/(Vector lhs, Vector const& rhs);
	double matmul(Vector const& lhs, Vector const& rhs);
	std::ostream& operator<<(std::ostream& os, Vector const& v);
}