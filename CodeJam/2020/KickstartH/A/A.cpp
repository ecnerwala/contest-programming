#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t N, K, S; cin >> N >> K >> S;

		int64_t ans = min(N + 1, (K-S) + (N-S) + 1) + (K-1);
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
