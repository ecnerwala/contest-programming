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

	int N; cin >> N;
	std::vector<std::vector<int>> adj(4*N);
	for (int i = 0; i < N; i++) {
		adj[i].reserve(3);
		for (int z = 0; z < 3; z++) {
			int v; cin >> v; v--;
			assert(0 <= v && v < N);
			v += (z+1) * N;
			adj[i].push_back(v);
			adj[v].push_back(i);
		}
	}

	std::vector<std::array<int, 2>> ops; ops.reserve(3*N);
	std::vector<int> cnt(N, 0);
	std::vector<int> waiting(4*N, -1);
	auto add_edge = [&](int a, int b) -> void {
		if (a > b) std::swap(a, b);
		assert(a < N);
		assert(b >= N);
		cnt[a]++;
		assert(cnt[a] <= 3);
		if (waiting[b] == -1) {
			waiting[b] = a;
		} else {
			ops.push_back({a, waiting[b]});
			waiting[b] = -1;
		}
	};
	std::vector<bool> vis(4*N);
	for (int st = 0; st < N; st++) {
		if (vis[st]) continue;
		bool bad = std::y_combinator([&](auto&& self, int cur) -> bool {
			if (!vis[cur]) return false;
			vis[cur] = true;
			bool val = (cur < N ? true : false);
			for (int nxt : adj[cur]) {
				if (self(nxt)) {
					add_edge(cur, nxt);
					val = !val;
				}
			}
			return val;
		})(st);
		if (bad) {
			cnt[st] ++;
		}
	}

	for (int i = 0; i < 4*N; i++) {
		assert(waiting[i] == -1);
	}

	for (int i = 0; i < N; i++) {
		assert(cnt[i] & 1);
		while (cnt[i] <= 3) {
			ops.push_back({i, i});
			cnt[i] += 2;
		}
	}
	cout << ops.size() << '\n';
	for (auto [u, v] : ops) {
		cout << u+1 << ' ' << v+1 << '\n';
	}

	return 0;
}
