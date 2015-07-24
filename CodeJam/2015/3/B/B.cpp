#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2000;
const int MAXK = 200;

int N, K;
ll S[MAXN];
ll R[MAXK];

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cin >> N >> K;
		for(int i = 0; i < N - K + 1; i++) cin >> S[i];
		ll tot = S[0];
		for(int m = 0; m < K; m++) {
			ll val = 0;
			ll mi = 0;
			ll ma = 0;
			for(int i = m; i + K < N; i += K) {
				val += S[i + 1] - S[i];
				mi = min(mi, val);
				ma = max(ma, val);
			}

			//cerr << mi << ' ' << ma << '\n';
			// shift all values so min = 0
			tot += mi;
			R[m] = ma - mi;
		}

		//cerr << tot << '\n';

		tot %= K;
		if(tot < 0) tot += K;

		sort(R, R + K);
		
		for(int i = 0; i < K; i++) {
			tot -= R[K - 1] - R[i];
		}

		ll res = R[K - 1];
		if(tot > 0) res ++;

		cout << "Case #" << t << ": " << res << '\n';
	}
	return 0;
}
