#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, M; cin >> T >> M;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<std::array<int, 2>, 2>> corners(N);
		for (auto& c : corners) for (auto& v : c) for (auto& x : v) { cin >> x; x--; }
		if (M == 1) {
		} else if (M == 2) {
			std::string ans(N, '?');
			std::vector<int> bit(2*N, 2*N);
			// M = 2: just go backwards
			for (int i = N-1; i >= 0; i--) {
				// check if box i can be removed
				{
					auto [qx, qy] = corners[i][1];
					// check if something < qx is < qy
					bool good = true;
					for (int b = qx; b > 0; b &= (b-1)) {
						if (bit[b-1] < qy) good = false;
					}
					ans[i] = good ? '1' : '0';
				}
				{
					auto [qx, qy] = corners[i][0];
					for (int b = qx; b < 2*N; b |= b+1) {
						bit[b] = std::min(bit[b], qy);
					}
				}
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
