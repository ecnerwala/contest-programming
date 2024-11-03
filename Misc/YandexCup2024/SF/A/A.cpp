#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::array<std::vector<int>, 2> A;
	A[0].resize(N);
	A[1].resize(N);
	for (int i = 0; i < N; i++) {
		cin >> A[0][i] >> A[1][i];
	}
	int64_t ans = 0;
	for (int z = 0; z < 2; z++) {
		std::sort(A[z].begin(), A[z].end());
		for (int i = 0; i < N; i++) {
			ans += std::abs(A[z][i] - (i+1));
		}
	}
	cout << ans << '\n';

	return 0;
}
