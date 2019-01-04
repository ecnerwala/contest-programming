#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll dp[3];
ll ndp[3];

ll go(ll N, ll I) {
	dp[0] = 1;
	dp[1] = 0;
	dp[2] = 0;
	while(N) {
		ll n = N % 3;
		for(ll v = 0; v < 3 && v <= I; v++) {
			ll i = (I - v) % 3;
			for(ll a = 0; a < 3; a++) {
				ll b = (i + a) % 3;
				assert((b + a + a) % 3 == i);
				assert((2 * a + b + v) % 3 == I % 3);
				assert(((2 * a + b + v) - I % 3) / 3 < 3);
				assert(((2 * a + b + v) - I % 3) / 3 < 3);
				if(a + b > n) {
					continue;
				}
				ll c = n - a - b;
				ll val = 1;
				if(bool(a) + bool(b) + bool(c) == 2) {
					val = 2;
				}
				ndp[((2 * a + b + v) - I % 3) / 3] += val * dp[v];
			}
		}
		for(ll v = 0; v < 3; v++) {
			dp[v] = ndp[v] % 3;
			ndp[v] = 0;
		}
		N /= 3;
		I /= 3;
	}
	assert(I < 3);
	return dp[I];
}

int main() {
	ll K;
	cin >> K;
	while(K--) {
		ll N, I;
		cin >> N >> I;
		cout << go(N, I) % 3 << '\n';
	}
	return 0;
}
