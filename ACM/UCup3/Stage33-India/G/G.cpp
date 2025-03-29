#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	
	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::string S; S.reserve(N); cin >> S;
		assert(int(S.size()) == N);
		std::vector<int> nxt_stop(N);
		for (int i = N-1; i >= 0; i--) {
			if (i == N-1 || S[i] < S[i+1]) {
				nxt_stop[i] = i;
			} else {
				nxt_stop[i] = nxt_stop[i+1];
			}
		}
		std::vector<int> first_eq(N);
		for (int i = 0; i < N; i++) {
			if (i == 0 || S[i] != S[i-1]) {
				first_eq[i] = i;
			} else {
				first_eq[i] = first_eq[i-1];
			}
		}
		for (int q = 0; q < Q; q++) {
			int l, r; cin >> l >> r; l--, r--;
			int e = std::min(r, nxt_stop[l]);
			int ans = std::max(l, first_eq[e]);
			cout << ans+1 << '\n';
		}
	}

	return 0;
}
