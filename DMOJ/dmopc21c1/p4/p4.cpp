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

	int N, M; int64_t K; cin >> N >> M >> K;
	std::vector<std::vector<tuple<int, int64_t, int64_t>>> adj(N);

	int64_t tot_len = 0;

	for (int e = 0; e < M; e++) {
		int u, v; int64_t l, b; cin >> u >> v >> l >> b; u--, v--;
		tot_len += l;
		adj[u].push_back({v, l, b});
		adj[v].push_back({u, l, b});
	}

	std::vector<int> roots; roots.reserve(N-M);
	{
		std::vector<bool> vis(N);
		for (int i = 0; i < N; i++) {
			if (!vis[i]) {
				roots.push_back(i);
				std::y_combinator([&](auto self, int cur, int prv) -> void {
					vis[cur] = true;
					for (auto [nxt, l, cost] : adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur);
					}
				})(i, -1);
			}
		}
	}

	auto get_cost = [&](int64_t diam) -> int64_t {
		int64_t ans = 0;
		auto dfs = std::y_combinator([&](auto self, int cur, int prv) -> std::vector<std::pair<int64_t, int64_t>> {
			std::vector<std::pair<int64_t, int64_t>> a;
			a.push_back({0, 0});
			for (auto [nxt, l, cost] : adj[cur]) {
				if (nxt == prv) continue;
				auto b = self(nxt, cur);
				b.insert(b.begin(), {0, b.back().second + cost});
				for (int i = 1; i < int(b.size()); i++) {
					b[i].first += l;
					b[i].second = min(b[i].second, b[i-1].second);
				}
				while (b.back().first > diam) b.pop_back();

				{
					// meld a and b
					std::vector<std::pair<int64_t, int64_t>> na;
					std::vector<std::pair<int64_t, int64_t>> nb;
					for (int z = 0; z < 2; z++, std::swap(a, b), std::swap(na, nb)) {
						na = a;
						for (int i = 0, j = 0; i < int(a.size()); i++) {
							int64_t up = min(a[i].first, diam - a[i].first);
							assert(up >= 0);
							while (j < int(b.size()) && b[j].first <= up) j++;
							while (j > 0 && b[j-1].first > up) j--;
							assert(j > 0);
							na[i].second += b[j-1].second;
						}
					}
					a = std::move(na);
					b = std::move(nb);
				}

				std::vector<std::pair<int64_t, int64_t>> c(a.size()+b.size());
				std::merge(a.begin(), a.end(), b.begin(), b.end(), c.begin());
				for (int i = 1; i < int(c.size()); i++) {
					c[i].second = min(c[i].second, c[i-1].second);
				}
				a = std::move(c);
			}
			return a;
		});
		for (int rt : roots) {
			ans += dfs(rt, -1).back().second;
		}
		return ans;
	};

	int64_t mi = -1;
	int64_t ma = tot_len+1;
	while (ma - mi > 1) {
		int64_t md = mi + (ma - mi) / 2;
		if (get_cost(md) <= K) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';

	return 0;
}
