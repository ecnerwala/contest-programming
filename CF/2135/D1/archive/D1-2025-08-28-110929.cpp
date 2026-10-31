#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int WMAX = int(1e5);
	int v_prev = -1;
	for (int i = 1; i <= WMAX; i++) {
		int v = (WMAX + i - 1) / i;
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
		// k * resp1 >= WMAX > k * (resp1 - 1)
	}

	return 0;
}
