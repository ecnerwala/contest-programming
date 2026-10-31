#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		string S; cin >> S;
		int Q; cin >> Q;
		std::vector<bool> f(N);
		int ans = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') {
				f[i] = true;
				ans++;
				for (int j = i; j < N; j += i+1) {
					S[j] ^= 1;
				}
			}
		}
		for (int q = 0; q < Q; q++) {
			int b; cin >> b; b--;
			if (f[b]) {
				ans--;
				f[b] = false;
			} else {
				ans++;
				f[b] = true;
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
