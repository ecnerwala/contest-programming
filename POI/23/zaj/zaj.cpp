#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int> B(M); for (auto& b : B) cin >> b;

	vector<int> next_eq(M);
	{
		unordered_map<int, int> mp;
		for (int j = M-1; j >= 0; j--) {
			next_eq[j] = std::exchange(mp[B[j]], j);
			if (next_eq[j] == 0) next_eq[j] = -1;
		}
	}

	vector<int> cur_b_cnd(M, 0);

	vector<int> dp(M+1, 0);
	for (int i = 0; i < N; i++) {
		for (int j = M-1; j >= 0; j--) {
			if (A[i] == B[j]) {
				if (next_eq[j] != -1) {
					setmax(cur_b_cnd[next_eq[j]], dp[j]+2);
				}
				setmax(dp[j+1], cur_b_cnd[j]);
				// doesn't really matter
				cur_b_cnd[j] = 0;
			}
		}
		for (int j = 0; j < M; j++) {
			setmax(dp[j+1], dp[j]);
		}
	}

	cout << dp[M] << '\n';

	return 0;
}
