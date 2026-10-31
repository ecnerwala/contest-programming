#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		int num_win = 1;
		for (int n = 2; n <= N; n++) {
			if (n == 2 || S[n-2] == S[n-3]) {
				// no change
			} else {
				num_win = n-1;
			}
			cout << num_win << " \n"[n==N];
		}
	}

	return 0;
}
