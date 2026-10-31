#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, L; cin >> N >> L;
		vector<string> S(N);
		for (auto& s : S) cin >> s;
		vector<int> ans(L);
		map<string, int> mp;
		int nxt_idx = 1;
		for (int i = 0; i < L; i++) {
			string cur(N, '\0'); for (int j = 0; j < N; j++) cur[j] = S[j][i];
			auto& v = mp[cur];
			if (!v) v = nxt_idx++;
			ans[i] = v;
		}
		cout << nxt_idx - 1 << '\n';
		for (int i = 0; i < L; i++) {
			cout << ans[i] << " \n"[i+1==L];
		}
	}

	return 0;
}
