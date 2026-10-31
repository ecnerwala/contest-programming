#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S, T;
	while (cin >> S >> T) {
		int N = int(S.size());
		int M = int(T.size());

		reverse(S.begin(), S.end());
		reverse(T.begin(), T.end());

		vector<int> Tfail(M+1);
		{
			Tfail[0] = -1;
			for (int i = 0; i < M; i++) {
				int cur = Tfail[i];
				while (cur != -1 && T[cur] != T[i]) {
					cur = Tfail[cur];
				}
				Tfail[i+1] = cur+1;
			}
		}
		int best_match = 0;
		vector<bool> good_suff(N+1);
		{
			int cur = 0;
			for (int i = 1; i < N; i++) {
				while (cur == M || (cur != -1 && S[i] != T[cur])) cur = Tfail[cur];
				cur++;
				if (cur == M) {
					// mark this suffix as good
					good_suff[i+1] = true;
				}
			}
			best_match = cur;
		}
		std::reverse(good_suff.begin(), good_suff.end());
		std::reverse(S.begin(), S.end());

		vector<int> Sfail(N+1);
		Sfail[0] = -1;
		for (int i = 0; i < N; i++) {
			int cur = Sfail[i];
			while (cur != -1 && S[cur] != S[i]) {
				cur = Sfail[cur];
			}
			Sfail[i+1] = cur+1;
		}

		vector<int> ans(N+1, -1);
		ans[0] = best_match;
		for (int i = 0; i < N; i++) {
			if (i) {
				if (good_suff[i]) {
					ans[i] = M + i;
				} else {
					ans[i] = ans[Sfail[i]];
				}
			} else {
				ans[i] = best_match;
			}
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
