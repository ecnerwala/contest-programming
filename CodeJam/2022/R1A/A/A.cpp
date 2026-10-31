#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		string S; cin >> S;
		std::vector<std::pair<char, int>> runs; runs.reserve(S.size());
		string res; res.reserve(S.size() * 2);
		char last = '\0';
		int cnt = 0;
		for (int i = 0; i < int(S.size()); i++) {
			if (S[i] != last) {
				if (last < S[i]) {
					cnt *= 2;
				}
				while (cnt--) res += last;
				last = S[i];
				cnt = 0;
			}
			cnt++;
		}
		while (cnt--) res += last;

		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
