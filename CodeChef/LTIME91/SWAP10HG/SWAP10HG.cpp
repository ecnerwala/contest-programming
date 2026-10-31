#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S, P; cin >> S >> P;
		bool good = true;
		int c0 = 0;
		int c1 = 0;
		for (int i = 0; i < N; i++) {
			c0 += S[i] - '0';
			c1 += P[i] - '0';
			if (c1 > c0) {
				good = false;
			}
		}
		if (c0 != c1) good = false;
		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
