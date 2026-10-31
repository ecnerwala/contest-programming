#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int A, B, AB, BA; cin >> A >> B >> AB >> BA;
		string S; cin >> S;
		cout << ([&]() -> bool {
			assert(int(S.size()) == A + B + 2 * AB + 2 * BA);
			std::array<int, 2> cnt{0, 0};
			for (char c : S) {
				cnt[c-'A']++;
			}
			if (A + AB + BA != cnt[0] || B + AB + BA != cnt[1]) {
				return false;
			}
			// we just need to split S into ABs and BAs
			int tot_cnt = 0;
			std::array<std::vector<int>, 2> cnts;
			std::array<int, 2> cur_cnts{0, 0};
			for (int i = 0; i < int(S.size()); ) {
				int j = i;
				while (j+1 < int(S.size()) && S[j] != S[j+1]) {
					j++;
				}
				int seq_len = j - i + 1;
				tot_cnt += seq_len / 2;
				if (!(seq_len & 1)) {
					cnts[S[i] - 'A'].push_back(seq_len / 2);
					cur_cnts[S[i] - 'A'] += seq_len / 2;
				}

				i = j+1;
			}

			tot_cnt -= AB + BA;
			cur_cnts[0] -= AB;
			cur_cnts[1] -= BA;
			for (int z = 0; z < 2; z++) {
				std::sort(cnts[z].begin(), cnts[z].end());
				while (cur_cnts[z] > 0) {
					int v = cnts[z].back();
					cnts[z].pop_back();
					cur_cnts[z] -= v;
					tot_cnt --;
				}
			}

			return tot_cnt >= 0;
		}() ? "YES" : "NO") << '\n';
	}

	return 0;
}
