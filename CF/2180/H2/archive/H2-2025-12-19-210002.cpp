#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto solve = [&](int64_t g_hi, int64_t d) -> int {
		assert(d >= 1);
		if (d & 1) {
			g_hi = g_hi / d + ((g_hi % d) & 1) + 1;
			int cnt = __builtin_popcountll(g_hi) - 2 * __builtin_popcountll(g_hi & (uint64_t(-4)/3));
			return std::array<int, 4>{0, 2, 3, 1}[cnt & 3];
		} else {
			int64_t g = g_hi / d + 1;
			return (g_hi & 1) ? (__builtin_ctzll(g) & 1) ? 0 : __builtin_popcountll(g) & 1 ? 1 : 2 : 0;
		}
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			int64_t a, b, c, l, r; cin >> a >> b >> c >> l >> r;
			ans ^= solve(r-c+1, c-b) ^ solve(l-c, c-b);
		}
		cout << (ans ? "Bug" : "Feature") << '\n';
	}

	return 0;
}
