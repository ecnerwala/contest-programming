#include<bits/stdc++.h>
using namespace std;

const int MOD = int(1e9)+7;

int main() {
	if (fopen("help.in", "r")) {
		freopen("help.in", "r", stdin);
		freopen("help.out", "w", stdout);
	}

	int N; cin >> N;
	vector<int> p2(N+1);
	p2[0] = 1;
	for (int i = 0; i < N; i++) p2[i+1] = p2[i] * 2 % MOD;
	vector<int> pref(2*N);
	for (int i = 0; i < N; i++) {
		int l, r; cin >> l >> r;
		l--, r--;
		pref[l]++;
		pref[r]--;
	}
	for (int i = 0; i+1 < 2 * N; i++) {
		pref[i+1] += pref[i];
	}
	int ans = 0;
	for (int i = 1; i < 2*N; i++) {
		if (pref[i] < pref[i-1]) {
			// endpoint
			int numBad = pref[i];
			ans = (ans + p2[N - numBad - 1]) % MOD;
		}
	}
	cout << ans << '\n';

	return 0;
}
