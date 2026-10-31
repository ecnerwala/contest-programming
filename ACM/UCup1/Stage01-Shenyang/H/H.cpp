#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_STRINGS; cin >> N_STRINGS;

	const int MAXV = int(1e6) + 20;
	std::vector<std::array<int, 26>> ch; ch.reserve(MAXV);
	std::vector<int> fail; fail.reserve(MAXV);
	std::vector<int> num_fail; num_fail.reserve(MAXV);
	std::vector<int> len; len.reserve(MAXV);

	ch.emplace_back();
	ch.back().fill(-1);
	len.push_back(-1);
	fail.push_back(-1);
	num_fail.push_back(0);

	ch.emplace_back();
	ch.back().fill(-1);
	len.push_back(0);
	fail.push_back(0);
	num_fail.push_back(0);

	int64_t ans = 0;
	for (int z = 0; z < N_STRINGS; z++) {
		string S; cin >> S;

		// empty palindrome
		int cur_suff = 1;
		for (int i = 0; i < int(S.size()); i++) {
			while (!(i - len[cur_suff] - 1 >= 0 && S[i] == S[i - len[cur_suff] - 1])) {
				cur_suff = fail[cur_suff];
			}
			int v = S[i] - 'a';
			if (ch[cur_suff][v] == -1) {
				// new palindrome!
				int idx = int(ch.size());
				ch[cur_suff][v] = idx;
				ch.emplace_back();
				ch.back().fill(-1);
				len.push_back(len[cur_suff] + 2);
				int f = fail[cur_suff];
				if (f == -1) {
					// empty palindrome
					f = 1;
				} else {
					while (!(i - len[f] - 1 >= 0 && S[i] == S[i - len[f] - 1])) {
						f = fail[f];
					}
					f = ch[f][v];
					assert(f != -1);
				}
				fail.push_back(f);
				if (len[idx] % (len[idx] - len[f]) == 0) {
					ans += 2 * (len[idx] / (len[idx] - len[f]) - 1);
				}
				ans ++;
				num_fail.push_back(num_fail[f] + 1);

				//cerr << idx << " = " << S.substr(i - len[idx] + 1, len[idx]) << " = " << S[i] << "+" << cur_suff << "+" << S[i] << ", " << "fail = " << f << '\n';
			}
			cur_suff = ch[cur_suff][v];
		}
	}
	cout << ans << '\n';

	return 0;
}
