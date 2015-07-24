#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class MinimumCuts {

public:

	static const int INF = 1e9;
	static const int MAXN = 200;
	int N;
	int P[MAXN];
	int V[MAXN];
	bool leaf[MAXN];
	vector<int> C[MAXN];

	int trp[MAXN];
	int ret[MAXN];

	int costPaid(vector <int> parent, vector <int> cost) {
		N = int(parent.size()) + 1;
		memset(leaf, true, sizeof(leaf));
		P[0] = -1;
		V[0] = INF;
		for(int i = 1; i < N; i++) {
			P[i] = parent[i - 1];
			V[i] = cost[i - 1];
			assert(0 <= P[i] && P[i] < i);
			C[P[i]].push_back(i);
		}
		for(int i = N - 1; i >= 0; i--) {

			if(C[i].empty()) {
				trp[i] = INF;
				ret[i] = V[i];
			} else if(int(C[i].size()) == 1) {
				int n = C[i][0];
				trp[i] = V[i] + min(trp[n], ret[n]);
				ret[i] = min(ret[n], V[i]);
			} else if(int(C[i].size()) == 2) {
				int l = C[i][0];
				int r = C[i][1];
				trp[i] = V[i] + max(min(trp[l], ret[l] + min(trp[r], ret[r])), min(trp[r], ret[r] + min(trp[l], ret[l])));
				ret[i] = min(ret[l] + ret[r], V[i]);
			} else assert(false);
			assert(ret[i] <= trp[i]);
			cerr << i << ' ' << ret[i] << ' ' << trp[i] << '\n';
		}

		return ret[0];
	}
};

// trp[i] is cost to choose edge i, not come out
// trp[i] = V[i] + min(trp[l], max(ret[l] + trp[r], ret[r] + trp[l]), ret[l] + ret[r])
// ret[i] is cost to choose edge i, and get back to P[i]
// ret[leaf] = V[leaf]
// trp[leaf] = INF

// vim:ft=cpp
