#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		assert(int(S.size()) == N);
		bool is_A = false;
		if (S.back() == 'A') {
			is_A = true;
		} else {
			for (int i = 0; i+1 < N; i++) {
				if (S[i] == 'B' && S[i+1] == 'A') is_A = true;
			}
		}
		cout << (is_A ? 'A' : 'B') << '\n';
	}

	return 0;
}
