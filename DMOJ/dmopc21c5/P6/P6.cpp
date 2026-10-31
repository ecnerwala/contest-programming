#include <bits/stdc++.h>

namespace kactl {
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
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(2e6);
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}
	auto is_prime = [&](int a) -> bool {
		assert(a > 0);
		return pfac[a] == a;
	};
	cerr << "precomp done" << '\n';

	auto solve = [&](int N) {
		assert(N >= 1);
		std::vector<int> A(N+1);
		std::vector<bool> used(N+1);
		if (N & 1) {
			A[1] = 1;
			used[1] = true;
		} else {
			A[1] = 2;
			A[2] = 1;
			used[2] = used[1] = true;
		}
		for (int i = (N & 1) ? 2 : 3; i <= N; i++) {
			int j = i;
			while (j <= N && !is_prime(i+j)) j++;
			if (j == N+1) {
				// fixup
				std::vector<int> rvals; rvals.reserve(N);
				std::vector<int> btoa; btoa.reserve(N);
				for (int v = N; v >= i; v--) {
					rvals.push_back(v);
					btoa.push_back(-1);
				}
				std::vector<std::vector<int>> g; g.reserve(N);
				for (int v = N; v >= i; v--) {
					g.emplace_back();
					for (int u = 0; u < int(rvals.size()); u++) {
						if (is_prime(v+rvals[u])) g.back().push_back(u);
					}
				}
				while (true) {
					i--;
					if (i == 0) {
						cout << -1 << '\n';
						exit(0);
					}
					// i..N
					{
						g.emplace_back();
						for (int u = 0; u < int(rvals.size()); u++) {
							if (is_prime(i+rvals[u])) g.back().push_back(u);
						}
					}
					rvals.push_back(A[i]);
					btoa.push_back(-1);
					for (int v = N; v >= i; v--) {
						if (is_prime(v + rvals[N-i])) g[N-v].push_back(N-i);
					}
					std::fill(btoa.begin(), btoa.end(), -1);

					assert(int(g.size()) == N-i+1);
					assert(int(btoa.size()) == N-i+1);

					int f = kactl::hopcroftKarp(g, btoa);
					if(f == N-i+1) {
						break;
					}
				}
				//cerr << "redoing " << i << ".." << N << '\n';
				for (int k = i; k <= N; k++) {
					std::sort(g[N-k].begin(), g[N-k].end(), [&](int a, int b) { return rvals[a] < rvals[b]; });
					while (true) {
						int cnd = g[N-k].back();
						g[N-k].pop_back();
						std::fill(btoa.begin(), btoa.end(), -1);
						if (kactl::hopcroftKarp(g, btoa) == int(btoa.size())) {
							continue;
						}
						A[k] = rvals[cnd];
						g[N-k].push_back(cnd);
						break;
					}
				}

				break;
			}
			int p = i+j;
			for (int k = i; k <= j; k++) {
				used[p-k] = true;
				A[k] = p-k;
			}
			//cerr << i << ' ' << j << '\n';
			i = j;
		}
		for (int i = 1; i <= N; i++) {
			cout << A[i] << " \n"[i==N];
		}
	};

	int N; cin >> N;
	solve(N);

	return 0;
}
