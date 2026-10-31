#include <bits/stdc++.h>

struct nim_prod_t {
	uint64_t bit_prod[64][64]{};
	constexpr nim_prod_t() {
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				if ((i & j) == 0) {
					bit_prod[i][j] = uint64_t(1) << (i|j);
				} else {
					int a = (i&j) & -(i&j);
					bit_prod[i][j] = bit_prod[i ^ a][j] ^ bit_prod[(i ^ a) | (a-1)][(j ^ a) | (i & (a-1))];
				}
			}
		}
	}
	constexpr uint64_t operator () (uint64_t x, uint64_t y) const {
		uint64_t res = 0;
		for (int i = 0; i < 64 && (x >> i); i++)
			if ((x >> i) & 1)
				for (int j = 0; j < 64 && (y >> j); j++)
					if ((y >> j) & 1)
						res ^= bit_prod[i][j];
		return res;
	}
};
constexpr nim_prod_t nimProd;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		uint64_t a, b; cin >> a >> b;
		cout << nimProd(a, b) << '\n';
	}

	return 0;
}
