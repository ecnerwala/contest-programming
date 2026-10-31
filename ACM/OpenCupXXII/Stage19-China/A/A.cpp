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
		std::vector<std::vector<int>> ch(N);
		for (int e = 0; e < N-1; e++) {
			int x, y; cin >> x >> y; x--, y--;
			ch[x].push_back(y);
		}
		std::vector<int> depth(N);
		std::vector<int> sz(N);
		std::y_combinator([&](auto self, int cur, int d) -> int {
			depth[cur] = d;
			int v = 1;
			for (int nxt : ch[cur]) {
				v += self(nxt, d+1);
			}
			sz[cur] = v;
			return v;
		})(0, 0);
		for (int i = 0; i < N; i++) {
			cout << depth[i]+1 << ' ' << N + 1 - sz[i] << '\n';
		}
	}

	return 0;
}
