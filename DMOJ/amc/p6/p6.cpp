#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// The variance is M/4 - (N/4)^2
	int64_t N, M; cin >> N >> M;
	if (M * 4 - N * N < 0) {
		cout << "no" << '\n';
	} else {
		cout << fixed << setprecision(20) << N * 0.25 + std::sqrt(3 * (M * 4 - N * N)) / 4 << '\n';
	}

	return 0;
}
