#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		int N = int(S.size());
		int ans = 0;
		for (int i = 0, j = 0; i < N; i = j) {
			if (S[i] == 'I') { j = i+1; continue; }
			int cntM = 0;
			int cntT = 0;
			while (j < N && ((j-i)%2 == 1) == (S[j] == 'I')) {
				if (S[j] == 'M') cntM++;
				if (S[j] == 'T') cntT++;
				j++;
			}
			ans += std::min(cntM, cntT/2);
		}
		cout << ans << '\n';
	}

	return 0;
}
