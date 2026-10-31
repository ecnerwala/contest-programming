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
		std::vector<int> F(N);
		for (auto& f : F) { cin >> f; f--; }
		std::vector<int> G(N);
		for (auto& g : G) { cin >> g; g--; }

		std::vector<std::array<int, 2>> ops; ops.reserve(2*N*N);
		auto solve = [&]() -> bool {
			auto path_compress = [&](int a) -> void {
				int r = a;
				while (F[r] != r) r = F[r];
				if (r == F[a]) return;
				ops.push_back({a, -1});
				while (a != r) {
					a = std::exchange(F[a], r);
				}
			};

			std::vector<int> par(N, -1);
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
				if (par[a] > par[b]) std::swap(a, b);
				par[a] += par[b];
				par[b] = a;
				return true;
			};

			std::vector<std::vector<int>> outEdges(N);
			std::vector<int> inDeg(N);
			for (int i = 0; i < N; i++) {
				if (G[i] != F[i]) {
					// if it's not equal, it must be equal to something that's originally a root
					if (F[G[i]] != G[i]) {
						return false;
					}
					path_compress(i);
					assert(F[F[i]] == F[i]);
					// Now, F[i] must be before G[i] in the toposort, and they must be linked
					merge(F[i], G[i]);
					inDeg[G[i]]++;
					outEdges[F[i]].push_back(G[i]);
				}
			}

			std::vector<std::pair<int, int>> comps; comps.reserve(N);
			for (int i = 0; i < N; i++) {
				if (F[i] == i) {
					if (inDeg[i] == 0) {
						comps.push_back({get_par(i), i});
					}
				}
			}
			std::sort(comps.begin(), comps.end());

			auto it = comps.begin();
			std::vector<int> q; q.reserve(N);
			for (int z = 0; z < int(q.size()) || it != comps.end(); z++) {
				if (z == int(q.size())) {
					assert(it != comps.end());
					q.push_back(it->second);
					++it;
				}
				for (int nxt : outEdges[q[z]]) {
					inDeg[nxt]--;
					if (inDeg[nxt] == 0) q.push_back(nxt);
				}
			}

			for (int i = 0; i < N; i++) {
				if (F[i] == i) {
					if (inDeg[i] != 0) return false;
				}
			}

			// now, we have them linked up in a line, and also components should be adjacent
			for (int z = 0; z+1 < int(q.size()); z++) {
				int cur = q[z];
				int nxt = q[z+1];
				assert(F[cur] == cur);
				assert(F[nxt] == nxt);
				assert(get_par(cur) <= get_par(nxt));
				if (get_par(cur) < get_par(nxt)) {
					assert(G[cur] == cur);
					continue;
				}
				if (G[cur] == cur) {
					return false;
				}
				ops.push_back({cur, nxt});
				F[cur] = nxt;
				for (int i = 0; i < N; i++) {
					if (F[i] == cur && G[i] != cur) {
						path_compress(i);
						assert(F[i] == nxt);
					}
				}
			}
			assert(F == G);
			return true;
		};
		if (solve()) {
			cout << "YES" << '\n';
			cout << ops.size() << '\n';
			for (auto [x, y] : ops) {
				if (y == -1) {
					cout << 1 << ' ' << x+1 << '\n';
				} else {
					cout << 2 << ' ' << x+1 << ' ' << y+1 << '\n';
				}
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
