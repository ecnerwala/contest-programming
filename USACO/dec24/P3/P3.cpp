#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N = 3; N <= 11; N += 2) {
		int best = N/2;
		std::vector<int> P(N);
		std::iota(P.begin(), P.end(), -N/2);
		do {
			int V = N+1;
			for (int i = 0; i+1 < N; i++) {
				V = std::min(V, std::abs(P[i+1] - P[i]));
			}
			assert(V <= best);
			if (V == best) {
				for (int i = 0; i < N; i++) {
					cerr << (P[i] < 0 ? '-' : P[i] > 0 ? '+' : ' ') << std::abs(P[i]);
					if (i+1 < N) {
						cerr << " *"[std::abs(P[i+1] - P[i]) > best + 1];
					} else {
						cerr << '\n';
					}
				}
			}
		} while (std::next_permutation(P.begin(), P.end()));
	}

	return 0;
}

// you need to alternate bottom and top, and the up needs to be at least the down
