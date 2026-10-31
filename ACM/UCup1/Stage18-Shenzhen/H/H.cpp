#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
		}
		std::vector<int> pref = A;
		std::vector<int> suff = A;
		for (int i = 0; i+1 < N; i++) {
			pref[i+1] &= pref[i];
		}
		for (int i = N-1; i-1 >= 0; i--) {
			suff[i-1] &= suff[i];
		}

		int ans = 0;
		for (int i = 0; i+1 < N; i++) {
			ans = std::max(ans, pref[i] + suff[i+1]);
		}
		for (int i = 0; i < N; i++) {
			ans = std::max(ans, pref[N-1] + A[i]);
		}

		for (int b = 0; b <= 29; b++) {
			if (pref[N-1] & (1 << b)) continue;
			int first = -1;
			int last = -1;
			for (int i = 0; i < N; i++) {
				if (!(A[i] & (1 << b))) {
					if (first == -1) first = i;
					last = i;
				}
			}
			if (first == -1) continue;
		}

		cout << ans << '\n';
	}

	return 0;
}
