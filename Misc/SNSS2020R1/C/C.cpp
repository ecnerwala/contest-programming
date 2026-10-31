#include<bits/stdc++.h>

namespace kactl {
// https://github.com/kth-competitive-programming/kactl/blob/master/content/graph/hopcroftKarp.h

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
	if (A[a] != L) return 0;
	A[a] = -1;
	for (int b : g[a]) if (B[b] == L + 1) {
		B[b] = 0;
		if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B))
			return btoa[b] = a, 1;
	}
	return 0;
}

int hopcroftKarp(vector<vi>& g, vi& btoa) {
	int res = 0;
	vi A(g.size()), B(btoa.size()), cur, next;
	for (;;) {
		fill(all(A), 0);
		fill(all(B), 0);
		/// Find the starting nodes for BFS (i.e. layer 0).
		cur.clear();
		for (int a : btoa) if(a != -1) A[a] = -1;
		rep(a,0,sz(g)) if(A[a] == 0) cur.push_back(a);
		/// Find all layers using bfs.
		for (int lay = 1;; lay++) {
			bool islast = 0;
			next.clear();
			for (int a : cur) for (int b : g[a]) {
				if (btoa[b] == -1) {
					B[b] = lay;
					islast = 1;
				}
				else if (btoa[b] != a && !B[b]) {
					B[b] = lay;
					next.push_back(btoa[b]);
				}
			}
			if (islast) break;
			if (next.empty()) return res;
			for (int a : next) A[a] = lay;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		rep(a,0,sz(g))
			res += dfs(a, 0, g, btoa, A, B);
	}
}

#undef rep
#undef all
#undef sz
} // namespace kactl

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	map<string, int> str_id;
	auto get_id = [&](string s) -> int {
		auto it = str_id.find(s);
		if (it != str_id.end()) return it->second;
		int res = int(str_id.size());
		str_id.emplace(s, res);
		return res;
	};

	vector<vector<int>> g(N);
	for (int i = 0; i < N; i++) {
		string L, R; cin >> L >> R;
		for (int a = 1; a <= int(L.size()); a++) {
			for (int b = 1; b <= int(R.size()); b++) {
				g[i].push_back(get_id(L.substr(0, a) + R.substr(0, b)));
			}
		}
	}
	vector<int> btoa(str_id.size(), -1);
	int num_matched = kactl::hopcroftKarp(g, btoa);
	if (num_matched < N) {
		cout << "No solution" << '\n';
		exit(0);
	}

	vector<string> ans(N);
	for (const auto& it : str_id) {
		if (btoa[it.second] != -1) {
			ans[btoa[it.second]] = it.first;
		}
	}
	for (int i = 0; i < N; i++) {
		assert(!ans[i].empty());
		cout << ans[i] << '\n';
	}

	return 0;
}
