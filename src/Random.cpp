#include "cppml/Random.h"

#include <random>

namespace cppml
{
	// random seed by default
	Random::Random()
	{
		std::random_device rd;
		gen_.seed(rd());
	}

	void Random::change_seed(unsigned int seed)
	{
		gen_.seed(seed);
	}

	// [)
	double Random::uniform_real_distribution(double a, double b)
	{
		std::uniform_real_distribution urd(a, b);
		return urd(gen_);
	}

	// [)
	long long Random::uniform_int_distribution(long long a, long long b)
	{
		std::uniform_int_distribution uid(a, b - 1);
		return uid(gen_);
	}

	Random& get_random()
	{
		static Random instance;
		return instance;
	}
}