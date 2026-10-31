#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ThreeWaySplit {

public:

	string split(vector <int> loot) {
		struct cnd_t {
			int64_t net_val;
			int64_t alice_val;
			int64_t msk;
		};
		auto do_add = [&](std::vector<cnd_t> &vals, int64_t v) {
			const int64_t INF = 1e18;
			std::vector<cnd_t> nvals; nvals.reserve(vals.size() * 3);
			for (int a = 0, b = 0, c = 0; a < int(vals.size()) || b < int(vals.size()) || c < int(vals.size()); ) {
				int64_t va = (a == int(vals.size()) ? INF : vals[a].net_val - v);
				int64_t vb = (b == int(vals.size()) ? INF : vals[b].net_val);
				int64_t vc = (c == int(vals.size()) ? INF : vals[c].net_val + v);
				int64_t best = std::min({va, vb, vc});
				cnd_t nxt;
				if (va == best) {
					assert(a < int(vals.size()));
					nxt = vals[a++];
					nxt.net_val -= v;
					nxt.alice_val += v;
					nxt.msk = nxt.msk * 3 + 0;
				} else if (vb == best) {
					assert(b < int(vals.size()));
					nxt = vals[b++];
					nxt.msk = nxt.msk * 3 + 2;
				} else if (vc == best) {
					assert(c < int(vals.size()));
					nxt = vals[c++];
					nxt.net_val += v;
					nxt.msk = nxt.msk * 3 + 1;
				} else assert(false);

				if (!nvals.empty() && nvals.back().net_val == nxt.net_val) {
					if (nvals.back().alice_val >= nxt.alice_val) {
						// do nothing
					} else {
						nvals.back() = nxt;
					}
				} else {
					assert(nvals.empty() || nxt.net_val > nvals.back().net_val);
					nvals.push_back(nxt);
				}
			}
			vals = std::move(nvals);
		};

		int N = int(loot.size());
		int L = N / 2;
		std::vector<cnd_t> lvals;
		lvals.push_back({0, 0, 0});
		for (int i = L-1; i >= 0; i--) {
			do_add(lvals, loot[i]);
		}
		std::vector<cnd_t> rvals;
		rvals.push_back({0, 0, 0});
		for (int i = N-1; i >= L; i--) {
			do_add(rvals, loot[i]);
		}
		{
			int64_t rval_mult = 1;
			for (int i = 0; i < L; i++) rval_mult *= 3;
			for (auto& v : rvals) {
				v.msk *= rval_mult;
			}
		}

		/*
		cerr << "lvals" << '\n';
		for (auto it : lvals) {
			cerr << it.net_val << " " << it.alice_val << " " << it.msk << endl;
		}
		cerr << '\n';
		cerr << "rvals" << '\n';
		for (auto it : rvals) {
			cerr << it.net_val << " " << it.alice_val << " " << it.msk << endl;
		}
		*/

		std::pair<int64_t, int64_t> best{-1, -1};
		for (int i = 0, j = int(rvals.size())-1; i < int(lvals.size()) && j >= 0; ) {
			int64_t net_val = lvals[i].net_val + rvals[j].net_val;
			if (net_val == 0) {
				std::pair<int64_t, int64_t> cnd(lvals[i].alice_val + rvals[j].alice_val, lvals[i].msk + rvals[j].msk);
				best = std::max(best, cnd);
				i++, j--;
			} else if (net_val > 0) {
				j--;
			} else {
				i++;
			}
		}
		std::string ans(N, '\0');
		int64_t v = best.second;
		for (int i = 0; i < N; i++) {
			ans[i] = (v % 3) + 'A';
			v /= 3;
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ThreeWaySplit;

// vim:ft=cpp
