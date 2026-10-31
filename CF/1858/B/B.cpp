#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, D; cin >> N >> M >> D;
		std::vector<int> S; S.reserve(M+2); S.resize(M);
		for (auto& s : S) { cin >> s; }
		S.insert(S.begin(), 1);
		S.push_back(N+1);
		int64_t ans = 0;
		for (int i = 0; i+1 < int(S.size()); i++) {
			int diff = S[i+1] - S[i];
			ans += (diff + (D-1)) / D;
		}
		int64_t best = ans+1;
		int best_cnt = 0;
		for (int i = 0; i+2 < int(S.size()); i++) {
			int64_t cnd = ans - (S[i+1] - S[i] + (D-1)) / D - (S[i+2] - S[i+1] + (D-1)) / D + (S[i+2] - S[i] + (D-1)) / D;
			if (cnd < best) {
				best = cnd;
				best_cnt = 0;
			}
			if (cnd == best) best_cnt++;
		}
		cout << best << ' ' << best_cnt << '\n';
	}

	return 0;
}
