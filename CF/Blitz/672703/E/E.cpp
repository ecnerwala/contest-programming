#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(nullptr);
	int T; std::cin >> T;
	while (T--) {
		std::string S; std::cin >> S;
		// if there's no E, we should change the first character
		// if there is an E, we should change something negative
		int i = 0; while (i < int(S.size()) && S[i] == 'E') i++;
		if (i < int(S.size())) {
			if (*std::max_element(S.begin() + i, S.end()) != 'E') S[i] = 'E';
			else {
				int j = int(S.size())-1;
				while (S[j] != 'E') j--;
				int best = i;
				for (int k = i; k < j; k++) {
					if (S[k] == 'E') continue;
					if (S[k] > S[best]) best = k;
				}
				// max that's not e
				S[best] = 'E';
			}
		}
		std::reverse(S.begin(), S.end());
		char mx = 0;
		int64_t ans = 0;
		for (char c : S) {
			int w;
			if (c < mx) { w = -1; } else { w = 1, mx = c; }
			if (c == 'A') w *= 1;
			else if (c == 'B') w *= 10;
			else if (c == 'C') w *= 100;
			else if (c == 'D') w *= 1000;
			else if (c == 'E') w *= 10000;
			else assert(false);
			ans += w;
		}
		std::cout << ans << '\n';
	}
}
