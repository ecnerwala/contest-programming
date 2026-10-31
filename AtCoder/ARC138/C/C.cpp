#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first;
		A[i].second = i;
	}
	std::nth_element(A.begin(), A.begin() + N / 2, A.end());
	int64_t ans = 0;
	for (int i = N/2; i < N; i++) {
		ans += A[i].first;
	}
	std::vector<int> val(N, -1);
	for (int i = 0; i < N/2; i++) {
		val[A[i].second] = 1;
	}
	std::vector<int> pref(N);
	for (int i = 1; i < N; i++) {
		pref[i] = pref[i-1] + val[i-1];
	}
	int k = int(std::min_element(pref.begin(), pref.end()) - pref.begin());
	cout << k << ' ' << ans << '\n';

	return 0;
}
