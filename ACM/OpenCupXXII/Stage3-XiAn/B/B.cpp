#include <bits/stdc++.h>

int64_t ceil(int64_t a, int64_t b) { return (a + b - 1) / b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t Cp, Cm, H0, A1, D1, N; cin >> Cp >> Cm >> H0 >> A1 >> D1 >> N;
		H0 = ceil(H0, Cp);

		int64_t ans = 0;

		for (int64_t turns = 1; turns <= H0; turns++) {
			int64_t D0;
			if (turns == 1) {
				D0 = 0;
			} else {
				int64_t max_attack = (H0-1) / (turns-1);
				assert(max_attack > 0);
				D0 = A1 - max_attack;
			}
			turns = ceil(H0, max<int64_t>(1, A1 - D0));

			if (D0 > N) break;

			// we can survive turns

			// If magic == 0, then we put all points into A0
			{
				int64_t A0 = N - D0;
				ans = max(ans, turns * Cp * max<int64_t>(1, A0 - D1));
			}
			// If attack == 0, then we have
			//   Cm * M * (N-D0-M) + Cp * (turns - M)
			//   (N-D0 - (Cp / Cm) * turns) / 2 == M
			{
				int64_t cnd = (Cm * (N - D0) - Cp) / (2 * Cm);
				if (cnd < 0) cnd = 0;
				for (int z = 0; z < 2; z++, cnd++) {
					int64_t M = min<int64_t>({cnd, turns, N-D0});
					ans = max(ans, Cm * M * (N-D0-M) + Cp * (turns - M));
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
