#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		// total answer: N+2 choose 3
		int64_t tot_ans = int64_t(N+2) * int64_t(N+1) * int64_t(N) / 6;

		for (int z = 0; z < 2; z++, std::reverse(A.begin(), A.end())) {
			std::vector<int> nxt_occ(N, N);
			std::vector<std::pair<int, int>> stk; stk.reserve(N+1);
			int64_t cur_val = 0;
			for (int i = N-1; i >= 0; i--) {
				int j = std::exchange(nxt_occ[A[i]], i);
				while (!stk.empty() && stk.back().second >= j) {
					int oj = stk.back().second;
					stk.pop_back();
					if (!stk.empty()) {
						cur_val -= int64_t(oj - stk.back().second) * stk.back().first;
					}
				}
				if (!stk.empty()) {
					cur_val += int64_t(j - stk.back().second) * stk.back().first;
				}
				stk.push_back({i, j});

				int64_t pref_del = cur_val - int64_t(stk.back().second - stk.front().second) * i;
				tot_ans -= pref_del;
				cerr << z << ' ' << i << ' ' << pref_del << '\n';

				if (z == 0) {
					int del_all = stk.front().second - i;
					cerr << i << ' ' << del_all << '\n';
					tot_ans -= int64_t(del_all + 1) * int64_t(del_all) / 2;
				}
			}
		}
		tot_ans += N;
		cout << tot_ans << '\n';
	}

	return 0;
}
