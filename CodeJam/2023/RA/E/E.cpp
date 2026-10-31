#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		string S; cin >> S;
		int ans = 0;
		if (S.size() >= 2 && S.front() == S.back()) {
			int a1 = 1, a2 = 1;
			for (int i = 2; i < int(S.size()); i++) {
				if (S[i] == S[i-1]) {
					a1++;
					i++;
				}
			}
			for (int i = 1; i < int(S.size())-1; i++) {
				if (S[i] == S[i-1]) {
					a2++;
					i++;
				}
			}
			ans = std::min(a1, a2);
		} else {
			for (int i = 1; i < int(S.size()); i++) {
				if (S[i] == S[i-1]) {
					ans++;
					i++;
				}
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
