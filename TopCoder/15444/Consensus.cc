#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class Consensus {

public:

	double expectedTime(vector <int> opinions) {
		int N = 0;
		for (int v : opinions) N += v;
		assert(N <= 3000);
		vector<pair<double, double>> t;
		t.emplace_back(0, 0);
		t.emplace_back(1, 0);

		for (int i = 1; i < N; i++) {
			pair<double, double> cur = t[i];
			cur.first *= 2;
			cur.second *= 2;
			cur.first -= t[i-1].first;
			cur.second -= t[i-1].second;

			cur.second -= double(N-1) / double(N - i);

			t.emplace_back(cur);
		}

		double sol = t.back().second / -t.back().first;
		cerr << N << ' ' << sol << ' ' << double(N-1) * double(N-1) / double(N) << '\n';
		for (int i = 0; i <= N; i++) {
			t[i].second += t[i].first * sol;
			t[i].first = 0;
		}

		double res = 0;
		for (int v : opinions) {
			res += t[v].second;
		}
		return res;
	}
};

void my_tests() {
	Consensus c;
	c.expectedTime(vector<int>(50, 50));
}

}  // namespace ecnerwala

using ecnerwala::Consensus;

// vim:ft=cpp
