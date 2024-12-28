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
			if (r - l == 1) seen[l]++;
		}
		std::vector<int> pref(2*N+1);
		for (int i = 0; i < int(seen.size()); i++) {
			pref[i+1] = pref[i] + bool(seen[i]);
		}
		for (int i = 0; i < N; i++) {
			auto [l, r] = bounds[i];
			if ((r - l == 1) ? (seen[l] >= 2) : (pref[r] - pref[l] == r - l)) {
				cout << '0';
			} else {
				cout << '1';
			}
		}
		cout << '\n';
	}

	return 0;
}
