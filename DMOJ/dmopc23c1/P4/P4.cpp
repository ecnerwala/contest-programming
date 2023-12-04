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
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int a, b, c; cin >> a >> b >> c; a--, b--;
		adj[a].push_back({b, c});
		adj[b].push_back({a, c});
	}

	const int64_t INF = 1e18;
	int64_t ans = 0;
	std::y_combinator([&](auto self, int cur, int prv) -> std::array<int64_t, 3> {
		std::array<int64_t, 3> res{0, -INF, 0};
		for (auto [nxt, c] : adj[cur]) {
			if (nxt == prv) continue;
			auto ch = self(nxt, cur);
			assert(c >= 1);
			if (c == 1) {
				ch[1]++;
				ch[0] = ch[2] = 0;
			} else if (c & 1) {
				ch[1] += c;
				ch[0] += c-1;
				ch[2] += c-1;
			} else {
				ch[0] += c;
				ch[2] += c;
				ch[1] += c-1;
			}
			std::array<int64_t, 3> nres{-INF, -INF, -INF};
			for (int a = 0; a <= 2; a++) {
				for (int b = 0; a+b <= 2; b++) {
					nres[a+b] = std::max(nres[a+b], res[a] + ch[b]);
				}
			}
		}
		ans = std::max(ans, res[2]);
		return res;
	})(0, -1);
	cout << ans << '\n';

	return 0;
}

// We need all vertex parities to be even except 2 of them
// c_i == 1 is kinda bad
