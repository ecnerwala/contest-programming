#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> B(N);
	for (auto& b : B) cin >> b;

	std::sort(A.begin(), A.end());
	std::sort(B.begin(), B.end());

	std::vector<int> pref(N+1);
	for (int i = 0; i < N; i++) {
		int b = 0;
		int first_win = std::upper_bound(B.begin(), B.end(), A[i]) - B.begin();
		if (first_win <= i) {
			 pref[i - first_win] ++;
		}
		int first_loss = int(std::lower_bound(B.begin(), B.end(), A[i]) - B.begin());
		pref[0]--;
		if (first_loss <= i) {
			pref[i - first_loss]++;
		}
	}
	int ans = -N;
	int cur = 0;
	for (auto v : pref) {
		cur += v;
		ans = std::max(ans, cur);
	}
	cout << ans << '\n';

	return 0;
}
