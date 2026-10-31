#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	string S, T; cin >> S >> T;

	int64_t ans = 0;

	// number of flips of S which are done before
	int cur_need = S[0] ^ T[0];
	for (int i = 1; i < N; i++) {
		ans += cur_need;
		int nxt_need;
		if (S[i] == T[i]) {
			nxt_need = cur_need;
		} else if (S[i] == '1' && T[i] == '0') {
			nxt_need = (cur_need == 0 ? 1 : cur_need - 1);
		} else {
			nxt_need = cur_need + 1;
		}
		cur_need = nxt_need;
	}
	if (cur_need == 0) {
		cout << ans << '\n';
	} else {
		cout << -1 << '\n';
	}

	return 0;
}
