#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		N += 2;
		std::string S; cin >> S;
		S = 'B' + S + 'B';
		assert(int(S.size()) == N);
		std::vector<int64_t> X(N);
		for (int i = 1; i < N; i++) {
			if (i == 1 || i == N-1) {
				X[i] = 0;
			} else {
				cin >> X[i];
			}
			X[i] += X[i-1];
		}

		for (int i = 0; i < N; i++) {
			cerr << X[i] << ' ';
		}
		cerr << '\n';

		std::vector<int64_t> alternating_Xs(N+2);
		for (int i = 0; i < N; i++) {
			alternating_Xs[i+2] = alternating_Xs[i] + X[i];
		}

		std::vector<int> balance_A(N+1);
		balance_A[0] = N;
		std::vector<int> As; As.reserve(N);
		std::vector<int64_t> pref_A_Xs(N+1);
		for (int i = 0; i < N; i++) {
			if (S[i] == 'A') {
				As.push_back(i);
				pref_A_Xs.push_back(pref_A_Xs.back() + X[i]);
			}
			balance_A[i+1] = balance_A[i] + (S[i] == 'A' ? +1 : -1);
		}

		int L = int(As.size());
		std::vector<std::array<int, 2>> jumps(L);
		{
			std::vector<int> last_val(2*N+1, -1);
			int l = 0;
			for (int i = 0; i < N; i++) {
				last_val[balance_A[i]] = i;
				if (S[i] == 'A') {
					jumps[l++][0] = last_val[balance_A[i+1]];
				}
			}
			assert(l == L);
		}
		{
			std::vector<int> last_val(2*N+1, N);
			int l = L;
			for (int i = N-1; i >= 0; i--) {
				last_val[balance_A[i+1]] = i;
				if (S[i] == 'A') {
					jumps[--l][1] = last_val[balance_A[i]];
				}
			}
			assert(l == 0);
		}

		const int64_t INF = 2e18;
		std::vector<int64_t> dp(N, INF);
		dp[0] = 0;
		int l = 0;
		for (int i = 0; i+1 < N; i++) {
			if (S[i] == 'B' && S[i+1] == 'B') {
				setmin(dp[i+1], dp[i]);
			}
			if (S[i] == 'A') {
				assert(As[l] == i);
				auto [j0, j1] = jumps[l];
				if (j0 >= 0 && j1 < N) {
					assert(S[j0] == 'B');
					assert(S[j1] == 'B');
					int left_As = (i - j0 - 1) / 2;
					int right_As = (j1 - i - 1) / 2;
					int64_t right_tgt = alternating_Xs[j1 + 1] - alternating_Xs[i];
					int64_t right_src = pref_A_Xs[l+right_As+1] - pref_A_Xs[l];
					int64_t left_src = pref_A_Xs[l] - pref_A_Xs[l - left_As];
					int64_t left_tgt = alternating_Xs[i] - alternating_Xs[j0+1];
					int64_t cost = (left_src - left_tgt) + (right_tgt - right_src);

					cerr << l << ' ' << i << ' ' << j0 << ' ' << j1 << ' ' << cost << ' ' << left_tgt << "<-" << left_src << ' ' << right_src << "->" << right_tgt << '\n';
					setmin(dp[j1], dp[j0] + cost);
				}
				l++;
			}
		}
		assert(l == L);
		cout << dp[N-1] << '\n';
	}

	return 0;
}
