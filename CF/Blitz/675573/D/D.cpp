#include <bits/stdc++.h>

int main() {
	int N; std::cin >> N;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; std::cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	std::vector<std::pair<int, int>> colors(N);
	for (int i = 0; i < N; i++) {
		std::cin >> colors[i].first;
		colors[i].second = i;
	}
	std::sort(colors.begin(), colors.end());
	std::vector<std::array<int, 2>> paths; paths.reserve(N);
	auto add_path = [&](int i, int j) -> void {
		// we need to lie on this path
		paths.push_back({i, j});
	};
	for (int i = 0, j = 0; i < N; i = j) {
		while (j < N && colors[j].first == colors[i].first) j++;
		if (j - i >= 3) {
			add_path(colors[i].second, colors[i+1].second);
			add_path(colors[i].second, colors[i+2].second);
			add_path(colors[i+1].second, colors[i+2].second);
		} else if (j - i == 2) {
			add_path(colors[i].second, colors[i+1].second);
		}
	}
	if (paths.empty()) {
		std::cout << "YES" << '\n';
		std::cout << 1 << '\n';
		exit(0);
	}
	int rt = paths[0][0];
	std::vector<int> par(N);
	[&](this auto&& self, int cur, int prv) -> void {
		par[cur] = prv;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
	}(rt, -1);
	std::vector<bool> is_main_path(N, false);
	std::vector<int> main_path; main_path.reserve(N);
	for (int cur = paths[0][1]; cur != -1; cur = par[cur]) {
		main_path.push_back(cur);
		is_main_path[cur] = true;
	}
	for (int st : main_path) {
		[&](this auto&& self, int cur, int prv) -> void {
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (is_main_path[nxt]) continue;
				self(nxt, cur);
			}
		}(st, -1);
	}
}
