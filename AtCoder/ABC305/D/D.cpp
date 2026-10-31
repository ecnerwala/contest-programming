#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int> pref(N+1);
	for (int i = 0; i < N; i++) {
		pref[i+1] = pref[i] + ((i & 1) ? -A[i] : A[i]);
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r;
		int i = int(std::lower_bound(A.begin(), A.end(), l) - A.begin());
		int j = int(std::lower_bound(A.begin(), A.end(), r) - A.begin());

		int64_t res = int64_t(pref[j]) - int64_t(pref[i]);
		if (i & 1) {
		} else {
			res -= l;
		}

		if (j & 1) {
		} else {
			res += r;
		}
		cout << res << '\n';
	}

	return 0;
}
