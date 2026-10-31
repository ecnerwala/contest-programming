#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		string S; cin >> S;
		assert(int(S.size()) == N);
		int ans = 0;
		char prv = '\0';
		for (char c : S) {
			if (c == 'F') continue;
			if (prv == '\0') prv = c;
			if (prv != c) {
				ans++;
				prv = c;
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
