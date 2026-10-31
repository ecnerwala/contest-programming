#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	N++;
	struct edge_t {
		int st, en;
		int w;
	};
	std::vector<edge_t> edges(M);
	for (auto& e : edges) {
		int l, r; char o0, o1; int s; cin >> l >> r >> o0 >> o1 >> s;
		l--;
		assert(o1 == '=');
		if (o0 == '<') {
			// v[r] - v[l] <= s
			// v[r] <= v[l] + s
			e = {l, r, s};
		} else if (o0 == '>') {
			// v[r] - v[l] >= s
			// v[l] <= v[r] - s
			e = {r, l, -s};
		} else assert(false);
	}
	// now we look for a negative cycle
	std::vector<int64_t> dist(N, 0);
	for (int i = 0; i < N; i++) {
		bool did_change = false;
		for (auto [st, en, w] : edges) {
			if (dist[st] + w < dist[en]) {
				dist[en] = dist[st] + w;
				did_change = true;
			}
		}
		if (!did_change) {
			cout << "YES" << '\n';
			exit(0);
		}
	}
	cout << "NO" << '\n';
	exit(0);

	return 0;
}
