#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
	if (A[a] != L) return 0;
	A[a] = -1;
	trav(b, g[a]) if (B[b] == L + 1) {
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
		trav(a, btoa) if(a != -1) A[a] = -1;
		rep(a,0,sz(g)) if(A[a] == 0) cur.push_back(a);
		/// Find all layers using bfs.
		for (int lay = 1;; lay++) {
			bool islast = 0;
			next.clear();
			trav(a, cur) trav(b, g[a]) {
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
			trav(a, next) A[a] = lay;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		rep(a,0,sz(g))
			res += dfs(a, 0, g, btoa, A, B);
	}
}

const int V = 1.1e7;
bool isPrime[V + 1];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	for (int i = 2; i <= V; i++) isPrime[i] = true;
	for (int p = 2; p <= V; p++) {
		if (!isPrime[p]) continue;
		for (int j = p+p; j <= V; j += p) isPrime[j] = false;
	}

	int N; cin >> N;
	vector<int> X(N);
	map<int, int> evtCnt;
	for (int i = 0; i < N; i++) {
		cin >> X[i];
		evtCnt[X[i]] ^= 1;
		evtCnt[X[i]+1] ^= 1;
	}
	vector<int> left;
	vector<int> right;
	for (auto it : evtCnt) {
		if (it.second == 0) continue;
		if (it.first & 1) right.push_back(it.first);
		else left.push_back(it.first);
	}

	int A = int(left.size());
	int B = int(right.size());
	vector<vector<int>> g(A);
	vector<int> btoa(B, -1);
	isPrime[2] = false;
	for (int i = 0; i < A; i++) {
		for (int j = 0; j < B; j++) {
			if (isPrime[abs(left[i] - right[j])]) {
				g[i].push_back(j);
			}
		}
	}

	int ans = hopcroftKarp(g, btoa); // number that can be paired
	cerr << ans << ' ' << A << ' ' << B << '\n';
	A -= ans, B -= ans;
	cout << ans + (A/2) * 2 + (B/2) * 2 + (A % 2 ? 3 : 0) << '\n';

	return 0;
}
