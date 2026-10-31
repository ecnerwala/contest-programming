#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		std::vector<std::array<int, 2>> V; V.reserve(N);
		if ([&]() -> bool {
			assert(int(S.size()) == N);
			int hold = 1;
			int nxt = 2;
			for (char c : S) {
				int tgt;
				if (c == 'O') {
					tgt = 1;
				} else if (c == 'E') {
					tgt = 0;
				} else assert(false);

				if (((hold + nxt) & 1) == tgt) {
					if (nxt > 2 * N) return false;
					V.push_back({hold, nxt});
					hold = nxt+1;
					nxt += 2;
				} else {
					if (nxt+1 > 2 * N) return false;
					V.push_back({hold, nxt+1});
					hold = nxt;
					nxt += 2;
				}
			}
			return true;
		}()) {
			cout << "YES" << '\n';
			for (auto [x, y] : V) cout << x << ' ' << y << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
