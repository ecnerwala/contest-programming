#include<bits/stdc++.h>

std::vector<std::vector<int>> run_ijk(const std::vector<std::vector<int>>& adj) {
	int N = int(adj.size());
	std::vector<std::vector<int>> dist(N, std::vector<int>(N, N));
	for (int i = 0; i < N; i++) {
		for (int j : adj[i]) dist[i][j] = 1;
		dist[i][i] = 0;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < N; k++) {
				dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
	return dist;
}

std::vector<std::vector<int>> run_ikj(const std::vector<std::vector<int>>& adj) {
	int N = int(adj.size());
	std::vector<std::vector<int>> dist(N, std::vector<int>(N, N));
	for (int i = 0; i < N; i++) {
		for (int j : adj[i]) dist[i][j] = 1;
		dist[i][i] = 0;
	}
	for (int i = 0; i < N; i++) {
		for (int k = 0; k < N; k++) {
			for (int j = 0; j < N; j++) {
				dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
	return dist;
}

std::vector<std::vector<int>> run_kij(const std::vector<std::vector<int>>& adj) {
	int N = int(adj.size());
	std::vector<std::vector<int>> dist(N, std::vector<int>(N, N));
	for (int i = 0; i < N; i++) {
		for (int j : adj[i]) dist[i][j] = 1;
		dist[i][i] = 0;
	}
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
	return dist;
}

std::vector<std::vector<int>> real_dist(const std::vector<std::vector<int>>& adj) {
	return run_kij(adj);
}

bool solve_ikj(const std::vector<std::vector<int>>& adj) {
	bool good = true;
	[&](this auto&& self, int cur, int prv) -> void {
		assert(prv < cur);
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			if (nxt > cur) {
				self(nxt, cur);
			} else {
				if (adj[nxt].size() == 1) {
					assert(adj[nxt][0] == cur);
					assert(prv < cur);
					if (prv == -1) {
						assert(cur == 0);
					} else if (adj[prv].size() == 1) {
						assert(prv == 0);
						// still good
					} else {
						good = false;
					}
				} else {
					// node 1 won't see nxt's children, which is bad
					good = false;
				}
			}
		}
	}(0, -1);
	if (int(adj.size()) <= 50) {
		bool slo = run_ikj(adj) == real_dist(adj);
		assert(good == slo);
	}
	return good;
}

bool solve_ijk(const std::vector<std::vector<int>>& adj) {
	bool good = true;
	[&](this auto&& self, int cur, int prv) -> void {
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			if (nxt > cur) {
			} else {
				if (prv == 0) {
				} else {
					good = false;
				}
			}
			self(nxt, cur);
		}
	}(0, -1);
	if (int(adj.size()) <= 50) {
		bool slo = run_ijk(adj) == real_dist(adj);
		assert(good == slo);
	}
	return good;
}

void test() {
	std::mt19937 mt(48);
	for (int z = 0; z < 10000; z++) {
		int N = std::uniform_int_distribution<int>(2, 20)(mt);
		std::cerr << "test " << z << ' ' << N << '\n';
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		std::vector<std::vector<int>> adj(N);
		for (int ccs = N; ccs > 1; ) {
			int u = std::uniform_int_distribution(0, N-1)(mt);
			int v = std::uniform_int_distribution(0, N-1)(mt);
			if (merge(u, v)) {
				adj[u].push_back(v);
				adj[v].push_back(u);
				ccs--;
			}
		}
		{
			bool ans = solve_ikj(adj);
			bool slo = run_ikj(adj) == real_dist(adj);
			assert(ans == slo);
		}
		{
			bool ans = solve_ijk(adj);
			bool slo = run_ijk(adj) == real_dist(adj);
			assert(ans == slo);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	//test();

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		std::string s; cin >> s;
		int N; cin >> N;
		cerr << "Case #" << case_num << " " << "N = " << N << ' ' << s << '\n';
		assert(N >= 2);
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		bool good;
		if (s[0] == 'k') {
			good = true;
		} else if (s[1] == 'k') {
			good = solve_ikj(adj);
		} else if (s[2] == 'k') {
			good = solve_ijk(adj);
		} else assert(false);

		cout << "Case #" << case_num << ": " << (good ? "Lucky" : "Wrong") << '\n';
	}

	return 0;
}
