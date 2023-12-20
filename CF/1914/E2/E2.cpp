#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		int64_t ans = 0;
		for (auto& a : A) {
			int b; cin >> b; b--;
			ans -= b;
			a += b;
		}
		std::sort(A.begin(), A.end(), std::greater<int>());
		for (int i = 0; i < N; i += 2) ans += A[i];
		cout << ans << '\n';
	}

	return 0;
}
