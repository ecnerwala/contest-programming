#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

bool dfs(int a, int layer, const vector<vi>& g, vi& btoa,
			vi& A, vi& B) {
	if (A[a] != layer) return 0;
	A[a] = -1;
	trav(b, g[a]) if (B[b] == layer + 1) {
		B[b] = -1;
		if (btoa[b] == -1 || dfs(btoa[b], layer+2, g, btoa, A, B))
			return btoa[b] = a, 1;
	}
	return 0;
}

int hopcroftKarp(const vector<vi>& g, vi& btoa) {
	int res = 0;
	vi A(g.size()), B(btoa.size()), cur, next;
	for (;;) {
		fill(all(A), 0);
		fill(all(B), -1);
		/// Find the starting nodes for BFS (i.e. layer 0).
		cur.clear();
		trav(a, btoa) if(a != -1) A[a] = -1;
		rep(a,0,sz(g)) if(A[a] == 0) cur.push_back(a);
		/// Find all layers using bfs.
		for (int lay = 1;; lay += 2) {
			bool islast = 0;
			next.clear();
			trav(a, cur) trav(b, g[a]) {
				if (btoa[b] == -1) {
					B[b] = lay;
					islast = 1;
				}
				else if (btoa[b] != a && B[b] == -1) {
					B[b] = lay;
					next.push_back(btoa[b]);
				}
			}
			if (islast) break;
			if (next.empty()) return res;
			trav(a, next) A[a] = lay+1;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		rep(a,0,sz(g)) {
			if(dfs(a, 0, g, btoa, A, B))
				++res;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<string> L, R;
	for (int z = 0; z < N; z++) {
		string s; cin >> s;
		int side = 0;
		for (int i = 0; i < int(s.size()); i++) {
			for (int j = i+1; j < int(s.size()); j++) {
				side ^= (s[j] > s[i]);
			}
		}
		if (side) R.push_back(s);
		else L.push_back(s);
	}

	vi btoa(R.size(), -1);
	vector<vi> g(L.size());
	for (int i = 0; i < int(L.size()); i++) {
		for (int j = 0; j < int(R.size()); j++) {
			int numBad = 0;
			for (int k = 0; k < int(L[i].size()); k++) {
				numBad += L[i][k] != R[j][k];
			}
			if (numBad <= 2) {
				g[i].push_back(j);
			}
		}
	}

	cout << int(L.size()) + int(R.size()) - hopcroftKarp(g, btoa) << '\n';

	return 0;
}
