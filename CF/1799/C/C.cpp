#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int cases; cin >> cases;
	while (cases--) {
		string S; cin >> S;
		int N = int(S.size());
		std::array<int, 26> cnt;
		cnt.fill(0);
		for (char c : S) cnt[c - 'a']++;

		string T(N, '?');
		int lo = 0, hi = N-1;
		int first_mismatch = -1;
		for (int i = 0; i < 26; i++) {
			if (cnt[i] == 0) continue;
			while (cnt[i] >= 2 && (first_mismatch == -1 || hi - lo + 1 == cnt[i] + 1)) {
				T[lo++] = char('a' + i);
				T[hi--] = char('a' + i);
				cnt[i] -= 2;
			}
			if (first_mismatch != -1) {
				// Whoops, benign bug here: we don't check cnt >0
				// if cnt == 0, we must have had hi - lo + 1 == cnt[i] + 1, which means that T[hi--] overwrites T[lo++], so the algo works
				T[lo++] = char('a' + i);
				T[hi--] = char('a' + first_mismatch);
				cnt[i]--;
				cnt[first_mismatch]--;
				break;
			}
			if (cnt[i] >= 1) {
				first_mismatch = i;
			}
		}
		for (int i = 0; i < 26; i++) {
			while (cnt[i] >= 1) {
				T[lo++] = char('a' + i);
				cnt[i]--;
			}
		}
		cout << T << '\n';
	}

	return 0;
}
