#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ProposalOptimization {

	int R, C, K;
	vector<int> roses;
	vector<int> tulips;
	vector<int> costs;
	int dest;
	vector<vector<tuple<int, int, int>>> opts;
	void recurse(int cur_x, int cur_y, tuple<int, int, int> cur_sum) {
		get<0>(cur_sum) += costs[cur_x * C + cur_y];
		get<1>(cur_sum) += roses[cur_x * C + cur_y];
		get<2>(cur_sum) += tulips[cur_x * C + cur_y];
		if (cur_x + cur_y == dest) {
			opts[cur_x].push_back(cur_sum);
			return;
		}
		if (cur_x + 1 < R) recurse(cur_x+1, cur_y, cur_sum);
		if (cur_y + 1 < C) recurse(cur_x, cur_y+1, cur_sum);
	}

	void recurse_once() {
		opts.assign(R, {});
		recurse(0, 0, tuple<int, int, int>{0,0,0});
		for (int i = 0; i < R; i++) sort(opts[i].begin(), opts[i].end());
	}

	double solve() {
		dest = ((R-1) + (C-1)) / 2;
		recurse_once();
		vector<vector<tuple<int, int, int>>> lopts = std::move(opts);
		reverse(roses.begin(), roses.end());
		reverse(tulips.begin(), tulips.end());
		reverse(costs.begin(), costs.end());
		dest = ((R-1) + (C-1)) - dest;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (i + j == dest) {
					roses[i*C+j] = 0;
					tulips[i*C+j] = 0;
					costs[i*C+j] = 0;
				}
			}
		}
		recurse_once();
		vector<vector<tuple<int, int, int>>> ropts = std::move(opts);
		reverse(ropts.begin(), ropts.end());

		assert(int(lopts.size()) == R);
		assert(int(ropts.size()) == R);

		bool is_possible = false;
		for (int i = 0; i < R; i++) {
			assert(lopts[i].empty() == ropts[i].empty());
			if (lopts[i].empty()) continue;

			if (get<0>(lopts[i][0]) + get<0>(ropts[i][0]) <= K) {
				is_possible = true;
				break;
			}
		}
		if (!is_possible) {
			return -1;
		}
		auto check_vec = [&](double v, const vector<tuple<int, int, int>>& a, const vector<tuple<int, int, int>>& b) -> bool {
			assert(a.empty() == b.empty());
			if (a.empty()) return false;
			const double INF = 1e99;
			double best_b = -INF;
			for (int i = int(a.size())-1, j = 0; i >= 0; i--) {
				while (j < int(b.size()) && get<0>(a[i]) + get<0>(b[j]) <= K) {
					best_b = max(best_b, get<1>(b[j]) - v * get<2>(b[j]));
					j++;
				}
				if (best_b + get<1>(a[i]) - v * get<2>(a[i]) >= 0) return true;
			}
			return false;
		};
		auto is_good = [&](double v) {
			for (int z = 0; z < R; z++) {
				if (check_vec(v, lopts[z], ropts[z])) return true;
			}
			return false;
		};

		double mi = 0.9e-6;
		double ma = 1.1e6;
		for (int z = 0; z < 60; z++) {
			double md = (mi + ma) / 2;
			if (is_good(md)) mi = md;
			else ma = md;
		}

		return mi;
	}

public:

	double bestPath(int R_, int C_, int K_, vector <int> roses_, vector <int> tulips_, vector <int> costs_) {
		R = R_;
		C = C_;
		K = K_;
		roses = std::move(roses_);
		tulips = std::move(tulips_);
		costs = std::move(costs_);
		return solve();
	}
};

void my_tests() {
	// FILL IN TESTS
	auto test_random = [](int R, int C) {
		mt19937 mt(48);
		int K = R * C * int(1e6) / 3;
		vector<int> roses(R*C);
		vector<int> tulips(R*C);
		vector<int> costs(R*C);

		for (int i = 1; i < R*C-1; i++) {
			roses[i] = mt() % int(1e6) + 1;
			tulips[i] = mt() % int(1e6) + 1;
			costs[i] = mt() % int(1e6) + 1;
		}
		cerr << "testing " << R << 'x' << C << '\n';
		double ans = ProposalOptimization().bestPath(R, C, K, roses, tulips, costs);
		cerr << ans << '\n';
	};
	for (int i = 17; i >= 1; i--) {
		test_random(300 / i, i);
		test_random(i, 300 / i);
	}
}

}  // namespace ecnerwala

using ecnerwala::ProposalOptimization;

// vim:ft=cpp
