#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int64_t S; cin >> S;
		S = int64_t(N) * int64_t(N+1) / 2 - S;
		S = min(S, 2*(N+1) - S);
		cout << (S-1) / 2 << '\n';
	}

	return 0;
}
