#include "train.h"
#include <bits/stdc++.h>

namespace ecnerwala {

using namespace std;

typedef pair<int, int> pii;

struct train {
	static const int MAXN = 6000;
	int N, M;
	bool A[MAXN];
	bool G[MAXN];
	int D[MAXN]; // outdegree
	vector<int> adj[MAXN]; // previous edges

	bool res[MAXN];
	int thresh[MAXN];
	bool seen[MAXN];

	vector<int> go() {
		// initialize optimistically
		for (int i = 0; i < N; i++) res[i] = true;
		for (int t = 0; t < N; t++) {
			queue<int> q;
			for (int i = 0; i < N; i++) {
				if (res[i] && G[i]) {
					q.push(i);
				}
				thresh[i] = A[i] ? 1 : D[i];
				assert(thresh[i]);
			}
			for (int i = 0; i < N; i++) {
				res[i] = false;
				seen[i] = false;
			}
			while (!q.empty()) {
				int cur = q.front(); q.pop();
				if (seen[cur]) continue;
				seen[cur] = true;
				for (int prv : adj[cur]) {
					thresh[prv] --;
					if (thresh[prv] == 0) {
						assert(res[prv] == false);
						res[prv] = true;
						q.push(prv);
					}
				}
			}
		}
		vector<int> r(N);
		for (int i = 0; i < N; i++) {
			r[i] = res[i] ? 1 : 0;
		}
		return r;
	}

	train(vector<int> a, vector<int> r, vector<int> u, vector<int> v) {
		N = int(a.size());
		assert(int(r.size()) == N);
		assert(u.size() == v.size());
		M = int(u.size());
		for (int i = 0; i < N; i++) A[i] = a[i];
		for (int i = 0; i < N; i++) G[i] = r[i];
		for (int i = 0; i < N; i++) {
			D[i] = 0;
			adj[i].clear();
		}
		for (int i = 0; i < M; i++) {
			int x = u[i], y = v[i];
			D[x] ++;
			adj[y].push_back(x);
		}
		for (int i = 0; i < N; i++) {
			assert(D[i] > 0);
		}
	}
};


}

std::vector<int> who_wins(std::vector<int> a, std::vector<int> r, std::vector<int> u, std::vector<int> v) {
	ecnerwala::train t(a,r,u,v);
	return t.go();
}
