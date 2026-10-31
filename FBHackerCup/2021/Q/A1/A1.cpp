#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		auto is_vowel = [&](char c) -> bool { return c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'; };
		string S; cin >> S;
		int slow_ans = int(1e9);
		for (char c = 'A'; c <= 'Z'; c++) {
			int cnd = 0;
			for (char v : S) {
				if (v == c) continue;
				if (is_vowel(v) != is_vowel(c)) cnd ++;
				else cnd += 2;
			}
			slow_ans = min(slow_ans, cnd);
		}

		std::array<int, 26> cnt; cnt.fill(0);
		std::array<int, 2> vcnt; vcnt.fill(0);
		for (char c : S) cnt[c-'A']++, vcnt[is_vowel(c)]++;
		int ans = 2 * int(S.size()) + 1;
		for (char c = 'A'; c <= 'Z'; c++) {
			ans = min(ans, int(S.size()) + vcnt[is_vowel(c)] - 2*cnt[c-'A']);
		}

		assert(ans == slow_ans);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
