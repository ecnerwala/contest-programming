#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		int64_t ans = 0;
		int cnt = 0;
		for (int i = N-1; i >= 0; i--) {
			if (S[i] == '1' && cnt > 0) {
				ans += i+1;
				cnt--;
			} else {
				cnt++;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
