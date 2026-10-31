#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> C(N);
	for (auto& c : C) cin >> c;

	std::vector<int> vals = C;
	std::nth_element(vals.begin(), vals.begin() + M - 1, vals.end());
	int v_max = vals[M-1];
	int num_max = 0;
	for (int i = 0; i < M; i++) {
		num_max += vals[i] == v_max;
	}
	assert(num_max > 0);

	int other_lo = N;
	int other_hi = -1;
	for (int i = 0; i < N; i++) {
		if (C[i] < v_max) {
			other_lo = std::min(other_lo, i);
			other_hi = std::max(other_hi, i);
		}
	}

	int ans = N+1;
	for (int i = 0, j = 0; true; ) {
		assert(num_max >= 0);
		if (num_max == 0) {
			assert(i < j);
			ans = std::min(ans, std::max(other_hi + 1, j) - std::min(other_lo, i));
			num_max += C[i] == v_max;
			i++;
		} else {
			if (j == N) break;
			num_max -= C[j] == v_max;
			j++;
		}
	}
	assert(ans <= N);
	cout << ans << '\n';

	return 0;
}
