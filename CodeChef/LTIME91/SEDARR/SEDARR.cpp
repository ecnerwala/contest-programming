#include <bits/stdc++.h>

#define i main

int i() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		int sum = 0;
		while (N--) { int v; cin >> v; sum += v; }
		cout << bool(sum % K) << '\n';
	}

	return 0;
}
