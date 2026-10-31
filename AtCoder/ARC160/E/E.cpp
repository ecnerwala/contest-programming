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
		std::vector<int> W(N);
		for (auto& w : W) cin >> w;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		int num_take = 0;
		std::vector<int> take_weight(N);
		for (int i = 0; i < N; i++) {
			if (adj[i].size() == 1) {
				num_take++;
				take_weight[i] = 1;
			}
		}
		if (num_take & 1) {
			int num_3 = 0;
			for (int j = 0; j < N; j++) {
				if (adj[j].size() == 3) num_3++;
			}
			int i = -1;
			for (int j = 0; j < N; j++) {
				if (num_3 == 1 && adj[j].size() == 3) continue;
				if (i == -1 || W[j] < W[i]) i = j;
			}
			assert(i != -1);
			take_weight[i]++;
			num_take++;
		}

		int centroid = -1;
		std::y_combinator([&](auto self, int cur, int prv) -> int {
			int sz = take_weight[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				sz += self(nxt, cur);
			}
			if (centroid == -1 && sz * 2 >= num_take) {
				centroid = cur;
			}
			return sz;
		})(0, -1);

		if (adj[centroid].size() == 3 && take_weight[centroid] >= 1) {
			// we're fine, deleting the centroid isn't a problem at all
			assert(take_weight[centroid] == 1);
			auto get_sz = std::y_combinator([&](auto self, int cur, int prv) -> int {
				int sz = take_weight[cur];
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					sz += self(nxt, cur);
				}
				return sz;
			});
			std::vector<int> szs(3);
			for (int z = 0; z < 3; z++) {
				szs[z] = get_sz(adj[centroid][z], centroid);
			}
			assert(szs[0] + szs[1] + szs[2] + take_weight[centroid] == num_take);
			assert(szs[0] * 2 <= num_take);
			assert(szs[1] * 2 <= num_take);
			assert(szs[2] * 2 <= num_take);
			if (szs[1] == 1) {
				std::swap(adj[centroid][0], adj[centroid][1]);
			}
		}

		std::vector<int> takes; takes.reserve(num_take);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			for (int z = 0; z < take_weight[cur]; z++) {
				takes.push_back(cur);
			}
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(centroid, -1);
		cout << num_take / 2 << '\n';
		for (int i = 0; i < num_take / 2; i++) {
			cout << takes[i]+1 << ' ' << takes[num_take/2 + i]+1 << '\n';
		}
	}

	return 0;
}
