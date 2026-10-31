#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

int solve(int N, int K, std::vector<int> P) {
	// I think we just greedily place from the outside?
	std::vector<std::vector<int>> adj(N);
	for (int i = 1; i < N; i++) {
		adj[P[i]].push_back(i);
		adj[i].push_back(P[i]);
	}
	std::vector<int> deg(N);
	for (int i = 0; i < N; i++) {
		deg[i] = int(adj[i].size());
	}
	std::vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (deg[i] <= 1) q.push_back(i);
	}
	int st = 0;
	for (int z = 0; z < K/2; z++) {
		int en = int(q.size());
		for (int i = st; i < en; i++) {
			int cur = q[i];
			for (int nxt : adj[cur]) {
				deg[nxt]--;
				if (deg[nxt] == 1) {
					q.push_back(nxt);
				}
			}
		}
		st = en;
	}
	int ans = st;
	if (K & 1) {
		if (ans < N) {
			ans ++;
		}
	}
	return ans;
}

class TokenTree {

public:

	int maxTokens(int N, int K, vector <int> Pprefix, int D, int seed) {
		int64_t state = seed;
		int L = int(Pprefix.size());
		std::vector<int> P; P.reserve(N);
		P.push_back(-1);
		for (int i = 0; i < L; i++) P.push_back(Pprefix[i]);
		for (int i = L; i <= N-2; i++) {
			int lo = max(0, i-D+1);
			state = (state * 1103515245 + 12345) & ((1ll << 31) - 1);
			P.push_back(int(lo + (state % (i-lo+1))));
		}
		assert(int(P.size()) == N);
		return solve(N, K, std::move(P));
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::TokenTree;

// vim:ft=cpp
