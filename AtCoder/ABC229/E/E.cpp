#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
	}

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

	std::vector<int> ans(N);
	int cur_cc = 0;
	for (int i = N-1; i >= 0; i--) {
		ans[i] = cur_cc;
		cur_cc++;
		for (int j : adj[i]) cur_cc -= merge(i, j);
	}
	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
