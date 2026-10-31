#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N); for (auto& a : A) cin >> a;
	std::sort(A.begin(), A.end());

	int ans = 0;
	int64_t sum = 0;
	for (int i = 0; i < N; i++) {
		sum += A[i];
		int64_t v = sum / (i+1) + 1;
		int cnt = int((A.begin() + i + 1) - std::lower_bound(A.begin(), A.begin() + i + 1, v));
		ans = std::max(ans, cnt);
	}
	cout << ans << '\n';
	return 0;
}
