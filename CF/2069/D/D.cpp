#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		{
			int lo = 0, hi = int(S.size());
			assert(int(S.size()) % 2 == 0);
			while (hi - lo >= 2 && S[hi-1] == S[lo]) {
				lo++, hi--;
			}
			S = S.substr(lo, hi - lo);
		}
		std::array<int, 26> freq; freq.fill(0);
		for (char c : S) {
			freq[c - 'a'] ++;
		}
		for (auto& v : freq) {
			assert(!(v & 1));
			v /= 2;
		}
		int ans = int(S.size());
		for (int z = 0; z < 2; z++, std::reverse(S.begin(), S.end())) {
			auto cur_freq = freq;
			int cnd = int(S.size());
			for (char c : S) {
				if (cur_freq[c - 'a'] == 0) {
					break;
				}
				cnd--;
				cur_freq[c - 'a']--;
			}
			assert(2 * cnd <= int(S.size()));
			if (2 * cnd == int(S.size())) {
				// we got all the way to the midpoint, so just track how long the palindrome is
				for (; cnd < int(S.size()); cnd++) {
					if (S[cnd] == S[int(S.size()) - cnd - 1]) {
						// good;
					} else {
						break;
					}
				}
			}
			ans = std::min(ans, cnd);
		}
		cout << ans << '\n';
	}

	return 0;
}
