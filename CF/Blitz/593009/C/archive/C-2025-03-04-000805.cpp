#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int H, W; cin >> H >> W;
	std::vector<int> R(H);
	for (auto& r : R) cin >> r;
	std::vector<int> C(W);
	for (auto& c : C) cin >> c;

	std::vector<std::vector<int>> G(H, std::vector<int>(W));
	int64_t ans = 1;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			bool must_off = (i == C[j]) || (j == R[i]);
			bool must_on = (i < C[j]) || (j < R[i]);
			ans *= (!must_on) + (!must_off);
			ans %= int(1e9)+7;
		}
	}
	cout << ans << '\n';

	return 0;
}
