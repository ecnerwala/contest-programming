#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	string S; cin >> S;
	std::vector<int> pref(N);
	pref[0] = 0;
	for (int i = 0; i+1 < N; i++) {
		pref[i+1] = pref[i] + (S[i+1] != S[i]);
	}
	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r;
		r--, l--;
		int num_diffs = pref[r] - pref[l] + (S[r] != S[l]);
		cout << (num_diffs + 1)/2 << '\n';
	}

	return 0;
}
