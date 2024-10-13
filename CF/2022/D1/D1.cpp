#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		auto get_parity = [&](int a, int b) -> int {
			assert(a != b);
			cout << '?' << ' ' << a+1 << ' ' << b+1 << '\n' << flush;
			int resp; cin >> resp;
			assert(resp == 0 || resp == 1);
			return !resp;
		};
		std::vector<int> parity(N);
		for (int i = 0; i < N-1; i++) {
			parity[i+1] = parity[i] ^ get_parity(i, i+1);
		}
		int mi = 0, ma = N-1;
		while (ma - mi + 1 > 2) {
			int md = (mi + ma) / 2;
			assert(mi < md && md < ma);
			// mi .. md
			if (get_parity(md, mi) ^ parity[mi] ^ parity[md]) {
				ma = md;
			} else {
				mi = md+1;
			}
		}
		assert(ma >= mi);
		int res;
		if (ma - mi + 1 == 2) {
			// one more query
			if (mi == 0) {
				assert(ma == 1);
				assert(N >= 3);
				if (get_parity(ma+1, ma) ^ parity[ma] ^ parity[ma+1]) res = ma;
				else res = mi;
			} else {
				if (get_parity(mi, mi-1) ^ parity[mi-1] ^ parity[mi]) res = mi;
				else res = ma;
			}
		} else {
			res = mi;
		}
		cout << '!' << ' ' << res+1 << '\n' << flush;
	}

	return 0;
}
