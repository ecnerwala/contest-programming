#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; }
	if (N <= 10) {
		std::pair<double, std::array<int, 2>> best{0, {0, -1}};
		for (int i = 0; i < N; i++) {
			int v = 1;
			for (int j = i; j < N; j++) {
				v *= P[j];
				best = std::max(best, {double(v) / (j - i + 1), {i, j}});
			}
		}
		cout << best.second[0]+1 << ' ' << best.second[1]+1 << '\n';
	} else {
		int lo = 0, hi = N-1;
		if (P[lo] == 1) lo++;
		if (P[hi] == 1) hi--;
		cout << lo+1 << ' ' << hi+1 << '\n';
	}

	return 0;
}
