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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> ans(N);
	std::vector<std::array<int, 2>> times(N);
	for (int i = 0; i < N; i++) {
		cin >> times[i][0] >> times[i][1] >> ans[i];
	}
	{
		std::vector<std::pair<int, int>> coords(N);
		for (int i = 0; i < N; i++) {
			coords[i] = {times[i][1], i};
		}
		sort(coords.begin(), coords.end());
		for (int i = 0; i < N; i++) {
			times[coords[i].second][1] = i;
		}
		for (int i = 0; i < N; i++) {
			times[i][0] = int(std::lower_bound(coords.begin(), coords.end(), std::pair<int, int>{times[i][0],-1}) - coords.begin());
			assert(times[i][0] <= times[i][1]);
		}
	}

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			a = par[a];
		}
		return a;
	};

	std::vector<tuple<int, int, int>> undo_stack; undo_stack.reserve(N+M);

	auto merge = [&](int a, int b) -> void {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return;
		if (par[a] > par[b]) std::swap(a, b);
		undo_stack.push_back({a, b, par[b]});
		par[a] += par[b];
		par[b] = a;
		setmax(ans[a], ans[b]);
	};
	auto undo_to = [&](int loc) -> void {
		while (int(undo_stack.size()) > loc) {
			auto [a, b, v] = undo_stack.back();
			undo_stack.pop_back();

			setmax(ans[b], ans[a]);

			par[b] = v;
			par[a] -= v;
		}
	};

	std::vector<int> all_alive(N); std::iota(all_alive.begin(), all_alive.end(), 0);
	std::y_combinator([&](auto self, int lo, int hi, std::vector<int> alive) -> void {
		assert(lo <= hi);
		int checkpoint = int(undo_stack.size());
		{
			int L = 0;
			for (int z = 0; z < int(alive.size()); z++) {
				int i = alive[z];
				if (times[i][0] <= lo && hi <= times[i][1]) {
					for (int j : adj[i]) {
						if (times[j][0] > times[i][1]) {
							//assert(par[j] == -1);
							setmax(ans[get_par(i)], ans[j]);
						}
						if (times[j][0] <= lo && hi <= times[j][1]) {
							merge(i, j);
						}
					}
				} else if (times[i][1] < lo || times[i][0] > hi) {
					// we're bad
				} else {
					alive[L++] = i;
				}
			}
			alive.resize(L);
		}
		if (hi - lo >= 1) {
			int md = lo + (hi-lo) / 2;
			self(md+1, hi, alive);
			self(lo, md, alive);
		}
		undo_to(checkpoint);
	})(0, N-1, all_alive);

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
