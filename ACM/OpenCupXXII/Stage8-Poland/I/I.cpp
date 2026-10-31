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
		const int B = 21;
		std::vector<std::array<int, 2>> ch; ch.reserve(1 + N * B);
		std::vector<int> sz; sz.reserve(1 + N*B);
		ch.push_back({-1, -1});
		sz.push_back(0);
		const int ROOT = 0;
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			int cur = ROOT;
			sz[cur]++;
			for (int b = B-1; b >= 0; b--) {
				int d = (a >> b) & 1;
				if (ch[cur][d] == -1) {
					ch[cur][d] = int(ch.size());
					ch.push_back({-1, -1});
					sz.push_back(0);
				}
				cur = ch[cur][d];
				sz[cur]++;
			}
		}

		int ans = std::y_combinator([&](auto self, int l, int r, int b) -> int {
			if (l == -1 && r == -1) return 0;
			if (l == -1) return sz[r];
			if (r == -1) return sz[l];
			if (b == -1) {
				if (l == r) {
					return sz[l];
				} else {
					return sz[l] + sz[r];
				}
			}
			if (K & (1 << b)) {
				if (l == r) {
					return self(ch[l][0], ch[r][1], b-1);
				} else {
					return self(ch[l][0], ch[r][1], b-1) + self(ch[r][0], ch[l][1], b-1);
				}
			} else {
				if (l == r) {
					return std::max(self(ch[l][0], ch[r][0], b-1), self(ch[l][1], ch[r][1], b-1));
				} else {
					return std::max({self(ch[l][0], ch[r][0], b-1), self(ch[l][1], ch[r][1], b-1), sz[l], sz[r]});
				}
			}
		})(ROOT, ROOT, B-1);
		cout << ans << '\n';
	}

	return 0;
}
