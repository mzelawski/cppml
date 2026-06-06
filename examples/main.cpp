#include "cppml/Vector.h"
#include "cppml/Matrix.h"
#include "cppml/LinearRegression.h"
#include "cppml/Random.h"
#include "cppml/math_utils.h"
#include "cppml/KDTree.h"
#include "cppml/BallTree.h"
#include "cppml/KNN.h"

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
	cppml::Matrix X = { { 0, 3 }, { 2, 3 }, { 2, 5 }, { 3, 0 }, { 6, 7 }, { 7, 4 } };
	std::vector<unsigned long long> y = { 0, 1, 1, 2, 0, 1 };
	//cppml::KDTree t(X, 1, 2);
	//cppml::BallTree r(X, 1, 2);

	//auto v = t.query({ 7, 5 }, 3);
	//auto u = r.query({ 7, 5 }, 3);

	cppml::KNN knn(3, cppml::KNN::DataStructure::BallTree, 1, 2);
	knn.fit(X, y);

	auto pred = knn.predict({ { 0, 0 } });

	int x = 0;
}