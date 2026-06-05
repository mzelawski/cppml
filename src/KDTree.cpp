#include "cppml/KDTree.h"

#include "cppml/Vector.h"

#include <algorithm>
#include <utility>

namespace cppml
{
	void KDTree::build(Node* v, std::vector<size_t> const& indices)
	{
		if (indices.size() <= leaf_size_)
		{
			for (size_t idx : indices)
				v->indices.push_back(idx);
			return;
		}

		std::vector<double> mx(points_.front().size(), -1e300), mn(points_.front().size(), 1e300);
		for (size_t i = 0; i < indices.size(); i++)
			for (size_t j = 0; j < points_.front().size(); j++)
			{
				mx[j] = std::max(mx[j], points_[indices[i]][j]);
				mn[j] = std::min(mn[j], points_[indices[i]][j]);
			}

		std::vector<double> diffs(points_.front().size());
		for (size_t i = 0; i < diffs.size(); i++)
			diffs[i] = mx[i] - mn[i];

		v->d = std::max_element(diffs.begin(), diffs.end()) - diffs.begin();
		if (diffs[v->d] == 0.0)
		{
			for (size_t idx : indices)
				v->indices.push_back(idx);
			return;
		}

		std::vector<double> points_d(indices.size());
		for (size_t i = 0; i < points_d.size(); i++)
			points_d[i] = points_[indices[i]][v->d];

		double median = quickselect(points_d, points_d.size() / 2);
		
		std::vector<size_t> indices_left, indices_right;
		for (size_t idx : indices)
		{
			double p_d = points_[idx][v->d];
			if (v->indices.size() == 0 && p_d == median)
				v->indices.push_back(idx);
			else if (p_d < median)
				indices_left.push_back(idx);
			else
				indices_right.push_back(idx);
		}

		if (!indices_left.empty())
		{
			v->left = new Node;
			build(v->left, indices_left);
		}

		if (!indices_right.empty())
		{
			v->right = new Node;
			build(v->right, indices_right);
		}
	}

	void KDTree::copy(Node* v, Node* u)
	{
		v->indices = u->indices;
		v->d = u->d;
		v->pivot = u->pivot;

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

	void KDTree::free(Node* v)
	{
		if (v->left)
			free(v->left);
		if (v->right)
			free(v->right);
		delete v;
	}

	KDTree::KDTree(Matrix const& X, size_t leaf_size) : leaf_size_(leaf_size), root_(new Node)
	{
		auto [n, m] = X.shape();

		points_ = std::vector<Vector>(n, Vector(m, 0.0));
		for (size_t i = 0; i < n; i++)
			for (size_t j = 0; j < m; j++)
				points_[i][j] = X(i, j);

		std::vector<size_t> indices(n);
		for (size_t i = 1; i < n; i++)
			indices[i] = i;

		build(root_, indices);
	}

	KDTree::KDTree(KDTree const& other) : leaf_size_(other.leaf_size_), root_(new Node), points_(other.points_)
	{
		copy(root_, other.root_);
	}

	KDTree& KDTree::operator=(KDTree const& other)
	{
		if (this == &other)
			return *this;

		leaf_size_ = other.leaf_size_;
		points_ = other.points_;

		free(root_);
		root_ = new Node;

		copy(root_, other.root_);

		return *this;
	}

	KDTree::~KDTree()
	{
		free(root_);
	}
}