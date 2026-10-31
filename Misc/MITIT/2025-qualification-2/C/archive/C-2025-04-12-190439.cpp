#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {

		int N; cin >> N;
		std::string S; S.reserve(N); cin >> S;
		assert(int(S.size()) == N);

		auto colors = "rgby";
		auto to_int = [&](char c) -> int {
			if (c == 'r') return 0;
			else if (c == 'g') return 1;
			else if (c == 'b') return 2;
			else if (c == 'y') return 3;
			else assert(false);
		};

		// we sweep from the left to the right, each window we need to break by either moving it right, or providing a bad tile
		std::string ans; ans.reserve(N);
		std::array<std::vector<int>, 4> v;
		for (int z = 0; z < 4; z++) v[z].reserve(5);
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 4; z++) v[z].clear();
			for (int delta = -2; delta <= 2; delta++) {
				if (0 <= i + delta && i + delta < N) {
					v[to_int(S[i+delta])].push_back(i+delta);
				}
			}
			int picked = -1;
			for (int z = 0; z < 4; z++) {
				if (v[z].empty()) {
					picked = z;
					break;
				}
			}
			if (picked == -1) {
				// otherwise, all 4 have at least 1
				assert(i+1 < N);
				if (v[to_int(S[i+1])][0] == i+1) {
					// we're the first, so just use this to push forwards
					picked = to_int(S[i+1]);
				} else {
					picked = to_int(S[i+2]);
				}
			}
			assert(picked != -1);
			ans += colors[picked];
		}
		cout << ans << '\n';
	}

	return 0;
}
