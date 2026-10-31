#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K, P; cin >> N >> K >> P;
		std::vector<int> S_freq(N);
		for (int i = 0; i < N; i++) {
			int s; cin >> s; S_freq[s]++;
		}

		// Solve the N, K, P system
		// P[win] is equal to freq / N, by some symmetry arguments
		// so we only care about E[(time if win else 0)]

	}

	return 0;
}
