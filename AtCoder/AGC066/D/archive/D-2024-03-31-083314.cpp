#include <bits/stdc++.h>

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
			std::vector<int> last_val(2*N+1);
			int l = 0;
			for (int i = 0; i < N; i++) {
				last_val[balance_A[i]] = i;
				if (S[i] == 'A') {
					jumps[l++][0] = last_val[balance_A[i+1]] - 1;
				}
			}
			assert(l == L);
		}
		{
			std::vector<int> last_val(2*N+1);
			int l = L;
			for (int i = N-1; i >= 0; i--) {
				last_val[balance_A[i+1]] = i+1;
				if (S[i] == 'A') {
					jumps[l++][0] = last_val[balance_A[i+1]] - 1;
				}
			}
			assert(l == L);
		}
	}

	return 0;
}
