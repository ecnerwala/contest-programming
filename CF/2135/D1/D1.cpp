#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int WMAX = int(1e5);
	const int G = int(100);
	int v_prev = -1;
	for (int i = 1; i <= WMAX; i++) {
		int v = (i/G) ? (WMAX + (i/G) - 1) / (i/G) : 0;
		if (v != v_prev) {
			cerr << i << ' ' << v << '\n';
			v_prev = v;
		}
	}
	int T; cin >> T;
	while (T--) {
		// we can just do WMAX 1's, and then we know the answer if it's <= sqrt()
		cout << '?' << ' ' << WMAX;
		for (int z = 0; z < WMAX; z++) cout << ' ' << 1;
		cout << '\n' << flush;
		int resp1; cin >> resp1;
		if (resp1 == -1) exit(0);
		// w * resp1 >= WMAX > w * (resp1 - 1)
		int w_min = (WMAX + resp1 - 1) / resp1;
		int w_max = (WMAX - 1) / (resp1 - 1);
		assert(w_max >= w_min);
		if (w_max == w_min) {
			cout << '!' << ' ' << w_min << '\n' << flush;
			continue;
		}
	}

	return 0;
}
