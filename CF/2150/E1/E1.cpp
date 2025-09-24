#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> cnt(N, 2);
		int l = 0, r = 2*N-1;
		while (true) {
			int ans_cnd = -1;
			for (int i = 0; i < N; i++) {
				if (cnt[i] == 2) {
					if (ans_cnd == -1) ans_cnd = i;
					else ans_cnd = -2;
				}
			}
			assert(ans_cnd != -1);
			if (ans_cnd != -2) {
				cout << '!' << ' ' << ans_cnd+1 << '\n' << flush;
				break;
			}
			int m = (l+r) / 2;
			std::vector<int> cnt_l(N);
			std::vector<int> cnt_r(N);
			for (int i = 0; i < N; i++) {
				if (cnt[i] == 2) {
					{
						cout << '?' << i+1 << ' ' << m-l;
						for (int p = l; p < m; p++) cout << ' ' << p+1;
						cout << '\n' << flush;
						int res; cin >> res; if (res == -1) exit(0);
						cnt_l[i] = res;
					}
					if (cnt_l[i] == 0) {
						// we get to fastbreak
						cnt_r[i] = 2;
						continue;
					}
					{
						cout << '?' << i+1 << ' ' << r-m;
						for (int p = m; p < r; p++) cout << ' ' << p+1;
						cout << '\n' << flush;
						int res; cin >> res; if (res == -1) exit(0);
						cnt_r[i] = res;
					}
					int s = 2 / (cnt_l[i] + cnt_r[i]);
					cerr << cnt[i] << ' ' << cnt_l[i] << ' ' << cnt_r[i] << ' ' << s << '\n';
					cnt_l[i] *= s;
					cnt_r[i] *= s;
					assert(cnt_l[i] + cnt_r[i] == cnt[i]);
				}
			}
			for (int i = 0; i < N; i++) {
				if (cnt[i] == 1) {
					{
						cout << '?' << i+1 << ' ' << r-m;
						for (int p = m; p < r; p++) cout << ' ' << p+1;
						cout << '\n' << flush;
						int res; cin >> res; if (res == -1) exit(0);
						cnt_r[i] = res;
					}
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
	}

	return 0;
}
