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

int64_t solve_cycle(std::vector<int> sizes, std::vector<int> lens) {
	using std::vector;

	int L = int(sizes.size());
	assert(L == int(lens.size()));
	vector<int64_t> locs(L+1);
	locs[0] = 0;
	for (int i = 0; i < L; i++) locs[i+1] = locs[i] + lens[i];
	vector<int64_t> pref_sz(L+1);
	pref_sz[0] = 0;
	for (int i = 0; i < L; i++) pref_sz[i+1] = pref_sz[i] + sizes[i];
	vector<int64_t> pref_prod(L+1);
	pref_prod[0] = 0;
	for (int i = 0; i < L; i++) pref_prod[i+1] = pref_prod[i] + int64_t(sizes[i]) * int64_t(locs[i]);

	int64_t ans = 0;
	for (int i = 0, lo = 0, hi = 0; i < L; i++) {
		while (lo < i && locs[L] - (locs[i] - locs[lo]) < locs[i] - locs[lo]) lo++;
		while (hi < L && locs[hi] - locs[i] < locs[L] - (locs[hi] - locs[i])) hi++;
		int64_t v = 0;
		// we go from i to [lo,hi) without wrapping
		v += (pref_prod[hi] - pref_prod[i]) - (pref_sz[hi] - pref_sz[i]) * locs[i];
		v += (pref_sz[i] - pref_sz[lo]) * locs[i] - (pref_prod[i] - pref_prod[lo]);

		v += (pref_sz[L] - pref_sz[hi]) * (locs[L] + locs[i]) - (pref_prod[L] - pref_prod[hi]);
		v += (pref_prod[lo] - pref_prod[0]) - (pref_sz[lo] - pref_sz[0]) * (locs[i] - locs[L]);

		ans += v * sizes[i];
	}

	assert(ans % 2 == 0);
	ans /= 2;

	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<vector<pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v, w; cin >> u >> v >> w; u--, v--;
		adj[u].push_back({v, w});
		adj[v].push_back({u, w});
	}
	vector<pair<int, int>> par(N);
	vector<int> sz(N);
	par[0] = {-1, 0};
	int64_t orig_ans = 0;
	std::y_combinator([&](auto self, int cur) -> void {
		sz[cur] = 1;
		for (auto [nxt, d] : adj[cur]) {
			if (nxt == par[cur].first) continue;
			par[nxt] = {cur, d};
			self(nxt);
			sz[cur] += sz[nxt];
		}
		orig_ans += int64_t(sz[cur]) * int64_t(N - sz[cur]) * int64_t(par[cur].second);
	})(0);

	int Q; cin >> Q;
	while (Q--) {
		int x, y, d_new; cin >> x >> y >> d_new; x--, y--;
		int64_t ans = orig_ans;
		int cur_x_sz = 0;
		vector<pair<int, int>> x_path;
		int cur_y_sz = 0;
		vector<pair<int, int>> y_path;
		while (x != y) {
			if (sz[x] <= sz[y]) {
				ans -= int64_t(sz[x]) * int64_t(N-sz[x]) * int64_t(par[x].second);
				x_path.push_back({sz[x] - std::exchange(cur_x_sz, sz[x]), par[x].second});
				x = par[x].first;
			} else {
				ans -= int64_t(sz[y]) * int64_t(N-sz[y]) * int64_t(par[y].second);
				y_path.push_back({sz[y] - std::exchange(cur_y_sz, sz[y]), par[y].second});
				y = par[y].first;
			}
		}
		assert(x == y);
		int L = int(x_path.size()) + int(y_path.size()) + 1;
		vector<int> sizes; sizes.reserve(L);
		vector<int> lens; lens.reserve(L);
		for (auto it : x_path) {
			sizes.push_back(it.first);
			lens.push_back(it.second);
		}
		sizes.push_back(N - cur_x_sz - cur_y_sz);
		reverse(y_path.begin(), y_path.end());
		for (auto it : y_path) {
			sizes.push_back(it.first);
			lens.push_back(it.second);
		}
		lens.push_back(d_new);

		ans += solve_cycle(sizes, lens);

		cout << ans << '\n';
	}

	return 0;
}
