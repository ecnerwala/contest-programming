#include <bits/stdc++.h>

#define crows_are_cool_caw_caw main

int crows_are_cool_caw_caw() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// Note: r_i < p_i
	// or s_i / sqrt(2) < p_i
	//
	// 2r_i^2 < 2p_i^2
	// s_i^2 < 2p_i^2

	int N, M, K; cin >> N >> M >> K;
	vector<int> plots(N);
	for (auto& a : plots) { cin >> a; a = 2 * a * a; }
	vector<int> houses(M+K);
	for (int i = 0; i < M; i++) {
		int a; cin >> a;
		houses[i] = 2 * a * a;
	}
	for (int i = M; i < M+K; i++) {
		int a; cin >> a;
		houses[i] = a * a;
	}

	sort(plots.begin(), plots.end());
	reverse(plots.begin(), plots.end());
	sort(houses.begin(), houses.end());

	int ans = 0;
	for (int p : plots) {
		while (!houses.empty() && houses.back() >= p) houses.pop_back();
		if (!houses.empty()) {
			ans++;
			houses.pop_back();
		}
	}

	cout << ans << '\n';

	return 0;
}
