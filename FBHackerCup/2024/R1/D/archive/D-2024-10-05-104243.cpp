#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		std::string S; int K; cin >> S >> K;
		int cap = int(S.size());
		for (int i = int(S.size())-1; i >= 0; i--) {
			assert(i < cap);
			if (S[i] == '0') {
				assert(i > 0);
				if (S[i-1] == '?') {
					S[i-1] = char('1' + (K % 2));
					K /= 2;
				} else {
					assert(S[i-1] == '1' || S[i-1] == '2');
				}
				cap = i-1;
				i--;
				continue;
			}
			if (S[i] == '?') {
				if (i+1 == cap) {
					if (i > 0 && S[i-1] == '?') {
						// these two form a pair, anything from 11-19 or 21-26
						int v = 11 + (K % 15);
						K /= 15;
						if (v >= 20) v++;
						S[i] = char('0' + v % 10);
						S[i-1] = char('0' + v / 10);
						i--;
						continue;
					} else if (i > 0 && S[i-1] == '2') {
						// must be 1-6
						S[i] = char('1' + (K % 6));
						K /= 6;
					} else {
						// can be 1-9, since that guy is either 1, 0, or 3-9, any of which have the same merging right behavior
						S[i] = char('1' + (K % 9));
						K /= 9;
					}
				} else if (S[i+1] == '?' || ('1' <= S[i+1] && S[i+1] <= '6')) {
					assert(i + 2 < cap);
					S[i] = char('1' + (K % 2));
					K /= 2;
				} else if ('7' <= S[i+1] && S[i+1] <= '9') {
					// 1 only
					S[i] = '1';
				} else if (S[i+1] == '0') {
					assert(false);
				} else assert(false);
			}
		}
		assert(K == 0);

		for (char c : S) assert('0' <= c && c <= '9');

		cout << "Case #" << case_num << ": " << S << '\n';
	}

	return 0;
}
