#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int64_t> C(N);
	for (auto& c : C) cin >> c;
	std::vector<std::pair<int64_t, std::array<int, 2>>> edges(M);
	for (auto& e : edges) {
		int a, b; cin >> a >> b; a--, b--;
		e.second = {a, b};
		cin >> e.first;
	}

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	std::sort(edges.begin(), edges.end(), std::greater<>());
	const int64_t INF = 1e18;
	std::vector<int64_t> ans(N, INF); // max width to get there
	for (auto [w, verts] : edges) {
		int a = get_par(verts[0]);
		int b = get_par(verts[1]);
		if (a == b) continue;
		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		ans[a] = std::max(min(ans[a], w) - C[b], min(ans[b], w) - C[a]);
		C[a] += C[b];
	}

	int64_t res = ans[get_par(0)];
	if (res <= 0) {
		res = -1;
	}
	cout << res << '\n';

	return 0;
}
