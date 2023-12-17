#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) { cin >> a; }
	std::sort(A.begin(), A.end());

	// Loop it, then need things to be smaller as often as possible, must maximize this count
	// Break it at the smallest gap possible
	// Match things to the smallest ones
	std::vector<std::pair<int, int>> freq; freq.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!freq.empty() && A[i] == freq.back().first) {
			freq.back().second++;
		} else {
			freq.push_back({A[i], 1});
		}
	}

	int num_unmatched = 0;
	for (auto [a, cnt] : freq) {
		cerr << a << ' ' << cnt << '\n';
		num_unmatched = std::max(num_unmatched, cnt);
	}

	int best_gap = K;
	{
		// If we break at an unmatched pair, we should do the first and last max
		int first_max = K;
		int last_max = -1;
		for (auto [a, cnt] : freq) {
			if (cnt == num_unmatched) {
				if (first_max == K) first_max = a;
				last_max = a;
			}
		}
		assert(last_max != -1);
		assert(first_max <= last_max);
		best_gap = std::min(best_gap, last_max - first_max);
		if (first_max != freq.front().first) {
			best_gap = std::min(best_gap, freq.front().first + K - first_max);
		}
		if (last_max != freq.back().first) {
			best_gap = std::min(best_gap, last_max + K - freq.back().first);
		}
	}

	assert(0 <= best_gap && best_gap < K);

	cout << int64_t(K) * (N - num_unmatched) - best_gap << '\n';

	return 0;
}
