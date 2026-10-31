#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		string S; S.reserve(M);
		array<int, 2> c{0,0};
		for (int i = 0; i < N; i++) {
			cin >> S;
			for (int j = 0; j < M; j++) {
				c[((S[j] == '*') ^ i ^ j) & 1]++;
			}
		}
		cout << (N&M&1 ? c[0] : min(c[0], c[1])) << '\n';
	}

	return 0;
}
