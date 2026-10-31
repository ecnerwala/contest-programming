#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class RulerMaker {

public:

	void check(int n, vector<int> v) {
		assert(int(v.size()) == n);
		for (int i = 0; i < n; i++) {
			assert(0 <= v[i] && v[i] <= int(1e6));
		}
		for (int i = 1; i < n; i++) {
			assert(v[i] > v[i-1]);
		}
		int V = (n * n + 3) / 4;
		vector<bool> seen(V+1);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				int d = abs(v[i] - v[j]);
				if (d <= V) seen[d] = true;
			}
		}
		for (int i = 1; i <= V; i++) {
			assert(seen[i]);
		}
	}

	vector <int> placeStickers(int n) {
		int a = n / 2;
		vector<int> res;
		for (int i = 0; i < a; i++) {
			res.push_back(i);
		}
		for (int i = a; i < n; i++) {
			res.push_back((i-a) * (a+1) + a);
		}
		return res;
	}
};

void my_tests() {
	for (int n = 2; n <= 1000; n++) {
		RulerMaker r;
		auto v = r.placeStickers(n);
		cerr << n << '\n';
		//for (int i : v) { cerr << ' ' << i; } cerr << '\n';
		r.check(n, v);
	}
}

}  // namespace ecnerwala

using ecnerwala::RulerMaker;

// vim:ft=cpp
