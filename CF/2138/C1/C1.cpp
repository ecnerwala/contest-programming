#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;

		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }
		std::vector<int> depth(N);
		depth[0] = 0;
		for (int i = 1; i < N; i++) depth[i] = depth[P[i]] + 1;
		std::vector<int> sz(N, 1);
		for (int i = N-1; i >= 1; i--) sz[P[i]] += sz[i];

		int leaf_depth = depth[N-1];
		for (int i = 0; i < N; i++) if (sz[i] == 1) leaf_depth = std::min(leaf_depth, depth[i]);

		std::vector<int> freq(N);
		for (int i = 0; i < N; i++) freq[depth[i]]++;

		auto is_good = [&](int l) -> bool {
			std::vector<int> freq_freq(N+1);
			for (int i = 0; i < l; i++) {
				freq_freq[freq[i]] += 1;
			}
			for (int i = l; i < N; i++) {
				// everything after leaf depth is unconstrained, so we can just set it as 1
				freq_freq[1] += freq[i];
			}

			// Now, we just knapsack everything together
			std::vector<int> pos(N+1);
			pos[0] = 1;
			std::vector<int> npos(N+1);
			for (int i = 1; i <= N; i++) {
				if (!freq_freq[i]) continue;
				for (int k = 0; k < i; k++) {
					int ways = 0;
					for (int v = k; v <= N; v++) {
						ways += pos[v];
						npos[v] = bool(ways);
						if (v >= freq_freq[i] * i) ways -= pos[v - freq_freq[i] * i];
					}
				}
				std::swap(pos, npos);
			}
			return bool(pos[K]);
		};
		int mi = 0, ma = leaf_depth + 2;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (is_good(md)) mi = md;
			else ma = md;
		}
		cout << mi << '\n';
	}

	return 0;
}
