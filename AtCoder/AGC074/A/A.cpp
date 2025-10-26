#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [x, y] : edges) {
			cin >> x >> y; x--, y--;
		}
		{
			std::vector<int> P(N);
			for (auto& p : P) { cin >> p; p--; }
			for (auto& [x, y] : edges) { x = P[x], y = P[y]; assert(x < y); }
		}
		std::vector<int> nxt(N, N);
		std::vector<int> prv(N, -1);
		for (auto& [x, y] : edges) {
			prv[y] = std::max(prv[y], x);
			nxt[x] = std::min(nxt[x], y);
		}
		int ans = 0;
		std::vector<int> dp_prv(N);
		std::vector<int> dp_nxt(N);
		for (int i = 0; i < N; i++) {
			if (i > 0) {
				dp_prv[i] = std::max(dp_prv[i], dp_prv[i-1]);
				dp_nxt[i] = std::max(dp_nxt[i], dp_nxt[i-1]);
			}
			int v = dp_nxt[i] + 1;
			if (prv[i] >= 0) {
				v = std::max(v, dp_prv[prv[i]] + 1);
			}
			ans = std::max(ans, v);

			dp_prv[i] = std::max(dp_prv[i], v);
			if (nxt[i] < N) {
				dp_nxt[nxt[i]] = std::max(dp_nxt[nxt[i]], v);
			}
		}
		cout << N - ans << '\n';
	}

	return 0;
}
