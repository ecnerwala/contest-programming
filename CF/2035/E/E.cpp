#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t X, Y, Z, K; cin >> X >> Y >> Z >> K;
		int64_t ans = X + Y * Z;
		// min number of coins
		for (int64_t i = 1; i*i <= Z; i++) {
			// if we do i buffs, how many actual hits do we need?
			// as we do the i buffs, we hit 
			int64_t buff_val = (i / K) * (i / K + 1) / 2 * K;
			int64_t cnd = X * i + Y * (i / K);
			if (buff_val >= Z) {
				ans = std::min(ans, cnd);
				break;
			}
			cnd += ((Z - buff_val + i-1) / i) * Y;
			ans = std::min(ans, cnd);
		}
		for (int64_t i = 0; i * i <= Z; i++) {
			// if we want to do i real hits, how many buffs do we need?
			int64_t mi = 0;
			int64_t ma = i ? (Z + i - 1) / i : Z + K;
			while (ma - mi > 1) {
				int64_t md = mi + (ma - mi) / 2;
				int64_t buff_val = (md / K) * (md / K + 1) / 2 * K;
				if (buff_val + i * md >= Z) {
					ma = md;
				} else {
					mi = md;
				}
			}
			int64_t cnd = ma * X + (ma / K + i) * Y;
			ans = std::min(ans, cnd);
		}
		cout << ans << '\n';
	}

	return 0;
}
