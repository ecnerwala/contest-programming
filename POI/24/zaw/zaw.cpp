#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	vector<array<int, 2>> ranks(N);
	vector<vector<int>> adj(N);
	int num_loop = 0;
	for (int i = 0; i < N; i++) {
		char c; cin >> c;
		if (c == 'N') {
			int a, b; cin >> a >> b; a--, b--;
			ranks[i] = {a, b};
		} else if (c == 'T') {
			int a; cin >> a; a--;
			ranks[i] = {a, a};
			num_loop++;
		} else assert(false);

		for (int z = 0; z < 2; z++) {
			adj[ranks[i][z]].push_back(i);
		}
	}

	vector<int> deg(N);
	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		deg[i] = int(adj[i].size());
		if (deg[i] == 1) q.push_back(i);
	}

	vector<int> ans(N, -1);
	vector<bool> alive(N, true);

	for (int z = 0; z < int(q.size()); z++) {
		int v = q[z];
		if (!deg[v]) {
			cout << "NIE" << '\n';
			cout << 0 << '\n';
			exit(0);
		}
		int cur = -1;
		for (auto i : adj[v]) if (alive[i]) cur = i;
		assert(cur != -1);

		alive[cur] = false;
		ans[cur] = v;

		--deg[v];

		int o = ranks[cur][0] ^ ranks[cur][1] ^ v;
		--deg[o];
		if (deg[o] == 1) q.push_back(o);
	}

	// Check for impossible
	for (int i = 0; i < N; i++) {
		if (deg[i] != 0 && deg[i] != 2) {
			cout << "NIE" << '\n';
			cout << 0 << '\n';
			exit(0);
		}
	}

	// Count number of cycles using union-find
	vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	int loops = 0;
	for (int i = 0; i < N; i++) {
		if (!alive[i]) continue;
		if (ranks[i][0] == ranks[i][1]) {
			ans[i] = ranks[i][0];
			continue;
		}
		loops += !merge(ranks[i][0], ranks[i][1]);
	}

	if (loops == 0) {
		cout << "TAK" << '\n';
		for (int i = 0; i < N; i++) cout << ans[i]+1 << '\n';
	} else {
		cout << "NIE" << '\n';
		int ways = 1;
		const int MOD = int(1e9)+7;
		while (loops--) { ways <<= 1; ways = (ways >= MOD ? ways - MOD : ways); }
		cout << ways << '\n';
	}

	return 0;
}
