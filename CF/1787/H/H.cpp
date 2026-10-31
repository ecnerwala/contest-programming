#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int64_t base_score = 0;
		std::vector<std::pair<int64_t, int64_t>> coeffs(N);
		for (int i = 0; i < N; i++) {
			int64_t b, k, a; cin >> b >> k >> a;
			base_score += a;
			b -= a;
			b -= k;
			// might as well
			if (b < 0) b = 0;
			coeffs[i] = {k, b};
		}
		std::sort(coeffs.begin(), coeffs.end());
		// sorted list of deltas
		std::vector<int> deltas;
		deltas.push_back(0);
		for (auto [k, b] : coeffs) {
			if (b <= deltas[0]) {
				// there's nothing to actually do
				continue;
			}

			int bad_t = 0;
			while (bad_t < int(deltas.size()) && b - k * bad_t > deltas[bad_t]) bad_t++;
			int good_t = bad_t - 1;
			base_score += b - deltas[0];
		}

		cout << base_score << '\n';
	}

	return 0;
}
