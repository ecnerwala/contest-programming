#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;
		std::sort(A.begin(), A.end(), std::greater<>());
		std::sort(B.begin(), B.end(), std::greater<>());

		auto it = A.begin();
		int cnt = 0;
		int64_t ans = 1;
		for (int b : B) {
			while (it != A.end() && *it > b) {
				cnt++;
				++it;
			}
			ans *= cnt;
			ans %= int(1e9)+7;
			cnt--;
		}
		cout << ans << '\n';
	}

	return 0;
}
