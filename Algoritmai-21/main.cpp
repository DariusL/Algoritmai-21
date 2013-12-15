#include <vector>
#include <memory>
#include <random>
#include <functional>

using namespace std;

struct Element
{
	size_t key, p, q;
	Element(size_t key, size_t p, size_t q) :key(key), p(p), q(q){}
};

struct Node
{
	size_t key;
	unique_ptr<Node> left, right;
	Node(size_t key, unique_ptr<Node> left, unique_ptr<Node> right) :key(key), left(move(left)), right(move(right)){}
};

void ComputeCRW(const vector<Element> &data, vector < vector < size_t >> &c, vector < vector < size_t >> &r, vector < vector < size_t >> &w);
unique_ptr<Node> ConstructTree(const vector < vector < size_t >> &c, const vector < vector < size_t >> &r, const vector < vector < size_t >> &w, size_t i, size_t j);

int clamp(int n, int min);

int main()
{
	const size_t n = 6;
	vector < vector < size_t >> c, r, w;
	vector<Element> data;
	default_random_engine gen;
	uniform_int_distribution<size_t> dist(0, 10);
	auto num = bind(dist, gen);
	for (size_t i = 0; i < n; i++)
	{
		c.emplace_back();
		r.emplace_back();
		w.emplace_back();
		for (size_t j = 0; j < n; j++)
		{
			c[i].emplace_back();
			r[i].emplace_back();
			w[i].emplace_back();
		}
		size_t p = num();
		size_t q = num();
		data.emplace_back(i, p, q);
	}
	ComputeCRW(data, c, r, w);
	auto tree = ConstructTree(c, r, w, 0, n - 1);
	return 0;
}

void ComputeCRW(const vector<Element> &data, vector < vector < size_t >> &c, vector < vector < size_t >> &r, vector < vector < size_t >> &w)
{
	size_t n = data.size(), j, m, x, min;

	//Construct weight matrix W 
	for (size_t i = 0; i < n; i++)
	{
		w[i][i] = data[i].q;
		for (size_t j = i + 1; j < n; j++)
		{
			w[i][j] = w[i][j - 1] + data[j].p + data[j].q;
		}
	}

	//Construct cost matrix C and root matrix R 
	for (size_t i = 0; i < n; i++)
	{
		c[i][i] = w[i][i];
	}
	for (size_t i = 0; i < n - 1; i++)
	{
		j = i + 1;
		c[i][j] = c[i][i] + c[j][j] + w[i][j];
		r[i][j] = j;
	}
	for (size_t h = 2; h < n; h++)
	{
		for (size_t i = 0; i < n - h; i++)
		{
			j = i + h;
			m = r[i][j - 1];
			min = c[i][m - 1] + c[m][j];
			for (size_t k = m + 1; k < r[i + 1][j]; k++)
			{
				x = c[i][k - 1] + c[k][j];
				if (x < min)
				{
					m = k;
					min = x;
				}
			}
			c[i][j] = w[i][j] + min;
			r[i][j] = m;
		}
	}
}

unique_ptr<Node> ConstructTree(const vector < vector < size_t >> &c, const vector < vector < size_t >> &r, const vector < vector < size_t >> &w, size_t i, size_t j)
{
	if (i != j)
	{
		size_t root = r[i][j];
		return make_unique<Node>(root, ConstructTree(c, r, w, i, root - 1), ConstructTree(c, r, w, root, j));
	}
	return nullptr;
}

int clamp(int n, int min)
{
	if (n > min)
		return n;
	else
		return min;
}