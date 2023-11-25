#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		assert(int(S.size()) == 2*N);
		if (S[0] != S[2*N-1]) {
			cout << -1 << '\n';
			continue;
		}
		{
			int tot = 0;
			for (char c : S) tot ^= (c - '0');
			if (tot == 1) {
				cout << -1 << '\n';
				continue;
			}
		}
		if (S[0] == '1') {
			cout << 3 << '\n';
			for (int i = 0; i < 2*N; i++) {
				cout << (i&1 ? ')' : '(');
			}
			cout << '\n';
			for (int i = 0; i < 2*N; i++) {
				S[i] ^= 1;
			}
		} else {
			cout << 2 << '\n';
		}
		assert(S[0] == '0');
		int cnt_0 = 0;
		for (int i = 0; i < 2*N; i++) {
			if (S[i] == '0') cnt_0++;
		}
		assert(cnt_0 % 2 == 0);
		for (int z = 0; z < 2; z++) {
			int cur_0 = 0;
			int cur_1 = 0;
			for (int i = 0; i < 2*N; i++) {
				if (S[i] == '0') {
					if (cur_0 < cnt_0 / 2) {
						cout << '(';
					} else {
						cout << ')';
					}
					cur_0++;
				} else if (S[i] == '1') {
					if ((z ^ cur_1) & 1) {
						cout << ')';
					} else {
						cout << '(';
					}
					cur_1++;
				} else assert(false);
			}
			cout << '\n';
		}
	}

	return 0;
}
