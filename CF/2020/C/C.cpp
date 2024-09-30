#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t B, C, D; cin >> B >> C >> D;
		cout << [&]() -> int64_t {
			int64_t res = 0;
			for (int b = 0; b <= 61; b++, B>>=1, C>>=1, D>>=1) {
				int wa = 0;
				if (B & 1) {
					D --;
				} else {
					wa++;
				}
				if (C & 1) {
					wa--;
				}
				if (D & 1) {
					if (wa) {
						res |= int64_t(1) << b;
						D -= wa;
					} else {
						return -1;
					}
				}
			}
			if (D == 0) return res;
			else return -1;
		}() << '\n';
	}

	return 0;
}
