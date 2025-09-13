#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int C; int64_t R; cin >> C >> R;
		assert(1 <= C && C <= 7);
		std::string P, Q; cin >> P >> Q;
		assert(int(P.size()) == C);
		assert(int(Q.size()) == C);

		std::vector<int> msks; msks.reserve(40);
		for (int m = 0; m < (1 << C); m++) {
			bool good = true;
			for (int i = 0; i+1 < C; i++) {
				if (((m >> i) & 3) == 3) {
					good = false;
					break;
				}
			}
			if (!good) continue;
			msks.push_back(m);
		}
		const int64_t INF = R * C + 1;
		int64_t ans = INF;

		int M = int(msks.size());
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				for (int k = 0; k < M; k++) {
					// we need to check all entries of j
					bool is_valid = [&]() -> bool {
						int mi = msks[i];
						int mj = msks[j];
						int mk = msks[k];
						for (int c = 0; c < C; c++) {
							std::array<bool, 2> has{false, false};
							has[(mi >> c) & 1] = true;
							has[(mk >> c) & 1] = true;
							if (c-1 >= 0) has[(mj >> (c-1)) & 1] = true;
							if (c+1 < C) has[(mj >> (c+1)) & 1] = true;
							if ((mj >> c) & 1) {
								return !(has[0] && has[1]);
							} else {
								return has[0] && has[1];
							}
						}
						return true;
					}();
				}
			}
		}
		// TODO: Check if all 1's works
	}

	return 0;
}
