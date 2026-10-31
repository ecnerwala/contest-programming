#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		array<int, 2> N; cin >> N[0] >> N[1];
		int L = min(N[0], N[1]);
		vector<int> pref(L+1);

		int ans = 0;
		for (int z = 0; z < 2; z++) {
			vector<int> S(N[z]);
			for (auto& v : S) { cin >> v; v &= 1; }
			for (int i = 0, v = 0; i < L; i++) {
				v ^= S[i];
				pref[i+1] ^= v;
			}

			vector<int> F(N[z]+1);
			F[0] = -1;
			for (int i = 0; i < N[z]; i++) {
				int cur = F[i];
				while (cur != -1 && S[cur] != S[i]) cur = F[cur];
				F[i+1] = cur+1;
			}

			vector<bool> is_border(N[z]+1, false);
			for (int cur = N[z]; cur != -1; cur = F[cur]) {
				is_border[cur] = true;
			}
			for (int l = L; l > ans; l--) {
				if (!is_border[N[z]-l]) ans = l;
			}
		}
		for (int l = L; l > ans; l--) {
			if (!pref[l]) ans = l;
		}
		cout << ans << '\n';
	}

	return 0;
}
