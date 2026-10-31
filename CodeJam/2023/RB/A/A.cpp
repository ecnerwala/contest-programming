#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int64_t> pref(N+1);
		pref[0] = 0;
		for (int i = 0; i < N; i++) {
			int a; cin >> a;
			pref[i+1] = pref[i] + a;
		}

		int LA, RA, LB, RB; cin >> LA >> RA >> LB >> RB;
		LA--, RA--, LB--, RB--;

		int64_t res = 0;
		for (int a = LA; a <= RA; a++) {
			int64_t cur_score = pref[N];
			for (int b : {a-1, a+1, LB, RB}) {
				if (b == a) continue;
				if (b < LB) continue;
				if (b > RB) continue;
				if (b > a) {
					cur_score = std::min(cur_score, pref[a + (b - a) / 2 + 1]);
				} else if (b < a) {
					cur_score = std::min(cur_score, pref[N] - pref[a - (a - b) / 2]);
				} else assert(false);
			}
			res = std::max(res, cur_score);
		}

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
