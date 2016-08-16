#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef long double ld;

unordered_map<ll, ld> dp;
ld C(ll a) {
	if(a == 0) return 0;
	if(a == 1) return 1;
	if(dp.count(a)) return dp[a];
	return dp[a] = a + C(a / 2) + C(a / 2 - 1 + a % 2);
}

ld B(ll a) {
	return C(a) / a;
}

ld Q(ll a) {
	return -3 * a + 2 * (a + 1) * 23.6030665948919897007855933L; // (sum i=1->a of 1/i)
}

ld R(ll a) {
	return Q(a) / a;
}

int main() {
	ll n = 10000000000ll;
	cout << fixed << setprecision(8) << R(n) - B(n) << '\n';
	return 0;
}
