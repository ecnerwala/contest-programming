#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

void dfs(int cur, vector<int>& nxt, vector<int>& stk) {
	while (nxt[cur] != cur) {
		int v = nxt[cur];
		nxt[cur] ++, nxt[cur] %= int(nxt.size());
		dfs(v, nxt, stk);
		stk.push_back(cur*int(nxt.size()) + v);
	}
}

class TransposeColors {

public:

	vector <int> move(int N) {
		if (N == 1) return {};

		vector<int> adj(N);
		for (int i = 0; i < N; i++) {
			adj[i] = (i+1)%N;
		}
		vector<int> stk; stk.reserve(N*(N-1)+1);
		stk.push_back(N*N);
		dfs(0, adj, stk);
		reverse(stk.begin(), stk.end());
		return stk;
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int N = 1; N <= 40; N++) {
		vector<int> A(N*N+1);
		vector<int> B(N*N+1);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i*N+j] = i;
				B[i*N+j] = j;
			}
		}
		A[N*N] = B[N*N] = -1;
		int cur_empty = N*N;
		auto res = TransposeColors().move(N);
		assert(int(res.size()) == N * (N-1) + (N * (N-1) > 1));
		for (int v : res) {
			assert(v != cur_empty);
			std::swap(A[cur_empty], A[v]);
			cur_empty = v;
		}
		assert(A == B);
		cerr << "N = " << N << " passed" << '\n';
	}
}

}  // namespace ecnerwala

using ecnerwala::TransposeColors;

// vim:ft=cpp
