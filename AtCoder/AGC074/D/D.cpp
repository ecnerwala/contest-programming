#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto c2 = [](int a) -> int {
		return a * (a-1) / 2;
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		cerr << "case " << N << '\n';
		const int L = c2(N);
		std::vector<int> B(L);
		for (auto& b : B) { cin >> b; assert(b == 0 || b == 1); }
		std::vector<int> dp_fewest(L+1, -1);
		dp_fewest[0] = N;
		for (int i = N; i >= 2; i--) {
			int dl = c2(i);
			for (int l = 0; l + dl <= L; l++) {
				if (i <= dp_fewest[l]) {
					dp_fewest[l + dl] = std::max(dp_fewest[l + dl], dp_fewest[l] - i);
				}
			}
		}
		int64_t ans = -1;
		int tgt = std::accumulate(B.begin(), B.end(), 0);
		if (dp_fewest[tgt] != -1) {
			ans = 0;
		} else {
			int max_prv = tgt;
			while (dp_fewest[max_prv] == -1) max_prv--;
			int last_prv = -1;
			cerr << "tgt " << tgt << '\n';
			for (int nxt = tgt; nxt <= L; nxt++) {
				if (dp_fewest[nxt] == -1) continue;
				int prv = -1;
				for (int s = 2; s <= N && c2(s) <= nxt; s++) {
					if (s > dp_fewest[nxt - c2(s)]) continue;
					for (int i = 1, j = s-1; i <= j; i++, j--) {
						int cnd = nxt - c2(s) + c2(i) + c2(j);
						if (cnd <= tgt) prv = std::max(prv, cnd);
					}
				}
				if (prv <= last_prv) continue;

				assert(prv < tgt && tgt < nxt);

				int64_t cnd = 0;
				int cnt_0 = L - tgt;
				int cnt_1 = 0;
				for (int v : B) {
					if (v == 1) {
						cnt_1++;
						if (cnt_1 > prv) cnd += std::max(0, cnt_0 - (L - nxt));
					} else if (v == 0) {
						cnt_0--;
					} else assert(false);
				}
				cerr << prv << ' ' << nxt << ' ' << cnd << '\n';
				assert(cnt_0 == 0);
				assert(cnt_1 == tgt);
				if (ans == -1 || cnd < ans) ans = cnd;

				last_prv = prv;
				assert(prv <= max_prv);
				if (prv == max_prv) break;
			}
		}

		cout << ans << '\n';
	}

	return 0;
}
