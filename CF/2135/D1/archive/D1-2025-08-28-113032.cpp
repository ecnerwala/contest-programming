#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int WMAX = int(1e5);
	const int G = int(300);
	const int Q1 = 1e4;
	int v_prev = -1;
	for (int i = 1; i <= WMAX; i++) {
		int v = (i/G) ? (Q1 + (i/G) - 1) / (i/G) : 0;
		if (v != v_prev) {
			cerr << i << ' ' << v << '\n';
			v_prev = v;
		}
	}
	int T; cin >> T;
	while (T--) {
		// we can just do WMAX 1's, and then we know the answer if it's <= sqrt()
		cout << '?' << ' ' << Q1;
		for (int z = 0; z < Q1; z++) cout << ' ' << G;
		cout << '\n' << flush;
		int resp1; cin >> resp1;
		if (resp1 == -1) exit(0);
		if (resp1 == 0) {
		} else {
			assert(resp1 > 1);
			// (w/G) * resp1 >= Q1 > (w/G) * (resp1 - 1)
			int wG_min = (Q1 + resp1 - 1) / resp1;
			int wG_max = (Q1 - 1) / (resp1 - 1);
			assert(wG_max >= wG_min);
			int w_max = std::min(WMAX, wG_max * G + (G-1));
			int w_min = wG_min * G;
			assert(w_max >= w_min);
			assert(w_max < 2 * w_min);
			assert(w_min != w_max);
			cout << '?' << ' ' << 2 * (w_max - w_min);
			for (int i = w_min + 1; i <= w_max; i++) {
				cout << ' ' << w_min << ' ' << i - w_min;
			}
			cout << '\n' << flush;
			int resp2; cin >> resp2;
			// w_max is (w_max - w_min), and w_min is (w_max - w_min) * 2
			int ans = (w_max - w_min) + w_max - resp2;
			cout << '!' << ans << '\n' << flush;
		}
	}

	return 0;
}
