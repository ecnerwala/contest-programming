#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N = 1; N <= 5; N++) {
		std::vector<int> P(N);
		std::iota(P.begin(), P.end(), 0);
		do {
			int V = N+1;
			for (int i = 0; i+1 < N; i++) {
				V = std::min(V, std::abs(P[i+1] - P[i]));
			}
			assert(V >= (N-1)/2);
			if (V == (N-1)/2) {
				for (auto a : P) { cerr << a << ' '; } cerr << '\n';
			}
		} while (std::next_permutation(P.begin(), P.end()));
	}

	return 0;
}
