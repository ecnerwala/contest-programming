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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<int, 3> vert;
		for (auto& v : vert) { cin >> v; v--; }
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--; v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::array<int, 3> dist{-1, 0, 0};
		std::y_combinator([&](auto self, int cur, int prv) -> int {
			int r = 0;
			if (cur == vert[1]) r |= 1;
			if (cur == vert[2]) r |= 2;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				r |= self(nxt, cur);
			}
			if (r == 1) dist[1]++;
			if (r == 2) dist[2]++;
			if (r == 3) dist[0]++;
			return r;
		})(vert[0], -1);
		//cerr << dist[0] << ' ' << dist[1] << ' ' << dist[2] << '\n';
		std::array<char, 3> labels{'A', 'B', 'C'};
		if (dist[0] == 0) {
			cout << labels[0] << '\n';
		} else if (dist[1] == 0) {
			// we can't capture our target, so we lose
			cout << labels[1] << '\n';
		} else if (dist[2] == 0) {
			if (dist[0] == 1) {
				cout << labels[0] << '\n';
			} else {
				cout << labels[2] << '\n';
			}
		} else {
			while (!(dist[0] <= dist[1] && dist[0] <= dist[2])) {
				assert(dist[0] >= 2);
				dist[0]--;
				std::rotate(labels.begin(), labels.begin() + 1, labels.end());
				std::rotate(dist.begin(), dist.begin() + 1, dist.end());
			}
			assert(dist[0] <= dist[1] && dist[0] <= dist[2]);
			assert(dist[0] >= 1);
			int l = dist[0] - 1;
			dist[0] -= l;
			dist[1] -= l;
			dist[2] -= l;
			assert(dist[0] == 1);
			if (dist[1] >= 2) {
				cout << labels[0] << '\n';
			} else {
				assert(dist[1] == 1);
				if (dist[2] >= 2) {
					cout << labels[1] << '\n';
				} else {
					cout << "DRAW" << '\n';
				}
			}
		}
	}

	return 0;
}
