#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; cin >> N;
		string S, T; cin >> S >> T;

		std::vector<int> locs; locs.reserve(N+1);
		int cur = 0;
		for (int i = 0; i <= N; i++) {
			int nxt = (i == N ? 0 : S[i] ^ T[i]);
			if (nxt != cur) locs.push_back(i);
			cur = nxt;
		}

		if (locs.size() > 4) {
			cout << 0 << '\n';
		} else if (locs.size() == 4) {
			cout << 6 << '\n';
		} else if (locs.size() == 2) {
			cout << 2 * (N-1) << '\n';
		} else {
			cout << int64_t(N) * int64_t(N+1) / 2 << '\n';
		}
	}

	return 0;
}
