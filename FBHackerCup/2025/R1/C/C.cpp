#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		// base answer: just the sum of the lengths
		int64_t ans = int64_t(N) * int64_t(N+1) * int64_t(N+2) / 6;

		std::vector<int> pref(N+1);
		for (int i = 0; i < N; i++) pref[i+1] = pref[i] ^ A[i];
		std::sort(pref.begin(), pref.end());
		for (int i = 0, j = 0; i < int(pref.size()); i = j) {
			while (j < int(pref.size()) && pref[j] == pref[i]) j++;
			int cnt = j - i;
			ans -= int64_t(cnt) * int64_t(cnt + 1) * int64_t(cnt + 2) / 6;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
