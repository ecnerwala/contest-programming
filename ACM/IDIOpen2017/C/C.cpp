#include <bits/extc++.h>

#define block_keklux main

int block_keklux() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	int K; cin >> K;

	// we're always pointing to the next guy to start counting from
	int idx = 0;
	for (int sz = 1; sz < T;) {
		sz++;
		idx += K;
		idx %= sz;
	}
	cout << idx << '\n';

	return 0;
}
