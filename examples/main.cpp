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
	auto& rnd = cppml::get_random();
	rnd.change_seed(67);
	auto s = cppml::floyds_sampling_algorithm(-10, 10, 5);
	int a = 0;
	int x = 0;
	std::cout << 67;
}