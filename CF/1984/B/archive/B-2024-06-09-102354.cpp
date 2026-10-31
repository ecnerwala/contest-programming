#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t X; cin >> X;

		int64_t p10 = 1;
		for (int l = 1; l <= 18; l++) {
			p10 *= 10;
			int64_t base = (p10 - 1) / 9 * 10;
			if (base > X) {
				break;
			}
			int64_t extra = (X - base);
			// every digit of extra must be <= 8
			bool good = true;
			for (int i = 0; i < l; i++) {
				if (extra % 10 > 8) {
					good = false;
					break;
				}
				extra /= 10;
			}
			if (extra > 0) good = false;
			if (good) {
				cout << "YES" << '\n';
				goto done;
			}
		}
		cout << "NO" << '\n';
done:;
	}

	return 0;
}
