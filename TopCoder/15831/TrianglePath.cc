#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

class TrianglePath {

public:

	vector<int> go(const vector<pair<ll, ll>>& pts, ll Xmax, ll Ymax) {
		int N = int(pts.size());
		for (auto it : pts) {
			assert(0 <= it.first && 0 <= it.second && it.first * Ymax + it.second * Xmax <= Xmax * Ymax);
		}
		if (N == 0) return vector<int>({});
		if (*min_element(pts.begin(), pts.end()) == *max_element(pts.begin(), pts.end())) {
			vector<int> res(N);
			for (int i = 0; i < N; i++) res[i] = i;
			return res;
		}
		vector<pair<ll, ll>> v0;
		vector<int> i0;
		vector<pair<ll, ll>> v1;
		vector<int> i1;
		vector<pair<ll, ll>> v2;
		vector<int> i2;
		vector<pair<ll, ll>> v3;
		vector<int> i3;
		for (int i = 0; i < N; i++) {
			ll x = pts[i].first * 2;
			ll y = pts[i].second * 2;
			if (x > Xmax) {
				v0.push_back({x - Xmax, y});
				i0.push_back(i);
			} else if (y > Ymax) {
				v3.push_back({x, y - Ymax});
				i3.push_back(i);
			} else if (x * Ymax >= y * Xmax) {
				v1.push_back({y, Xmax - x});
				i1.push_back(i);
			} else {
				v2.push_back({Ymax - y, x});
				i2.push_back(i);
			}
		}
		vector<int> res;
		for (auto i : go(v0, Xmax, Ymax)) { res.push_back(i0[i]); }
		for (auto i : go(v1, Ymax, Xmax)) { res.push_back(i1[i]); }
		for (auto i : go(v2, Ymax, Xmax)) { res.push_back(i2[i]); }
		for (auto i : go(v3, Xmax, Ymax)) { res.push_back(i3[i]); }
		return res;
	}

	vector <int> construct(int N, int Xmax, int Ymax, vector <int> Xprefix, vector <int> Yprefix, int seed) {
		vector<pair<ll, ll>> pts;
		for (int i = 0; i < int(Xprefix.size()); i++) {
			pts.push_back({Xprefix[i], Yprefix[i]});
		}
		ll state = seed;
		while (int(pts.size()) < N) {
			state = (state * 1103515245 + 12345) % (1ll << 31);
			ll xnew = state % (Xmax + 1);
			state = (state * 1103515245 + 12345) % (1ll << 31);
			ll ynew = state % (Ymax + 1);
			if (xnew * Ymax + ynew * Xmax <= 1ll * Xmax * Ymax) {
				pts.push_back({xnew, ynew});
			}
		}
		return go(pts, Xmax, Ymax);
	}
};

void my_tests() {
	// FILL IN TESTS
	
	vector<pair<ll, ll>> inputs;
	for (int i = 0; i < 3000; i++) {
		inputs.push_back({0,i});
	}
	auto res = TrianglePath().go(inputs, int(1e6), int(1e6));

	TrianglePath().construct(3000, int(1e6), int(1e6), {}, {}, 42);
	TrianglePath().construct(3000, int(1e6), int(1e6), {}, {}, 60);
}

}  // namespace ecnerwala

using ecnerwala::TrianglePath;

// vim:ft=cpp
