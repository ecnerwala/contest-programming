#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int E; cin >> E;

	const int V = 1e6;
	vector<int> par(V, -1);
	auto get_par = [&](int a) {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	vector<int> num_e(V);

	auto merge = [&](int a, int b) {
		a = get_par(a);
		b = get_par(b);
		if (a == b) {
			num_e[a]++;
			return false;
		}

		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		num_e[a] += num_e[b];
		num_e[a]++;

		par[b] = a;
		return true;
	};

	while (E--) {
		int u, v; cin >> u >> v;
		merge(u, v);
	}
	int ans = 0;
	for (int i = 0; i < V; i++) {
		if (par[i] >= 0) continue;
		int val = min(-par[i], num_e[i]);
		ans += val;
	}
	cout << ans << '\n';

	return 0;
}
