#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 987654321;

ll Q(ll n) {
	assert(n);
	if(n == 1) return 0;
	if(n == 2 || n == 3) return 1;
	return ((n & 2) + 1 + (Q(n / 4) << 2)) % MOD;
}

ll P(ll n) {
	return Q(n) + 1;
}


ll T(ll n) {
	if(n == 0) return 0;
	else if(n % 4 != 3) return Q(n) + T(n - 1);
	return (Q(1) + Q(2) + Q(3) + T(n / 4) * 4 * 4 % MOD + (n / 4) * (1 + 1 + 3 + 3) % MOD) % MOD;
}

ll S(ll n) {
	return (T(n) + n) % MOD;
}

int main() {
	assert(P(1) == 1);
	assert(P(9) == 6);
	assert(P(1000) == 510);
	assert(S(1000) == 268271);
	
	cout << S(ll(1e18)) << '\n';

	return 0;
}
