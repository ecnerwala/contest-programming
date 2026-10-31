#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;

		int cur_val = S[0] - '0';
		for (int i = 1; i < N; i++) {
			if (S[i] == '0') {
				cout << '+';
			} else if (S[i] == '1') {
				if (cur_val == 0) {
					cout << '+';
					cur_val++;
				} else {
					cout << '-';
					cur_val--;
				}
			} else assert(false);
		}
		cout << '\n';
	}

	return 0;
}
