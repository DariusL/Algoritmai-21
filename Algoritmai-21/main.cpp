#include <vector>
#include <memory>
#include <random>
#include <functional>
#include <numeric>
#include <fstream>
#include <ctime>
#include <iostream>

using namespace std;

size_t ops = 0;

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

void ComputeCRW(const vector<Element> &data, vector < vector < size_t >> &r);
unique_ptr<Node> ConstructTree(const vector < vector < size_t >> &r, size_t i, size_t j);

int main()
{
	const size_t counts[] = { 10, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
	ofstream out("rez.csv", ios::trunc);
	size_t time;
	for (auto &count : counts)
	{
		ops = 0;
		vector < vector < size_t >> r;
		vector<Element> data;
		default_random_engine gen;
		uniform_int_distribution<size_t> dist(0, 100);
		auto num = bind(dist, gen);

		for (size_t i = 1; i <= count; i++)
		{
			size_t p = num();
			data.emplace_back(i, p);
		}
		time = clock();
		ComputeCRW(data, r);
		ConstructTree(r, 1, count);
		time = clock() - time;
		cout << " count " << count << " time " << time << " ops " << ops << endl;
		out << count << ';' << time << ';' << ops << ";\n";
	}
	return 0;
}

void ComputeCRW(const vector<Element> &data, vector < vector < size_t >> &r)
{
	size_t j, max = numeric_limits<size_t>::max(), t, n = data.size();
	vector<vector<size_t>> c(n + 2, vector<size_t>(n + 1)),
		w(n + 2, vector<size_t>(n + 1));
	r.clear();
	r = vector<vector<size_t>>(n + 2, vector<size_t>(n + 1));
	ops += 4;

	for (size_t i = 1; i <= n+1; i++)
	{
		c[i][i - 1] = 0;
		w[i][i - 1] = 0;
		ops += 3;
	}
	for (size_t l = 1; l <= n; l++)
	{
		ops += 2;
		for (size_t i = 1; i <= n - l + 1; i++)
		{
			ops += 6;
			j = i + l - 1;
			c[i][j] = max;
			w[i][j] = w[i][j - 1] + data[j-1].p;
			for (size_t m = i; m <= j; m++)
			{
				ops += 5;
				t = c[i][m - 1] + c[m + 1][j] + w[i][j];
				if (t < c[i][j])
				{
					ops += 3;
					c[i][j] = t;
					r[i][j] = m;
				}
			}
		}
	}
}

unique_ptr<Node> ConstructTree(const vector < vector < size_t >> &r, size_t i, size_t j)
{
	ops += 2;
	if (i <= j)
	{
		ops += 3;
		size_t root = r[i][j];
		return make_unique<Node>(root, ConstructTree(r, i, root - 1), ConstructTree(r, root + 1, j));
	}
	return nullptr;
}