#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	const int INF = int(1e9);
	struct node_t {
		std::map<std::pair<int, int>, int> ch;
		int dist = INF;
	};
	auto build_num = [&](int i, int a) -> std::pair<int, int> {
		if (a == 0) return {0, 0};
		while (i > 0 && int64_t(i) * int64_t(abs(a)) <= INF) {
			a *= i;
			i--;
		}
		return {i, a};
	};

	std::vector<node_t> nodes; nodes.reserve(int(2.01e5));
	nodes.emplace_back();
	std::vector<int> queries(M);
	for (int z = 0; z < N+M; z++) {
		int K; cin >> K; K++;
		std::vector<std::pair<int, int>> a(K);
		for (int i = 0; i < K; i++) {
			int v; cin >> v;
			a[i] = build_num(i, v);
		}
		std::reverse(a.begin(), a.end());
		int cur = 0;
		for (auto v : a) {
			int& x = nodes[cur].ch[v];
			if (!x) {
				x = int(nodes.size());
				cur = x;
				nodes.emplace_back();
			} else {
				cur = x;
			}
		}
		if (z < N) {
			nodes[cur].dist = 0;
		} else {
			queries[z-N] = cur;
		}
	}

	int V = int(nodes.size());
	for (int v = V-1; v >= 0; v--) {
		for (auto [_, c] : nodes[v].ch) {
			nodes[v].dist = min(nodes[v].dist, nodes[c].dist + 1);
		}
	}
	for (int v = 0; v < V; v++) {
		for (auto [_, c] : nodes[v].ch) {
			nodes[c].dist = min(nodes[c].dist, nodes[v].dist + 1);
		}
	}

	for (int q = 0; q < M; q++) {
		cout << nodes[queries[q]].dist << '\n';
	}

	return 0;
}
