#include "cppml/BallTree.h"

#include "cppml/Vector.h"

#include <algorithm>
#include <queue>
#include <utility>
#include <cmath>
#include <stdexcept>

namespace cppml
{
	size_t BallTree::farthest(Vector const& point, std::vector<size_t> const& indices)
	{
		double max_dist = -1.0;
		size_t res = 0;
		for (size_t i = 0; i < indices.size(); i++)
		{
			double dist = distance(points_[indices[i]], point, norm_);
			if (dist > max_dist)
			{
				max_dist = dist;
				res = indices[i];
			}
		}

		return res;
	}

	void BallTree::build(Node* v, std::vector<size_t> const& indices)
	{
		if (indices.size() <= leaf_size_)
		{
			v->indices = indices;
			return;
		}

		Vector centroid(points_.front().size(), 0.0);
		for (size_t idx : indices)
			centroid += points_[idx];
		centroid = matmul(1.0 / (double)indices.size(), centroid);

		v->left_pivot_idx = farthest(centroid, indices);
		v->right_pivot_idx = farthest(points_[v->left_pivot_idx], indices);
		std::vector<size_t> indices_left, indices_right;
		double left_radius = 0.0, right_radius = 0.0;
		bool toggle = false;
		for (size_t idx : indices)
		{
			double left_dist = distance(points_[v->left_pivot_idx], points_[idx], norm_);
			double right_dist = distance(points_[v->right_pivot_idx], points_[idx], norm_);

			if (left_dist < right_dist || left_dist == right_dist && toggle)
			{
				indices_left.push_back(idx);
				left_radius = std::max(left_radius, left_dist);

				if (left_dist == right_dist)
					toggle = !toggle;
			}
			else 
			{
				indices_right.push_back(idx);
				right_radius = std::max(right_radius, right_dist);

				if (left_dist == right_dist)
					toggle = !toggle;
			}
		}

		if (!indices_left.empty())
		{
			v->left = new Node;
			v->left->radius = left_radius;
			build(v->left, indices_left);
		}

		if (!indices_right.empty())
		{
			v->right = new Node;
			v->right->radius = right_radius;
			build(v->right, indices_right);
		}
	}

	void BallTree::copy(Node* v, Node* u)
	{
		v->indices = u->indices;
		v->left_pivot_idx = u->left_pivot_idx;
		v->right_pivot_idx = u->right_pivot_idx;

		if (u->left)
		{
			v->left = new Node;
			copy(v->left, u->left);
		}
		if (u->right)
		{
			v->right = new Node;
			copy(v->right, u->right);
		}
	}

	void BallTree::free(Node* v)
	{
		if (v->left)
			free(v->left);
		if (v->right)
			free(v->right);
		delete v;
	}

	void BallTree::query_node(Node* v, Vector const& point, std::priority_queue<std::pair<double, size_t>>& pq, size_t neighbors) const
	{
		if (v->left && v->right)
		{
			double left_dist = distance(points_[v->left_pivot_idx], point, norm_);
			double right_dist= distance(points_[v->right_pivot_idx], point, norm_);

			if (left_dist < right_dist)
			{
				query_node(v->left, point, pq, neighbors);
				if (pq.size() < neighbors || right_dist - v->right->radius < pq.top().first)
					query_node(v->right, point, pq, neighbors);
			}
			else
			{
				query_node(v->right, point, pq, neighbors);
				if (pq.size() < neighbors || left_dist - v->left->radius < pq.top().first)
					query_node(v->left, point, pq, neighbors);
			}
		}
		else if (v->left && !v->right)
			query_node(v->left, point, pq, neighbors);
		else if (!v->left && v->right)
			query_node(v->right, point, pq, neighbors);

		for (size_t idx : v->indices)
		{
			double dist = distance(points_[idx], point, norm_);
			if (pq.size() < neighbors || dist < pq.top().first)
			{
				pq.push({ dist, idx });
				if (pq.size() > neighbors)
					pq.pop();
			}
		}
	}

	BallTree::BallTree() : leaf_size_(0), norm_(0), root_(new Node) {}

	BallTree::BallTree(Matrix const& X, size_t leaf_size, long long norm) : leaf_size_(leaf_size), norm_(norm), root_(new Node), points_(X.shape().first, Vector(X.shape().second, 0.0))
	{
		auto [n, m] = X.shape();

		for (size_t i = 0; i < n; i++)
			for (size_t j = 0; j < m; j++)
				points_[i][j] = X(i, j);

		std::vector<size_t> indices(n);
		for (size_t i = 1; i < n; i++)
			indices[i] = i;

		build(root_, indices);
	}

	BallTree::BallTree(BallTree const& other) : leaf_size_(other.leaf_size_), norm_(other.norm_), root_(new Node), points_(other.points_)
	{
		copy(root_, other.root_);
	}

	BallTree& BallTree::operator=(BallTree const& other)
	{
		if (this == &other)
			return *this;

		leaf_size_ = other.leaf_size_;
		norm_ = other.norm_;
		points_ = other.points_;

		free(root_);
		root_ = new Node;

		copy(root_, other.root_);

		return *this;
	}

	BallTree::~BallTree()
	{
		free(root_);
	}

	std::vector<size_t> BallTree::query(Vector const& point, size_t neighbors) const
	{
		if (neighbors > points_.size())
			throw std::invalid_argument("neighbors must equal to or less than the number of points.");

		std::priority_queue<std::pair<double, size_t>> pq;
		query_node(root_, point, pq, neighbors);
		std::vector<size_t> res;
		res.reserve(neighbors);
		while (!pq.empty())
		{
			res.push_back(pq.top().second);
			pq.pop();
		}

		return res;
	}
}