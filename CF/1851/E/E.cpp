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
		int N, K; cin >> N >> K;
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;
		for (int k = 0; k < K; k++) {
			int p; cin >> p; p--; C[p] = 0;
		}

		std::vector<std::vector<int>> mixes(N);
		for (auto& v : mixes) {
			int l; cin >> l;
			v.resize(l);
			for (auto& e : v) { cin >> e; e--; }
		}

		std::vector<int> mem(N, -1);
		auto solve = std::y_combinator([&](auto self, int cur) -> int {
			if (mem[cur] != -1) return mem[cur];
			mem[cur] = C[cur];
			if (mixes[cur].empty()) {
				return mem[cur] = C[cur];
			}
			int64_t v = 0;
			for (int nxt : mixes[cur]) {
				v += self(nxt);
			}
			return mem[cur] = int(std::min<int64_t>(C[cur], v));
		});
		for (int i = 0; i < N; i++) {
			cout << solve(i) << " \n"[i+1==N];
		}
	}

	return 0;
}
