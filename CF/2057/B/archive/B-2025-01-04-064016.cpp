#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		std::vector<int> freq; freq.reserve(N);
		for (int i = 0, j = 0; i < N; i = j) {
			while (j < N && A[i] == A[j]) j++;
			freq.push_back(j - i);
		}
		std::sort(freq.begin(), freq.end());
		int ans = int(freq.size());
		for (auto v : freq) {
			if (K >= v) {
				K -= v;
				ans --;
			} else {
				break;
			}
		}
		ans = std::max(ans, 1);
		cout << ans << '\n';
	}

	return 0;
}
