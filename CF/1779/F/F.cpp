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
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> P(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

	std::vector<std::vector<std::pair<uint32_t, int>>> dp(N);
	std::vector<int> sz(N);
	for (int i = 0; i < N; i++) {
		dp[i].push_back({uint32_t(1) << A[i], -1});
		sz[i] = 1;
	}
	for (int i = N-1; i >= 0; i--) {
		uint32_t vi = dp[i].back().first;
		if (!sz[i]) {
			vi |= 1;
			dp[i].push_back({vi, -2});
		}

		int p = P[i];
		if (p == -1) break;
		sz[p] ^= sz[i];
		uint32_t vp = dp[p].back().first;

		uint32_t nv = 0;
		for (int a = 0; a < 32; a++) {
			for (int b = 0; b < 32; b++) {
				if ((vi >> a) & (vp >> b) & 1) {
					nv |= uint32_t(1) << (a^b);
				}
			}
		}
		dp[p].push_back({nv, i});
	}
	if (!(dp[0].back().first & 1)) {
		cout << -1 << '\n';
		exit(0);
	}
	std::vector<int> ops; ops.reserve(N+1);

	std::y_combinator([&](auto self, int cur, int tgt) -> void {
		assert(dp[cur].back().first & (uint32_t(1) << tgt));
		if (sz[cur] == 0) {
			if (tgt == 0) {
				ops.push_back(cur);
				return;
			}
			assert(dp[cur].back().second == -2);
			dp[cur].pop_back();
		}
		while (dp[cur].back().second != -1) {
			assert(dp[cur].back().first & (uint32_t(1) << tgt));
			int nxt = dp[cur].back().second;
			dp[cur].pop_back();
			for (int v = 0; v < 32; v++) {
				if ((dp[cur].back().first >> (tgt ^ v)) & (dp[nxt].back().first >> v) & 1) {
					self(nxt, v);
					tgt ^= v;
					goto found;
				}
			}
			assert(false);
found:
			;
		}
		assert(dp[cur].back().first & (uint32_t(1) << tgt));
		assert(tgt == A[cur]);
	})(0, 0);

	ops.push_back(0);
	cout << ops.size() << '\n';
	for (int z = 0; z < int(ops.size()); z++) {
		cout << ops[z]+1 << " \n"[z+1==int(ops.size())];
	}

	return 0;
}
