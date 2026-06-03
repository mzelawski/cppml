#pragma once

#include <random>

namespace cppml
{
	class Random
	{
	private:
		std::mt19937 gen_;

	public:
		Random();
		void change_seed(unsigned int seed);
		double uniform_real_distribution(double a, double b);
		long long uniform_int_distribution(long long a, long long b);
	};

	Random& get_random();
}