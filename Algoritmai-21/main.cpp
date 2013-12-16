#include <vector>
#include <memory>
#include <random>
#include <functional>
#include <numeric>

using namespace std;

struct Element
{
	size_t key, p;
	Element(size_t key, size_t p) :key(key), p(p){}
};

struct Node
{
	size_t key;
	unique_ptr<Node> left, right;
	Node(size_t key, unique_ptr<Node> left, unique_ptr<Node> right) :key(key), left(move(left)), right(move(right)){}
};

void ComputeCRW(const vector<Element> &data, vector < vector < size_t >> &c, vector < vector < size_t >> &r, vector < vector < size_t >> &w);
unique_ptr<Node> ConstructTree(const vector < vector < size_t >> &c, const vector < vector < size_t >> &r, const vector < vector < size_t >> &w, size_t i, size_t j);

int main()
{
	const size_t n = 10;
	vector < vector < size_t >> c, r, w;
	vector<Element> data;
	default_random_engine gen;
	uniform_int_distribution<size_t> dist(0, 10);
	auto num = bind(dist, gen);
	for (size_t i = 1; i <= n+2; i++)
	{
		c.emplace_back();
		r.emplace_back();
		w.emplace_back();
		for (size_t j = 1; j <= n+1; j++)
		{
			c[i - 1].emplace_back();
			r[i - 1].emplace_back();
			w[i - 1].emplace_back();
		}
	}
	for (size_t i = 1; i <= n; i++)
	{
		size_t p = num();
		data.emplace_back(i, p);
	}
	ComputeCRW(data, c, r, w);
	auto tree = ConstructTree(c, r, w, 1, n);
	return 0;
}

void ComputeCRW(const vector<Element> &data, vector < vector < size_t >> &c, vector < vector < size_t >> &r, vector < vector < size_t >> &w)
{
	size_t j, max = numeric_limits<size_t>::max(), t, n = data.size();

	for (size_t i = 1; i <= n+1; i++)
	{
		c[i][i - 1] = 0;
		w[i][i - 1] = 0;
	}
	for (size_t l = 1; l <= n; l++)
	{
		for (size_t i = 1; i <= n - l + 1; i++)
		{
			j = i + l - 1;
			c[i][j] = max;
			w[i][j] = w[i][j - 1] + data[j-1].p;
			for (size_t m = i; m <= j; m++)
			{
				t = c[i][m - 1] + c[m + 1][j] + w[i][j];
				if (t < c[i][j])
				{
					c[i][j] = t;
					r[i][j] = m;
				}
			}
		}
	}
}

unique_ptr<Node> ConstructTree(const vector < vector < size_t >> &c, const vector < vector < size_t >> &r, const vector < vector < size_t >> &w, size_t i, size_t j)
{
	if (i <= j)
	{
		size_t root = r[i][j];
		return make_unique<Node>(root, ConstructTree(c, r, w, i, root - 1), ConstructTree(c, r, w, root + 1, j));
	}
	return nullptr;
}