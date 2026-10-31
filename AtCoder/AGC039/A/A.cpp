#include<bits/stdc++.h>
using namespace std;

using ll = long long;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string S; cin >> S;
	int N = int(S.size());
	ll K; cin >> K;

	if (S == string(N, S[0])) {
		cout << (N * K) / 2 << '\n';
	} else {
		int prefLen = 0;
		for (int i = 0; S[i] == S[0]; i++) {
			prefLen ++;
		}
		int suffLen = 0;
		for (int i = N-1; S[i] == S[N-1]; i--) {
			suffLen ++;
		}
		int midVal = 0;
		for (int i = prefLen+1; i < N-suffLen; i++) {
			if (S[i] == S[i-1]) {
				midVal++;
				i++; // extra ++ to skip the next
			}
		}

		ll ans = midVal * K + (prefLen / 2) + (suffLen / 2) + (S[0] == S[N-1] ? (prefLen + suffLen) / 2 : prefLen/2 + suffLen / 2) * (K-1);
		cout << ans << '\n';
	}

	return 0;
}
