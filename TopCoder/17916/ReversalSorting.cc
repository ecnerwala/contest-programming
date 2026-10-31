#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ReversalSorting {

public:

	vector <int> solve(vector <int> P) {
		int N = int(P.size());
		for (auto& v : P) v++;
		P.insert(P.begin(), 0);
		P.push_back(N+1);
		std::vector<int> ops;
		auto do_op = [&](int l, int r) {
			assert(1 <= l && l <= r && r <= N+1);
			if (r - l >= 2) {
				ops.push_back(l-1);
				ops.push_back(r-1);
				std::reverse(P.begin() + l, P.begin() + r);
			}
		};
		while (!std::is_sorted(P.begin(), P.end())) {
			for (int i = 0; i <= N; i++) {
				int p1 = int(std::find(P.begin(), P.end(), i) - P.begin());
				int p2 = int(std::find(P.begin(), P.end(), i+1) - P.begin());
				if (abs(p1 - p2) == 1) continue;
				bool p1_incr = (p1 == 0 || P[p1-1] == i-1);
				bool p2_incr = (p2 == N+1 || P[p2+1] == i+2);
				if (p1_incr && p2_incr) {
					// both increasing
					continue;
				}
				bool p1_decr = (P[p1+1] == i-1);
				bool p2_decr = (P[p2-1] == i+2);
				if (p1_decr && p2_decr) {
					continue;
				}
				if (p1_incr || p2_decr) {
					// right sides should match
					do_op(min(p1, p2) + 1, max(p1, p2) + 1);
				} else {
					// left sides shoudl match
					do_op(min(p1, p2), max(p1, p2));
				}
				goto found;
			}
			// if we failed, then everything must be in increasing pairs
			for (int i = 0; i <= N; i++) {
				int p1 = int(std::find(P.begin(), P.end(), i) - P.begin());
				int p2 = int(std::find(P.begin(), P.end(), i+1) - P.begin());
				if (abs(p1 - p2) == 1) continue;
				bool p1_incr = (p1 == 0 || P[p1-1] == i-1);
				bool p2_incr = (p2 == N+1 || P[p2+1] == i+2);
				assert(p1_incr && p2_incr);
				int p3 = p2+1;
				while (P[p3] == P[p3-1] + 1) p3++;
				do_op(p2, p3);
				do_op(p1+1, p3);
				goto found;
			}
found:
			;
			//for (auto v : P) { cerr << v << ' '; } cerr << '\n';
		}
		for (int i = 0; i <= N+1; i++) {
			assert(P[i] == i);
		}
		return ops;
	}
};

void opt(int N) {
	std::map<std::vector<int>, int> dist;
	std::vector<std::vector<int>> q;
	q.emplace_back(N);
	std::iota(q[0].begin(), q[0].end(), 0);
	dist[q[0]] = 0;

	for (int z = 0; z < int(q.size()); z++) {
		auto cur = q[z];
		int d = dist[cur];

		{
			auto res = ReversalSorting().solve(cur);
			int num_ops = int(res.size()) / 2;
			if (num_ops > 2 * d || num_ops > N-1) {
				cerr << "Bad " << d << ' ' << num_ops << '\n';
				for (auto v : cur) { cerr << v << ','; } cerr << '\n';
				for (auto v : res) { cerr << v << ' '; } cerr << '\n';
			}
		}
		for (int x = 0; x < N; x++) {
			for (int y = x+2; y <= N; y++) {
				auto nxt = cur;
				std::reverse(nxt.begin()+x, nxt.begin()+y);
				if (!dist.count(nxt)) {
					dist[nxt] = d+1;
					q.push_back(nxt);
				}
			}
		}
	}
}


void my_tests() {
	// FILL IN TESTS
	for (int N = 1; N <= 10; N++) {
		cerr << "Testing " << N << '\n';
		opt(N);
		cerr << "Tested " << N << '\n';
	}
}

}  // namespace ecnerwala

using ecnerwala::ReversalSorting;

// vim:ft=cpp
