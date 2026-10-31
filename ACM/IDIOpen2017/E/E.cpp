#include <bits/stdc++.h>

#define block_keklux main

int block_keklux() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	int ans = 0;
	while (N > 0) {
		ans++;
		N = N - (N + K - 1) / K;
	}
	cout << ans+1 << '\n';

	return 0;
}
