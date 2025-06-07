#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	std::vector<int> starts(N);
	std::iota(starts.begin(), starts.end(), 0);

	while (true) {
		int K = int(starts.size());
		if (K == 1) break;

		cout << '?' << ' ' << K << '\n';
		for (int i = 0; i < K; i++) {
			cout << starts[i] << ' ' << (i+1==K ? N + starts[0] : starts[i+1]) - starts[i] << '\n';
		}
		cout << flush;

		int N_min; cin >> N_min;
		if (N_min == -1) exit(0);
		assert(N_min >= 1);
		assert(N_min <= K);
		if (N_min == K) break;

		std::vector<int> jumps(K, 0);
		for (int i = 0; i < N_min; i++) {
			int v; cin >> v;
			jumps[v] = 1;
		}
		for (int z = 0; z < 3; z++) {
			for (int i = K-1; i >= 0; i--) {
				if (jumps[i]) {
					jumps[i] = std::max(jumps[i], jumps[(i+1)%K]+1);
				}
			}
		}
		int max_jump = *std::max_element(jumps.begin(), jumps.end());
		std::vector<int> nstarts; nstarts.reserve(K);
		for (int i = 0; i < K; i++) {
			if (jumps[i] == max_jump) nstarts.push_back(starts[i]);
		}
		assert(!nstarts.empty());
		assert(2 * int(nstarts.size()) <= int(starts.size()));
		starts = std::move(nstarts);
	}

	cout << '!' << ' ' << int(starts.size()) << '\n';
	for (int i : starts) cout << i << '\n';
	cout << flush;

	return 0;
}
