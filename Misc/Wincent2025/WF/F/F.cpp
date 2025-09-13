#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int C, R; cin >> C >> R;
		assert(1 <= C && C <= 7);
		std::string P, Q; cin >> P >> Q;
		assert(int(P.size()) == C);
		assert(int(Q.size()) == C);

		std::vector<int> msks; msks.reserve(40);
		for (int m = 0; m < (1 << C); m++) {
			bool good = true;
			for (int i = 0; i+1 < C; i++) {
				if ((m >> i) & 3) {
					good = false;
					break;
				}
			}
			if (!good) continue;
			msks.push_back(m);
		}
		cerr << C << ' ' << int(msks.size()) << '\n';
	}

	return 0;
}
