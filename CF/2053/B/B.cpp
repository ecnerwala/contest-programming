#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> bounds(N);
		std::vector<int> seen(2*N);
		for (int i = 0; i < N; i++) {
			int l, r; cin >> l >> r; l--;
			bounds[i] = {l, r};
			if (r - l == 1) seen[r]++;
		}
		for (int i = 0; i < 2*N; i++) {
			seen[i+1] += seen[i];
		}
		for (int i = 0; i < 2*N; i++) {
			auto [l, r] = bounds[i];
			if (seen[r] - seen[l] - (r - l == 1)) {
				cout << '0';
			} else {
				cout << '1';
			}
		}
		cout << '\n';
	}

	return 0;
}
