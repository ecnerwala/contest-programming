#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> R(N);
	std::vector<int> C(N);
	for (int i = 0; i < N; i++) cin >> R[i] >> C[i];
	std::sort(R.begin(), R.end());
	std::sort(C.begin(), C.end());
	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		ans += std::abs(i+1 - R[i]);
		ans += std::abs(i+1 - C[i]);
	}
	cout << ans << '\n';

	return 0;
}
