#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, E, G; cin >> N >> E >> G;
	vector<pair<int, array<int, 2>>> edges(E);
	for (auto& e : edges) {
		int a, b, c; cin >> a >> b >> c; a--, b--;
		e = {c, {a, b}};
	}

	vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	vector<vector<int>> adj(G);
	for (auto& r : adj) {
		int k; cin >> k;
		r.resize(k);
		for (auto& v : r) { cin >> v; v--; }
	}

	vector<int> btoa(N, -1);

	vector<bool> vis(G);
	auto augment = std::y_combinator([&](auto self, int g) -> bool {
		if (g == -2) return false;
		if (vis[g]) return false;
		vis[g] = true;
		for (int v : adj[g]) {
			v = get_par(v);
			if (btoa[v] == -1) {
				btoa[v] = g;
				std::fill(vis.begin(), vis.end(), false);
				return true;
			}
		}
		for (int v : adj[g]) {
			v = get_par(v);
			assert(btoa[v] != -1);
			int g2 = btoa[v];
			btoa[v] = g;
			if (self(g2)) return true;
			btoa[v] = g2;
		}
		return false;
	});

	int cnt = 0;
	for (int g = 0; g < G; g++) {
		if (!augment(g)) {
			cout << -1 << '\n';
			exit(0);
		} else {
			cnt++;
		}
	}

	std::sort(edges.begin(), edges.end());
	int ans = 0;
	for (auto [w, v] : edges) {
		int a = v[0], b = v[1];
		a = get_par(a), b = get_par(b);
		if (a == b) continue;
		if (par[a] > par[b]) swap(a, b);
		if ([&]() -> bool {
			if (btoa[a] == -1 || btoa[b] == -1) return true;
			for (int x : {a, b}) {
				int g = std::exchange(btoa[x], -2);
				if (augment(g)) {
					btoa[x] = -1;
					return true;
				}
				btoa[x] = g;
			}
			return false;
		}()) {
			if (btoa[b] == -1) {
			} else if (btoa[a] == -1) {
				btoa[a] = btoa[b];
			} else assert(false);
			par[a] += par[b];
			par[b] = a;
			ans += w;
			cnt++;
		}
	}
	if (cnt == N) {
		cout << ans << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
