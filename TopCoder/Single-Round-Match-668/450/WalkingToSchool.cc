#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class WalkingToSchool {

	int N, M;
	pii E[4000];

	// 0 is forward, 1 is backward
	vector<int> adj[2][4000];

	bool vis[2][2][4000];
	bool is_good[2][4000];

	void dfs(int s, int d, int a) {
		if(vis[s][d][a]) return;
		vis[s][d][a] = true;
		for(int b : adj[d][a]) {
			dfs(s, d, b);
		}
	}

	int dist[4000];

	bool is_consistent(int c, int d, int m, int t) {
		if(!is_good[t][c]) return true;
		d %= m;
		if(dist[c] != -1) {
			if(dist[c] != d) return false;
			return true;
		}
		dist[c] = d;
		for(int b : adj[0][c]) {
			if(!is_consistent(b, d + 1, m, t)) return false;
		}
		return true;
	}

	// true if all k-routes exist
	bool go() {
		memset(vis, 0, sizeof(vis));
		dfs(0, 0, 0);
		dfs(1, 1, 1);
		dfs(1, 0, 1);
		dfs(0, 1, 0);
		for(int i = 0; i < N; i++) {
			is_good[0][i] = vis[0][0][i] && vis[1][1][i];
			is_good[1][i] = vis[0][1][i] && vis[1][0][i];
		}
		if(!(is_good[0][0] && is_good[1][1])) return false;

		for(int m = 2; m <= N; m++) {
			bool bad = true;
			for(int t = 0; t < 2; t++) {
				memset(dist, -1, sizeof(dist));
				if(!is_consistent(t, 0, m, t)) bad = false;
			}
			if(bad) {
				cerr << m << '\n';
				return false;
			}
		}

		return true;
	}

public:

	string canWalkExactly(int n, vector <int> from, vector <int> to) {
		N = n;
		M = int(from.size());
		for(int i = 0; i < M; i++) {
			E[i] = pii(from[i], to[i]);
			adj[0][from[i]].push_back(to[i]);
			adj[1][to[i]].push_back(from[i]);
		}
		return go() ? "Freedom" : "Chores";
	}
};

// vim:ft=cpp
