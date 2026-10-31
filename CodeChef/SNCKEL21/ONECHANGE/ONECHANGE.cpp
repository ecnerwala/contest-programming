#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

const int L = 26;
int dp[1 << L];

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);


	string S; cin >> S;
	int N = int(S.size());
	std::vector<std::array<int, L>> nxt(N);
	for (int i = N-1; i >= 0; i--) {
		if (i == N-1) {
			for (int z = 0; z < L; z++) {
				nxt[i][z] = N;
			}
		} else {
			nxt[i] = nxt[i+1];
		}
		nxt[i][S[i]-'a'] = i;
	}
	int ans = 0;
	for (int msk = 0; msk < (1 << L); msk++) {
		int c = dp[msk] / N;
		int p = dp[msk] % N;
		setmax(ans, c);
		for (int v = 0; v < L; v++) {
			if (msk & (1 << v)) continue;
			setmax(dp[msk | (1 << v)], c * N + nxt[p][v]);
		}
	}
	cout << 26 - ans << '\n';

	return 0;
}
