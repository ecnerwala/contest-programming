#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }
		std::vector<int> dp; dp.reserve(N);
		std::vector<int> nxt(N, -1);
		for (int i = N-1; i >= 0; i--) {
			int v = P[i];
			int mi = -1;
			int ma = int(dp.size());
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (dp[md] > v) {
					mi = md;
				} else {
					ma = md;
				}
			}
			if (ma == int(dp.size())) {
				nxt[v] = -1;
				dp.push_back(v);
			} else {
				nxt[v] = dp[ma];
				dp[ma] = v;
			}
		}
		std::vector<int> res; res.reserve(N);
		while (!dp.empty()) {
			int cur = dp.back(); dp.pop_back();
			for (; cur != -1; cur = nxt[cur]) {
				res.push_back(cur);
			}
		}
		for (int i = 0; i < N; i++) {
			cout << res[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
