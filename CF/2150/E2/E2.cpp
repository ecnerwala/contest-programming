#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = [&]() -> int {
			if (N == 1) return 0;

			std::vector<int> P(2*N-1);
			std::iota(P.begin(), P.end(), 0);
			std::shuffle(P.begin(), P.end(), mt);

			auto query = [&](int v, int l, int r) -> bool {
				cout << '?' << ' ' << v+1 << ' ' << r-l;
				for (int p = l; p < r; p++) cout << ' ' << P[p]+1;
				cout << '\n' << flush;
				int res; cin >> res; if (res == -1) exit(0);
				assert(res == 0 || res == 1);
				return res;
			};

			std::vector<int> cnt(N, 2);
			int l = 0, r = 2*N-1;
			for (int z = 0; z < 1; z++) {
				int m = (l+r) / 2;
				std::vector<int> cnt_l(N);
				std::vector<int> cnt_r(N);
				for (int i = 0; i < N; i++) {
					if (cnt[i] == 2) {
						cnt_l[i] = query(i, l, m);
						if (cnt_l[i] == 0) {
							// we get to fastbreak
							cnt_r[i] = 2;
							continue;
						}
						cnt_r[i] = query(i, m, r);
						int s = 2 / (cnt_l[i] + cnt_r[i]);
						cnt_l[i] *= s;
						cnt_r[i] *= s;
						assert(cnt_l[i] + cnt_r[i] == cnt[i]);
					}
				}
				for (int i = 0; i < N; i++) {
					if (cnt[i] == 1) {
						cnt_r[i] = query(i, m, r);
						cnt_l[i] = 1 - cnt_r[i];
					}
				}
				for (int i = 0; i < N; i++) {
					assert(cnt_l[i] + cnt_r[i] == cnt[i]);
				}
				int tot_l = std::accumulate(cnt_l.begin(), cnt_l.end(), 0);
				int tot_r = std::accumulate(cnt_r.begin(), cnt_r.end(), 0);
				assert(tot_l + tot_r == (r - l) + 1);
				if (tot_l > m - l) {
					r = m;
					cnt = cnt_l;
				} else {
					l = m;
					cnt = cnt_r;
				}
			}
			for (int i = 0; i < N; i++) {
				if (cnt[i] == 2) {
					int mi = l, ma = r;
					while (true) {
						if (ma - mi == 1) return i;
						int md = (mi + ma) / 2;
						if (!query(i, mi, md)) {
							// go right instead
							mi = md;
						} else if (!query(i, md, ma)) {
							// go left instead
							ma = md;
						} else {
							break;
						}
					}
				}
			}
			assert(false);
		}();
		cout << '!' << ' ' << ans+1 << '\n' << flush;
	}

	return 0;
}
