#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	int N = int(S.size());
	std::vector<int> Hs; Hs.reserve(N+2);
	Hs.push_back(-1);
	for (int i = 0; i < N; i++) {
		if (S[i] == 'H') Hs.push_back(i);
	}
	Hs.push_back(N);

	int64_t tot_ans = 0;

	int NH = int(Hs.size())-2;
	for (int Hsum = 2; Hsum <= 2*NH; Hsum++) {
		std::vector<int> cnt_by_sum(2*N);
		int cur_ans = 0;
		int cur_tgt_sum = 0;
		int cur_cnt_lt = 0;
		int cur_cnt_gt = 0;
		for (int Hd = (Hsum & 1); 1 <= (Hsum-Hd)/2 && (Hsum+Hd)/2 <= NH; Hd += 2) {
			int Hlo = (Hsum - Hd)/2;
			int Hhi = (Hsum + Hd)/2;
			{
				int s = Hs[Hlo] + Hs[Hhi];
				if (Hd <= 1) cur_tgt_sum = s;
				if (Hd == 0) {
					cnt_by_sum[s] += 1;
				} else {
					cnt_by_sum[s] += 2;
					if (s < cur_tgt_sum) cur_cnt_lt += 2;
					else if (s > cur_tgt_sum) cur_cnt_gt += 2;
					cur_ans += 2 * abs(s - cur_tgt_sum);
				}
			}
			for (int x = Hs[Hlo-1]+1; x <= Hs[Hlo]; x++) {
				for (int y = Hs[Hhi]; y <= Hs[Hhi+1]-1; y++) {
					int s = x+y;
					if ((Hsum & 1) == 0 && (s & 1) != 0) {
						tot_ans += -1;
						continue;
					}
					while (cur_tgt_sum > s) {
						cur_cnt_gt += cnt_by_sum[cur_tgt_sum];
						cur_tgt_sum--;
						cur_ans += cur_cnt_gt;
						cur_ans -= cur_cnt_lt;
						cur_cnt_lt -= cnt_by_sum[cur_tgt_sum];
					}
					while (cur_tgt_sum < s) {
						cur_cnt_lt += cnt_by_sum[cur_tgt_sum];
						cur_tgt_sum++;
						cur_ans += cur_cnt_lt;
						cur_ans -= cur_cnt_gt;
						cur_cnt_gt -= cnt_by_sum[cur_tgt_sum];
					}
					assert(cur_ans % 2 == 0);
					tot_ans += cur_ans / 2;
				}
			}
		}
	}

	cout << tot_ans << '\n';

	return 0;
}
