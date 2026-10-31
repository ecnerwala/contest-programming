#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t K; cin >> N >> K;
		assert(N > 0);
		int64_t A0, B0; cin >> A0 >> B0;

		struct op_t { int64_t a, b; };
		std::vector<op_t> ops(N-1);
		for (auto& op : ops) cin >> op.a >> op.b;

		std::sort(ops.begin(), ops.end(), [&](auto x, auto y) { return x.a < y.a; });

		const int64_t INF = 1e18;
		int64_t ans = -INF;
		for (int i = 0; i < int(ops.size()); i++) {
			ans = std::max({
				ans,
				ops[i].a,
				std::min(ans, ops[i].a) + ops[i].b,
				(i>0) ? ops[i-1].a + ops[i-1].b : -INF
			});
		}

		ans = std::max(A0, std::min(ans, A0) + B0);
		cout << (K <= ans ? "YES" : "NO") << '\n';
	}

	return 0;
}
