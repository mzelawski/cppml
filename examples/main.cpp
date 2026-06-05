#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/LinearRegression.h"
#include "cppml/Random.h"
#include "cppml/math_utils.h"

#include <iostream>
#include <chrono>
#include <functional>

long long measure_time(std::function<void()> const& f)
{
	auto start = std::chrono::steady_clock::now();
	f();
	auto end = std::chrono::steady_clock::now();
	auto dif = end - start;
	return dif.count();
}

int main()
{
	cppml::Vector v = { 6, 8 };
	std::cout << v.norm(2);
}