#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;
	std::vector<int> bad_pref(N-1);
	for (int i = 1; i+1 < N; i++) {
		bad_pref[i] = bad_pref[i-1] + (A[i-1] >= A[i] && A[i] >= A[i+1]);
	}

	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r; l--;
		cout << (r - l) - (r-l >= 2 ? bad_pref[r-2] - bad_pref[l] : 0) << '\n';
	}

	return 0;
}
