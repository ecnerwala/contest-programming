#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		cout << [&]() -> int {
			for (int i = 0; i+1 < N; i++) {
				if (S[i] == 'R' && S[i+1] == 'L') {
					return 0;
				}
			}
			for (int i = 0; i+1 < N; i++) {
				if (S[i] == 'L' && S[i+1] == 'R') {
					return i+1;
				}
			}
			return -1;
		}() << '\n';
	}

	return 0;
}
