#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		std::vector<int> A(N);
		int ans = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] == 0 && S[i] == '1') {
				ans++;
			} else if (A[i] == 1 && S[i] == '0') {
				ans++;
			} else {
				// just let them cancel out to 1 or 0
			}
			int j = i | (i+1);
			if (j >= N) continue;
			A[j] += (S[i] != '0');
		}
		cout << ans << '\n';
	}

	return 0;
}
